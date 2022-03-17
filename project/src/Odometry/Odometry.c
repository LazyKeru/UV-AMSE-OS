/*============================================================================================*/
/* application of the kinematic model of the robot to determine its linear and angular speeds */
/*--------------------------------------------------------------------------------------------*/
/*         Inspired by the example given by J.BOONAERT AMSE 2021-2022                         */
/*                           Killian ALLAIRE fise2023                                         */
/*============================================================================================*/

#include "../../header/Odometry.h"

/*----------*/
/* globales */
/*----------*/
int     GoOn = 1;           /* ->controle d'execution du processus             */
Cordonne _cordonne;
/*&&&&&&&&&&&&&&&&&&&&&&&&*/
/* gestionnaire de signal */
/*&&&&&&&&&&&&&&&&&&&&&&&&*/
void SignalHandler( int signal )
{
    if( signal == SIGALRM)
    {
        updateOrientation(&_cordonne);
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
    double startingX, startingY, starting0;
    double  Te;     // periode d'echantillonnage
    double * argAdress[NB_ARGS-1] = {&Te, &startingX, &startingY, &starting0};
    /*.................*/
    /* other Arguments */
    /*.................*/
    struct sigaction    sa;                 /* ->gestion du signal handler                                    */
    struct sigaction    sa_old;             /* ->gestion du signal handler                                    */
    sigset_t            mask;               /* ->liste des signaux a masquer                                  */
    struct itimerval    sTime;              /* ->periode du timer                                             */
    double  * cordonne;                     /* ->pointeur sur la zone partagee des cordonne                   */
    int     iFdStateCordonne;               /* ->descripteur pour la zone partagee des cordonne               */
    int     iFdVelocity;                    /* ->descripteur pour la zone des vitesses                        */
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
            usage( argv[0]);
            return( 0 );
        }
    }
    /*............*/
    /* Init robot */
    /*............*/
    _cordonne = initCordonne(Te);
    /*................................................*/
    /* lien / creation aux zones de memoire partagees */
    /*................................................*/
    if( (_cordonne.lpdb_vel = (double *)(Link2SharedMem(VELOCITY, 2 * sizeof(double), &iFdVelocity, 1 ))) == NULL )
    {
        fprintf(stderr,"%s.main()  : ERREUR ---> appel a Link2SharedMem() #3\n", argv[0]);
        return( 0 );
    };
    if( (cordonne = (double *)(Link2SharedMem(CORDONNE, 3 * sizeof(double), &iFdStateCordonne, 1 ))) == NULL )
    {
        fprintf(stderr,"%s.main()  : ERREUR ---> appel a Link2SharedMem() #3\n", argv[0]);
        return( 0 );
    };
    _cordonne.x = &cordonne[OFFSET_X];
    _cordonne.y = &cordonne[OFFSET_Y];
    _cordonne.O = &cordonne[OFFSET_O];
    /*.................*/
    /* initialisations */
    /*.................*/
    setCord(startingX, startingY, starting0, &_cordonne);
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