/*=============================================================================*/
/* Processus that simulates the output of a DCmotor depending on the intensity */
/*-----------------------------------------------------------------------------*/
/*         Inspired by the example given by J.BOONAERT AMSE 2021-2022          */
/*                           Killian ALLAIRE fise2023                          */
/*=============================================================================*/

#include "../../header/DCmotor.h"                                         */

/*----------*/
/* globales */
/*----------*/
int     GoOn = 1;       /* ->controle d'execution du processus */
DCmotor _DCMotor;
/*&&&&&&&&&&&&&&&&&&&&&&&&*/
/* gestionnaire de signal */
/*&&&&&&&&&&&&&&&&&&&&&&&&*/
void SignalHandler( int signal)
{
    if( signal == SIGALRM)
    {
        updateState(&_DCMotor);
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
    double  r;                              /* ->resistance de l'induit                         */
    double  l;                              /* ->inductance                                     */
    double  Ke;                             /* ->constante electrique                           */
    double  Km;                             /* ->constante moteur                               */
    double  f;                              /* ->coefficient de frottement                      */
    double  j;                              /* ->inertie totale au rotor                        */
    double  Te;                             /* ->periode d'echantillonnage                      */
    double * argAdress[NB_ARGS-1] = {&r, &l, &Ke, &Km, &f, &j, &Te};
    char    cDriveID;                       /* ->caractere pour identifier le moteur            */
    /*.................*/
    /* other Arguments */
    /*.................*/
    char    szCmdAreaName[STR_LEN];         /* ->nom de la zone contenant la commande           */
    char    szStateAreaName[STR_LEN];       /* ->nom de la zone contenant l'etat du moteur      */
    int     iFdCmd;                         /* ->descripteur pour la zone de commande           */
    int     iFdState;                       /* ->descripteur pour la zone d'etat                */
    int     iLoops = 0;                     /* ->compte le nombre d'iterations effectuees       */
    double  *lpdb_state;                    /* ->pointeur sur la zone partagee contenant l'etat */
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
    if( sscanf(argv[NB_ARGS-1],"%c",&cDriveID) == 0 )
    {
        fprintf(stderr,"%s.main()  : ERREUR ---> l'argument #8 doit etre un caractere\n", argv[0]);
        usage(argv[0]);
        return( 0 );
    };
    /*.....................*/
    /* Creating the modele */
    /*.....................*/
    _DCMotor = initModel(r,l,Ke,Km,f,j,Te);
    /*................................................*/
    /* lien / creation aux zones de memoire partagees */
    /*................................................*/
    sprintf(szCmdAreaName,"%s%c", CMD_BASENAME, cDriveID);
    sprintf(szStateAreaName,"%s%c", STATE_BASENAME, cDriveID);
    /* zone de commande */
    if(( iFdCmd = shm_open(szCmdAreaName, O_RDWR, 0600)) < 0 )
    {
        if(( iFdCmd = shm_open(szCmdAreaName, O_RDWR | O_CREAT, 0600)) < 0)
        {
            fprintf(stderr,"%s.main() :  ERREUR ---> appel a shm_open() #1\n", argv[0]);
            fprintf(stderr,"             code = %d (%s)\n", errno, (char *)(strerror(errno)));
            exit( -errno );
        }
        else
        {
            printf("CREATION de la zone %s\n", szCmdAreaName);
        };
    }
    else
    {
        printf("LIEN a la zone %s\n", szCmdAreaName);
    };
    if( ftruncate(iFdCmd, sizeof(double)) < 0 )
    {
        fprintf(stderr,"%s.main() :  ERREUR ---> appel a ftruncate() #1\n", argv[0]);
        fprintf(stderr,"             code = %d (%s)\n", errno, (char *)(strerror(errno)));
        exit( -errno );
    };
    if((_DCMotor.u = (double *)(mmap(NULL, sizeof(double), PROT_READ | PROT_WRITE, MAP_SHARED, iFdCmd, 0))) == MAP_FAILED )
    {
        fprintf(stderr,"%s.main() :  ERREUR ---> appel a mmap() #1\n", argv[0]);
        fprintf(stderr,"             code = %d (%s)\n", errno, (char *)(strerror(errno)));
        exit( -errno );
    };
    /* zone d'etat */
    if(( iFdState = shm_open(szStateAreaName, O_RDWR, 0600)) < 0 )
    {
        if(( iFdState = shm_open(szStateAreaName, O_RDWR | O_CREAT, 0600)) < 0)
        {
            fprintf(stderr,"%s.main() :  ERREUR ---> appel a shm_open() #2\n", argv[0]);
            fprintf(stderr,"             code = %d (%s)\n", errno, (char *)(strerror(errno)));
            exit( -errno );
        }else{
            printf("CREATION de la zone %s\n", szStateAreaName);
        };
    }else{
        printf("LIEN a la zone %s\n", szStateAreaName);
    };
    if( ftruncate(iFdState, 2 * sizeof(double)) < 0 )
    {
        fprintf(stderr,"%s.main() :  ERREUR ---> appel a ftruncate() #2\n", argv[0]);
        fprintf(stderr,"             code = %d (%s)\n", errno, (char *)(strerror(errno)));
        exit( -errno );
    };
    if((lpdb_state = (double *)(mmap(NULL, 2 * sizeof(double), PROT_READ | PROT_WRITE, MAP_SHARED, iFdState, 0))) == MAP_FAILED )
    {
        fprintf(stderr,"%s.main() :  ERREUR ---> appel a mmap() #2\n", argv[0]);
        fprintf(stderr,"             code = %d (%s)\n", errno, (char *)(strerror(errno)));
        exit( -errno );
    };
    _DCMotor.w = &lpdb_state[OFFSET_W];
    _DCMotor.i = &lpdb_state[OFFSET_I];
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
    sTime.it_interval.tv_sec = (int)(_DCMotor.Te);
    sTime.it_interval.tv_usec = (int)((_DCMotor.Te - (int)(_DCMotor.Te))*1e6);
    sTime.it_value.tv_sec = (int)(_DCMotor.Te);
    sTime.it_value.tv_usec = (int)((_DCMotor.Te - (int)(_DCMotor.Te))*1e6);
    
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
        /*_________________________________________________________________*/
        #if defined(USR_DBG)
        if( (iLoops % (int)(REFRESH_RATE)) == 0)
        {
            printf("u = %lf w = %lf i = %lf\n", *lpdb_u, *lpdb_w, *lpdb_i);
        };
        iLoops++;
        /*_________________________________________________________________*/
        #endif
    }
    return( 0 );   
}