/*=============================================================*/
/* filtre de base pour l'illustration du concept de "pipeline" */
/* Version complete                                            */
/*-------------------------------------------------------------*/
/* JBO AMSE  2021-2022                                         */
/*=============================================================*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
/*............*/
/* constantes */
/*............*/
#define NB_ARGS     4               /* ->nombre de chaines requises pour la commande */
/*..............*/
/* declarations */
/*..............*/
void usage( char *);                /* ->affiche l'aide relative au programme        */
/*.............*/
/* definitions */
/*.............*/
/*&&&&&&&&&&&&&&&&&&&&&&*/
/* aide de ce programme */
/*&&&&&&&&&&&&&&&&&&&&&&*/
void usage( char *szPgmName)
{
    if( szPgmName == NULL)
    {
        exit( -1 );
    };
    printf("%s <gain> <cste de temps> <periode d'echantillonnage>\n", szPgmName);
}
/*######*/
/* MAIN */
/*######*/
int main( int argc, char *argv[])
{
    double  u;      /* ->entree courante                     */
    double  y;      /* ->nouvelle sortie                     */
    double  y1;     /* ->sortie precedente                   */
    double  k;      /* ->gain statique                       */
    double  tau;    /* ->constante de temps                  */
    double  Te;     /* ->periode d'echantillonnage (s)       */
    double  a0,b0;  /* ->coefficients recurrence             */
    int     GoOn;   /* ->controle d'execution des iterations */
    /*.............................*/
    /*  verification des arguments */
    /*.............................*/
    if( argc != NB_ARGS)
    {
        usage( argv[0]);
        return( 0 );
    };
    if( sscanf(argv[1], "%lf", &k ) == 0)
    {
        printf("le premier parametre doit etre un reel...\n");
        usage( argv[0]);
        return( 0 );
    };
    if( sscanf(argv[2], "%lf", &tau ) == 0)
    {
        printf("le second parametre doit etre un reel...\n");
        usage( argv[0]);
        return( 0 );
    };
    if( sscanf(argv[3], "%lf", &Te ) == 0)
    {
        printf("le troisieme parametre doit etre un reel...\n");
        usage( argv[0]);
        return( 0 );
    };
    /*..........................................*/
    /* calcul des coefficients de le recurrence */
    /*..........................................*/
    if( tau != 0.0)
    {
        a0 = exp(-Te/tau);
    }
    else
    {
        a0 = 0.0;
    };
    b0 = k*(1.0 - a0);
    /*................*/
    /* initialisation */
    /*................*/
    y1 = 0.0;
    GoOn = 1;
    /*...........*/
    /* ierations */
    /*...........*/
    do
    {
        fflush( stdin );
        GoOn = scanf("%lf", &u);
        if( GoOn == 1)
        {
            y = a0 * y1 + b0 * u;
            y1 = y;
            printf("%lf\n", y);
            fflush( stdout );       /* ->force la sortie sur stdout */
        };
    }
    while (GoOn == 1);
    /*.....*/
    /* FIN */
    /*.....*/
    printf("STOP\n");
    fflush( stdout );   /* ->force la sortie sur stdout */
    return( 0 );
}
