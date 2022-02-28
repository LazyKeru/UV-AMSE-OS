/*==============================================*/
/* fonction qui permet de changer U d'un moteur */
/*==============================================*/
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include "DCmotor.h"
/*.......*/
/* const */
/*.......*/
int NB_ARGS = 10;
/*##################################*/
/* Enter double u and motor pointer */
/*##################################*/
int main(int argc, char *argv[])
{
    /*............*/
    /*  arguments */
    /*............*/
    double _u; // u
    DCmotor * dcm;
    /*.............................*/
    /*  verification des arguments */
    /*.............................*/
   /** if( argc != NB_ARGS)
    {
        usage( argv[0]);
        return( 0 );
    };
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
    dcm->u = _u;
    return 1;
}