/*=============================================================================*/
/* Processus that simulates the output of a DCmotor depending on the intensity */
/*-----------------------------------------------------------------------------*/
/*         Inspired by the example given by J.BOONAERT AMSE 2021-2022          */
/*                           Killian ALLAIRE fise2023                          */
/*=============================================================================*/
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include "DCmotor.h"
/*.......*/
/* const */
/*.......*/
#define CMD_BASENAME    "COMMAND_"
#define STATE_BASENAME  "STATE_"
#define NB_ARGS         10              /* ->Number of arguments expected  */
#define STR_LEN         64              /* ->taille des chaines par defaut */
/*#####################################*/
/* Enter double u, Returns int i and w */
/*#####################################*/
int main(int argc, char *argv[])
{
    char    cDriveID;                       /* ->caractere pour identifier le moteur            */
    char    szCmdAreaName[STR_LEN];         /* ->nom de la zone contenant la commande           */
    char    szStateAreaName[STR_LEN];       /* ->nom de la zone contenant l'etat du moteur      */
    int     iFdCmd;                         /* ->descripteur pour la zone de commande           */
    int     iFdState;                       /* ->descripteur pour la zone d'etat                */
    /*......................*/
    /*  physical arguments  */
    /*......................*/
    double R;                               /* ->resistance                 */
    double L;                               /* ->inductance                 */
    double Ke;                              /* ->constante electrique       */
    double Km;                              /* ->constante moteur           */
    double f;                               /* ->coefficient de frottements */
    double J;                               /* ->inertie totale             */
    double Te;                              /* ->periode d'echantilonnage   */
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
        if(sscanf(argv[i], "%lf",&R) == 0){
            fprintf(stderr,"%s.main()  : ERREUR ---> The #%i argument must be a real number\n", argv[0], i);
            usage(argv[0]);
            return( 0 );
        }
    }
    /*********************/
    /**    initModel    **/
    /*********************/
    DCmotor dcm = initModel(R,L,Ke,Km,f,J,Te);
    /********************************************/
    /** Link / creation of shared memory space **/
    /********************************************/
    sprintf(szCmdAreaName,"%s%c", CMD_BASENAME, cDriveID);
    sprintf(szStateAreaName,"%s%c", STATE_BASENAME, cDriveID);
    /*********************/
    /**  Commande zone  **/
    /*********************/
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
    if((lpdb_u = (double *)(mmap(NULL, sizeof(double), PROT_READ | PROT_WRITE, MAP_SHARED, iFdCmd, 0))) == MAP_FAILED )
    {
        fprintf(stderr,"%s.main() :  ERREUR ---> appel a mmap() #1\n", argv[0]);
        fprintf(stderr,"             code = %d (%s)\n", errno, (char *)(strerror(errno)));
        exit( -errno );
    };
    /******************/
    /**  State zone  **/
    /******************/
    if(( iFdState = shm_open(szStateAreaName, O_RDWR, 0600)) < 0 )
    {
        if(( iFdState = shm_open(szStateAreaName, O_RDWR | O_CREAT, 0600)) < 0)
        {
            fprintf(stderr,"%s.main() :  ERREUR ---> appel a shm_open() #2\n", argv[0]);
            fprintf(stderr,"             code = %d (%s)\n", errno, (char *)(strerror(errno)));
            exit( -errno );
        }
        else
        {
            printf("CREATION de la zone %s\n", szStateAreaName);
        };
    }
    else
    {
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
    lpdb_w = &lpdb_state[OFFSET_W];
    lpdb_i = &lpdb_state[OFFSET_I];
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



    /*.......................*/
    /*  Motor current update */
    /*.......................*/
    // given to us by the teacher
    int z0 = exp(-(Te * R / L));
    int b0 = (1 / R) * (1 - z0);
    int z1 = exp(-(Te * f / J));
    int b1 = (Km / f) * (1 - z1);
    dcm.i = z0 * dcm.i - Ke * b0 * dcm.w + dcm.u;
}