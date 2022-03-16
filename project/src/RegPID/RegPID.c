/*============================================================================================*/
/*                          processus of regulation PID d'un moteur CC                        */
/*--------------------------------------------------------------------------------------------*/
/*         Inspired by the example given by J.BOONAERT AMSE 2021-2022                         */
/*                           Killian ALLAIRE fise2023                                         */
/*============================================================================================*/
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <fcntl.h>
#include <signal.h>
#include <errno.h>
#include <sys/mman.h>
#include <sys/time.h>
#include "../../header/RegPID.h"
/*----------*/
/* globales */
/*----------*/
int     GoOn = 1;           /* ->controle d'execution du processus            */
PID _pid;                   /* ->PID                                          */
/*&&&&&&&&&&&&&&&&&&&&&&&&*/
/* gestionnaire de signal */
/*&&&&&&&&&&&&&&&&&&&&&&&&*/
void SignalHandler( int signal )
{
    if( signal == SIGALRM)
    {
        updateCommand(&_pid);
    };
}
/*######*/
/* MAIN */
/*######*/
int main( int argc, char *argv[])
{
    /*................................*/
    /* Arguments needed to be entered */
    /*................................*/
    double  Te;                             /* ->periode d'echantillonnage                    */
    double  Kcoeff;                         /* ->action proportionnelle                       */
    double  Icoeff;                         /* ->action integrale                             */
    double  Dcoeff;                         /* ->action derivee                               */
    double * argAdress[NB_ARGS-1] = {&Kcoeff, &Icoeff, &Dcoeff, &Te};
    char    cDriveID;                       /* ->caractere pour identifier le moteur            */

    char    szCmdAreaName[STR_LEN];         /* ->nom de la zone contenant la commande           */
    char    szStateAreaName[STR_LEN];       /* ->nom de la zone contenant l'etat du moteur      */
    char    szTargetAreaName[STR_LEN];      /* ->nom de la zone contenant la consigne           */
    int     iFdCmd;                         /* ->descripteur pour la zone de commande           */
    int     iFdState;                       /* ->descripteur pour la zone d'etat                */
    int     iFdTarget;                      /* ->descripteur pour la zone de consigne           */
    int     iLoops = 0;                     /* ->compte le nombre d'iterations effectuees       */
    double  *lpdb_state;                    /* ->pointeur sur la zone partagee contenant l'etat */
    double  r;                              /* ->resistance de l'induit                         */
    double  l;                              /* ->inductance                                     */
    double  Ke;                             /* ->constante electrique                           */
    double  Km;                             /* ->constante moteur                               */
    double  f;                              /* ->coefficient de frottement                      */
    double  j;                              /* ->inertie totale au rotor                        */
    struct sigaction    sa;                 /* ->gestion du signal handler                      */
    struct sigaction    sa_old;             /* ->gestion du signal handler                      */
    sigset_t            mask;               /* ->liste des signaux a masquer                    */
    struct itimerval    sTime;              /* ->periode du timer                               */
    /*.............................*/
    /*  verification des arguments */
    /*.............................*/
    if( argc != NB_ARGS)
    {
        usage( argv[0]);
        return( 0 );
    };
    for (int i = 1; i < argc-1; i++)
    {
        if(sscanf(argv[i], "%lf",argAdress[i-1]) == 0){
            fprintf(stderr,"%s.main()  : ERREUR ---> The #%i argument must be a real number\n", argv[0], i);
            usage(argv[0]);
            return( 0 );
        }
    }
    if( sscanf(argv[5],"%c",&cDriveID) == 0 )
    {
        fprintf(stderr,"%s.main()  : ERREUR ---> l'argument #8 doit etre un caractere\n", argv[0]);
        usage(argv[0]);
        return( 0 );
    };
    printf("Kcoeff = %lf\tIcoeff = %lf\tDcoeff = %lf\n", Kcoeff, Icoeff, Dcoeff);

    _pid = initPID( Te,  Kcoeff,  Icoeff,  Dcoeff);

    /*................................................*/
    /* lien / creation aux zones de memoire partagees */
    /*................................................*/
    sprintf(szCmdAreaName,"%s%c", CMD_BASENAME, cDriveID);
    if( (_pid.lpdb_u = (double *)(Link2SharedMem(szCmdAreaName, sizeof(double), &iFdCmd, 1 ))) == NULL )
    {
        fprintf(stderr,"%s.main()  : ERREUR ---> appel a Link2SharedMem() #1\n", argv[0]);
        return( 0 );
    };
    sprintf(szStateAreaName,"%s%c", STATE_BASENAME, cDriveID);
    if( (lpdb_state = (double *)(Link2SharedMem(szStateAreaName, 2 * sizeof(double), &iFdState, 1 ))) == NULL )
    {
        fprintf(stderr,"%s.main()  : ERREUR ---> appel a Link2SharedMem() #2\n", argv[0]);
        return( 0 );
    };
    _pid.lpdb_w = &lpdb_state[OFFSET_W];
    _pid.lpdb_i = &lpdb_state[OFFSET_I];
    sprintf(szTargetAreaName,"%s%c", TARGET_BASENAME, cDriveID);
    if( (_pid.lpdb_Tv = (double *)(Link2SharedMem(szTargetAreaName, sizeof(double), &iFdTarget, 1 ))) == NULL )
    {
        fprintf(stderr,"%s.main()  : ERREUR ---> appel a Link2SharedMem() #3\n", argv[0]);
        return( 0 );
    };
    *_pid.lpdb_Tv = 0.0;
    /*.................*/
    /* initialisations */
    /*.................*/
    _pid.e       = 0.0;
    _pid.e_prev  = 0.0;
    _pid.Ie      = 0.0;
    _pid.Ie_prev = 0.0;
    _pid.De      = 0.0;
    /*............................................*/
    /* installation de la routine d'interception  */
    /*............................................*/
    memset(&sa,0,sizeof(struct sigaction));
    sigemptyset( &mask );
    sa.sa_mask = mask;
    sa.sa_handler = SignalHandler;
    sa.sa_flags = 0;
    if( sigaction(SIGALRM, &sa, &sa_old) < 0 )
    {
        fprintf(stderr,"%s.main() :  ERREUR ---> appel a sigaction() #1\n", argv[0]);
        fprintf(stderr,"             code = %d (%s)\n", errno, (char *)(strerror(errno)));
        exit( -errno );
    };
    /*........................*/
    /* configuration du timer */
    /*........................*/
    sTime.it_interval.tv_sec = (int)(Te);
    sTime.it_interval.tv_usec = (int)((Te - (int)(Te))*1e6);
    
    sTime.it_value.tv_sec = (int)(Te);
    sTime.it_value.tv_usec = (int)((Te - (int)(Te))*1e6);
    
    if( setitimer( ITIMER_REAL, &sTime, NULL) < 0 )
     {
        fprintf(stderr,"%s.main() :  ERREUR ---> appel a setitimer() \n", argv[0]);
        fprintf(stderr,"             code = %d (%s)\n", errno, (char *)(strerror(errno)));
        exit( -errno );
    };
    /*************************/
    /* fonctionnement normal */
    /*************************/
    while( GoOn)
    {
        pause();
        /*___________________________________________________*/
        #if defined(USR_DBG)
                if( (iLoops % (int)(REFRESH_RATE)) == 0)
                {
                    printf("Tv = %lf w = %lf \n", *lpdb_Tv, *lpdb_w);
                };
                iLoops++;
        #endif
        /*___________________________________________________*/
    }
    return( 0 );
}