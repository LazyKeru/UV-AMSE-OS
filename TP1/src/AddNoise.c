/*============================================================*/
/* programme ajoutant un bruit uniforme a une valeur d'entree */
/*------------------------------------------------------------*/
/* JBO AMSE  2021-2022                                        */
/*============================================================*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
/*............*/
/* constantes */
/*............*/
#define NB_ARGS     2               /* ->nombre de chaines requises pour la commande */
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
    printf("%s <amplitude>\n", szPgmName);
}
/*######*/
/* MAIN */
/*######*/
int main( int argc, char *argv[])
{
    double  a;      /* ->amplitude                                                             */
    double  y;      /* ->sortie                                                                */
    double  u;      /* ->entree                                                                */
    double  t;      /* ->l'instant consideree                                                  */
    double  bruit;  /* ->bruit additionnel                                                     */
    time_t  t0;     /* ->on utilise la date pour le initialiser le generateur pseudo-aleatoire */
    int     i;      /* ->compteur de boucles                                                   */
    int     n;      /* ->borne d'iteration                                                     */
    int     GoOn=1; /* ->controle d'execution des iterations                                   */
    /*.............................*/
    /*  verification des arguments */
    /*.............................*/
    if( argc != NB_ARGS)
    {
        usage( argv[0]);
        return( 0 );
    };
    if( sscanf(argv[1], "%lf", &a ) == 0)
    {
        printf("le premier parametre doit etre un reel...\n");
        usage( argv[0]);
        return( 0 );
    };
    /*................*/
    /* initialisation */
    /*................*/
    time(&t0);
    srand((unsigned int)(t0));
    /*...........*/
    /* ierations */
    /*...........*/
    do
    {
        /* saisie de l'entree */
        fflush( stdin );
        GoOn = scanf("%lf", &u);
        if( GoOn == 1)
        {
            /* loi uniforme entre [-1, 1]  : */
            bruit = 2.0 * ((double)(rand())  -  0.5 *(double)(RAND_MAX)) / (double)(RAND_MAX);
            /* mise la l'echelle et addition a l'entree */
            y = u + a * bruit;
            printf("%lf\n", y);
            fflush( stdout );       /* ->force la sortie sur stdout */
        };
    }
    while( GoOn == 1);
    /*.....*/
    /* FIN */
    /*.....*/
    printf("STOP\n");
    fflush( stdout );   /* ->force la sortie sur stdout */
    return( 0 );
}
