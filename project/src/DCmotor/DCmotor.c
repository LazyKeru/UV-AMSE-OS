/*=======================================================================*/
/* fonction qui simule la sorite d'un DCmotor en fonction de l'intensité */
/*=======================================================================*/
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include "DCmotor.h"
/*.......*/
/* const */
/*.......*/
int NB_ARGS = 10;
/*#####################################*/
/* Enter double u, Returns int i and w */
/*#####################################*/
int main(int argc, char *argv[])
{
    /*............*/
    /*  arguments */
    /*............*/
    // Physical parameters
    double R;   // resistance of the DC motor
    double L;   // L'inductance of the DC Motor
    double Ke;  // Constante electrique
    double Km;  // Constante moteur
    double f;   // Coef frottement
    double J;   // Inertie total ramenée sur le Rotor
    int Te; // période d'échantillonage of the simDCmoteur, in seconds
    DCmotor * dcm;
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
    
    /**
    if( sscanf(argv[1], "%lf", &a ) == 0)
    {
        printf("le premier parametre doit etre un reel...\n");
        usage( argv[0]);
        return( 0 );
    };**/
    /*.......................*/
    /*  Motor current update */
    /*.......................*/
    // given to us by the teacher
    int z0 = exp(-(Te * R / L));
    int b0 = (1 / R) * (1 - z0);
    int z1 = exp(-(Te * f / J));
    int b1 = (Km / f) * (1 - z1);
    dcm->i = z0 * dcm->i - Ke * b0 * dcm->w + dcm->u;
}