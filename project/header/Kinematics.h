#ifndef KINEMATICS_H_   /* Include guard */
#define KINEMATICS_H_

/*.........*/
/* Library */
/*.........*/
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
/*............*/
/* constantes */
/*............*/
#define OFFSET_W        0               /* ->indice pour acceder a la vitesse angulaire d'un rotor                       */
#define STATE_L             "STATE_L"   /* ->etat du moteur gauche                                                       */
#define STATE_R             "STATE_R"   /* ->etat du moteur droit                                                        */
#define VELOCITY            "VELOCITY"  /* ->nom de la zone partagee pour les vitesses lineaire et angulaire             */
#define NB_ARGS         4               /* ->nombre d'arguments a passer en ligne de commande                            */
#define REFRESH_RATE    5               /* ->nombre d'iterations a realiser pour 1 affichage de l'etat et de la commande */
#define OFFSET_VC       0               /* ->offset sur la zone d'etat pour acceder a la vitesse angulaire               */
#define OFFSET_WC       1               /* ->offset sur la zone d'etat pour acceder a l'intensite                        */
#define STR_LEN         64              /* ->taille des chaines par defaut                                               */

/*********************/
/** Struct_Robot  **/
/*********************/
typedef struct Robot{
    /*********************/
    /** Will not change **/
    /*********************/
    double  Te;     // periode d'echantillonnage
    double  R0;     // rayon commun des roues du robot
    double  w;      // entraxe des roues
    /*********************/
    /**   Will change   **/
    /*********************/
    double * wR;    // pointer to the speed of the right motor
    double * wL;    // pointer to the speed of the left motor
    double * wG;    // point to the global speed of robot
    int iCount;     // number of iteration
} Robot;

/*&&&&&&&&&&&&*/
/* init robot */
/*&&&&&&&&&&&&*/
Robot initRobot(  double  Te,     // periode d'echantillonnage
                  double  R0,     // rayon commun des roues du robot
                  double  w      // entraxe des roues
                );

/*&&&&&&&&&&&&&&&&&&&&&&*/
/* aide de ce programme */
/*&&&&&&&&&&&&&&&&&&&&&&*/
void usage( char *szPgmName);

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&*/
/* creation ou lien a une zone de memoire */
/* partagee.                              */
/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&*/
void *Link2SharedMem(   char *szAreaName,           /* ->nom de la zone partagee        */
                        int iSize,                  /* ->taille de la zone (en octets)  */
                        int *iFd,                   /* ->descripteur associe a la zone  */
                        int iCreate            );   /* ->cree la zone (1) si necessaire */

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&*/
/* mise a jour des vitesses au centre cinematique */
/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&*/
void updateVelocity( Robot * robot );

#endif // DCMOTOR_H_

