/*============================================================================================*/
/* application of the kinematic model of the robot to determine its linear and angular speeds */
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
#include "../../header/Kinematics.h"
/*----------*/
/* globales */
/*----------*/
int     GoOn = 1;           /* ->controle d'execution du processus             */
Robot _Robot;
/*&&&&&&&&&&&&&&&&&&&&&&&&*/
/* gestionnaire de signal */
/*&&&&&&&&&&&&&&&&&&&&&&&&*/
void SignalHandler( int signal )
{
    if( signal == SIGALRM)
    {
        updateVelocity(&_Robot);
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
    double  Te;     // periode d'echantillonnage
    double  R0;     // rayon commun des roues du robot
    double  w;      // entraxe des roues
    double * argAdress[NB_ARGS] = {&R0, &w, &Te};
    /*.................*/
    /* other Arguments */
    /*.................*/
    int     iFdStateL;                      /* ->descripteur pour la zone d'etat moteur gauche                */
    int     iFdStateR;                      /* ->descripteur pour la zone d'etat moteur droit                 */
    int     iFdVelocity;                    /* ->descripteur pour la zone des vitesses                        */
    int     iLoops = 0;                     /* ->compte le nombre d'iterations effectuees                     */
    double  *lpdb_stateL;                   /* ->pointeur sur la zone partagee contenant l'etat moteur gauche */
    double  *lpdb_stateR;                   /* ->pointeur sur la zone partagee contenant l'etat moteur droit  */
    struct sigaction    sa;                 /* ->gestion du signal handler                                    */
    struct sigaction    sa_old;             /* ->gestion du signal handler                                    */
    sigset_t            mask;               /* ->liste des signaux a masquer                                  */
    struct itimerval    sTime;              /* ->periode du timer                                             */
    /*.......*/
    /* check */
    /*.......*/
    if( argc != NB_ARGS)
    {
        usage(argv[0]);
        return( 0 );
    };
    /*.............................*/
    /*  verification des arguments */
    /*.............................*/
    if( argc != NB_ARGS)
    {
        usage( argv[0]);
        return( 0 );
    };
    for (int i = 1; i < argc; i++)
    {
        if(sscanf(argv[i], "%lf",argAdress[i-1]) == 0){
            fprintf(stderr,"%s.main()  : ERREUR ---> The #%i argument must be a real number\n", argv[0], i);
            usage(argv[0]);
            return( 0 );
        }
    }
    /*............*/
    /* Init robot */
    /*............*/
    _Robot = initRobot(Te,R0,w);
    /*................................................*/
    /* lien / creation aux zones de memoire partagees */
    /*................................................*/
    if( (lpdb_stateL = (double *)(Link2SharedMem(STATE_L, 2 * sizeof(double), &iFdStateL, 1 ))) == NULL )
    {
        fprintf(stderr,"%s.main()  : ERREUR ---> appel a Link2SharedMem() #1\n", argv[0]);
        return( 0 );
    };
    if( (lpdb_stateR = (double *)(Link2SharedMem(STATE_R, 2 * sizeof(double), &iFdStateR, 1 ))) == NULL )
    {
        fprintf(stderr,"%s.main()  : ERREUR ---> appel a Link2SharedMem() #2\n", argv[0]);
        return( 0 );
    };
    if( (_Robot.wG = (double *)(Link2SharedMem(VELOCITY, 2 * sizeof(double), &iFdVelocity, 1 ))) == NULL )
    {
        fprintf(stderr,"%s.main()  : ERREUR ---> appel a Link2SharedMem() #3\n", argv[0]);
        return( 0 );
    };
    _Robot.wL  = &lpdb_stateL[OFFSET_W];
    _Robot.wR  = &lpdb_stateR[OFFSET_W];
    /*.................*/
    /* initialisations */
    /*.................*/
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
        /*___________________________________________________________________*/
        #if defined(USR_DBG)
                if( (iLoops % (int)(REFRESH_RATE)) == 0)
                {
                    printf("Vc = %lf Wc = %lf \n", lpdb_vel[0], lpdb_vel[1]);
                };
                iLoops++;
        #endif
        /*___________________________________________________________________*/
    }
    return( 0 );   
}