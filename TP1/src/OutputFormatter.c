/*================================================================*/
/* met en forme le "signal" en presentant ajoutant une estampille */
/* horaire, en nommant les colonnes et en remplace le '.' par ',' */
/* pour l'importation sous EXCEL                                  */
/*----------------------------------------------------------------*/
/* JBO AMSE  2021-2022                                            */
/*================================================================*/
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
#define NB_ARGS     2               /* ->nombre de chaines requises pour la commande        */
#define COLONNE1    "temps"         /* ->nom donne a la premiere colonne                    */
#define COLONNE2    "sortie"        /* ->nom donne a le seconde colonne                     */
#define STR_LEN     64              /* ->taile par defaut des chaines de caracteres         */
/*..............*/
/* declarations */
/*..............*/
void usage( char *);                /* ->affiche l'aide relative au programme               */
int remplace(char *, char, char );  /* ->remplace ub caractere par un autre dans une chaine */
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
    printf("%s <periode d'echantillonnage>\n", szPgmName);
}
/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&*/
/* remplacement d'un caractere par un autre dans */
/* une chaine de caracteres                      */
/* OUT : le nombre d'ocurrences remplacees       */
/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&*/
int remplace(   char *szInStr,  /* ->chaine en entree          */
                char cFind,     /* ->caractere a remplacer     */
                char cReplace ) /* ->caractere de remplacement */
{   register int i;             /* ->compteur de boucle        */
    register int n;             /* ->borne d'iterations        */
    register int iMatch = 0;    /* ->nombre de remplacements   */
    /* verification des arguments */
    if( szInStr == NULL)
    {
        fprintf(stderr,"ERREUR : remplace() ---> pointeur NULL passe en argument #1\n");
        return( -1 );
    };
    /* initialisation */
    n = strlen( szInStr);
    /* analyse de la chaine */
    for( i=0; i < n; i++)
    {
        if( szInStr[i] == cFind)
        {
            szInStr[i] = cReplace;
            iMatch++;
        };
    };
    return( iMatch);
}
/*######*/
/* MAIN */
/*######*/
int main( int argc, char *argv[])
{
    double  u;                      /* ->entree                                        */
    double  Te;                     /* ->periode d'echantillonnage                     */
    double  t;                      /* ->l'instant consideree                          */
    int     GoOn=1;                 /* ->controle d'execution des iterations           */
    char    szOutStr1[STR_LEN];     /* ->pour la mise en forme de la sortie            */
    char    szOutStr2[STR_LEN];     /* ->pour la mise en forme de la sortie            */
    /*.............................*/
    /*  verification des arguments */
    /*.............................*/
    if( argc != NB_ARGS)
    {
        usage( argv[0]);
        return( 0 );
    };
    if( sscanf(argv[1], "%lf", &Te ) == 0)
    {
        printf("le premier parametre doit etre un reel...\n");
        usage( argv[0]);
        return( 0 );
    };
    /*................*/
    /* initialisation */
    /*................*/
    t = 0.0;
    printf("%s\t%s\n", COLONNE1, COLONNE2);
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
            /* mise en forme de la sortie */
            memset(szOutStr1,0,STR_LEN);
            memset(szOutStr2,0,STR_LEN);
            sprintf(szOutStr1,"%lf", t);
            remplace(szOutStr1,'.',',');
            sprintf(szOutStr2,"%lf", u);
            remplace(szOutStr2,'.',',');
            /* affichage */
            printf("%s\t%s\n", szOutStr1, szOutStr2);
            fflush( stdout );
            t+= Te;
        };
    }
    while( GoOn == 1);
    /*.....*/
    /* FIN */
    /*.....*/
    fflush( stdout );   /* ->force la sortie sur stdout */
    return( 0 );
}
