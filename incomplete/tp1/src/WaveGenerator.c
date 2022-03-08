/*=============================================================*/
/* Programme generant une sinusoide et affichant le valeur     */
/* du signal au format texte.                                  */
/*-------------------------------------------------------------*/
/* JBO AMSE  2021-2022                                         */
/*=============================================================*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include <string.h>
#include <ctype.h>
/*............*/
/* constantes */
/*............*/
#define NB_ARGS     5               /* ->nombre de chaines requises pour la commande */
#define MY_PI       3.141592654
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
    printf("%s <amplitude> <frequence (Hz)> <duree (s)> <periode d'echantillonnage>\n", szPgmName);
}
/*######*/
/* MAIN */
/*######*/
int main( int argc, char *argv[])
{
    double  f;      /* ->frequence du signal                 */
    double  a;      /* ->amplitude                           */
    double  y;      /* ->sortie                              */
    double  t;      /* ->l'instant consideree                */
    double  duree;  /* ->duree (~) du signal a generer       */
    double  Te;     /* ->periode d'echantillonnage (s)       */
    int     i;      /* ->compteur de boucles                 */
    int     n;      /* ->borne d'iteration                   */
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
    if( sscanf(argv[2], "%lf", &f ) == 0)
    {
        printf("le second parametre doit etre un reel...\n");
        usage( argv[0]);
        return( 0 );
    };
    if( sscanf(argv[3], "%lf", &duree ) == 0)
    {
        printf("le troisieme parametre doit etre un reel...\n");
        usage( argv[0]);
        return( 0 );
    };
    if( sscanf(argv[4], "%lf", &Te ) == 0)
    {
        printf("le quatrieme parametre doit etre un reel...\n");
        usage( argv[0]);
        return( 0 );
    };
    /*................*/
    /* initialisation */
    /*................*/
    n = (int)(fabs(duree) / Te);
    /*...........*/
    /* ierations */
    /*...........*/
    for(t = 0.0, i=0; i < n; i++, t+= Te)
    {
        y = a * sin( 2.0 * MY_PI * f * t  );
        printf("%lf\n", y);
        fflush( stdout );       /* ->force la sortie sur stdout */
    }
    /*.....*/
    /* FIN */
    /*.....*/
    printf("STOP\n");
    fflush( stdout );   /* ->force la sortie sur stdout */
    return( 0 );
}
