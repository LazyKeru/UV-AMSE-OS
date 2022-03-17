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
#include <sys/types.h>
/*............*/
/* constantes */
/*............*/
#define VELOCITY            "VELOCITY"  /* ->nom de la zone partagee pour les vitesses lineaire et angulaire             */
#define CORDONNE            "CORDONNE"  /* ->nom de la zone partagee pour les vitesses lineaire et angulaire             */
#define NB_ARGS         5               /* ->nombre d'arguments a passer en ligne de commande                            */
#define REFRESH_RATE    5               /* ->nombre d'iterations a realiser pour 1 affichage de l'etat et de la commande */
#define OFFSET_VC       0               /* ->offset sur la zone d'etat pour acceder a la vitesse angulaire               */
#define OFFSET_WC       1               /* ->offset sur la zone d'etat pour acceder a l'intensite                        */
#define STR_LEN         64              /* ->taille des chaines par defaut               */
#define OFFSET_X        0               /* ->offset sur la zone de cordonne pour a X               */
#define OFFSET_Y        1               /* ->offset sur la zone de cordonne pour a Y                                                   */
#define OFFSET_O        2               /* ->offset sur la zone de cordonne pour son orientation O               */
/*********************/
/** Struct_Cordonne  **/
/*********************/
typedef struct Cordonne{
    /*********************/
    /** Will not change **/
    /*********************/
    double  Te;     // periode d'echantillonnage
    /********************************************/
    /**   Will Update Outside this processus   **/
    /********************************************/
    double  *lpdb_vel; /* ->pointeur sur la zone partagee des vitesses    */
    /***************************************/
    /**   Will Update in this processus   **/
    /***************************************/
    double * x;    // abscisse robot
    double * y;    // ordonnee robot
    double * O;    // orientation robot
} Cordonne;

/*&&&&&&&&&&&&*/
/* init robot */
/*&&&&&&&&&&&&*/
Cordonne initCordonne( double  Te ); // periode d'echantillonnage

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
void updateOrientation( Cordonne * cordonne );
/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&*/
/* SettingCord */
/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&*/
void setCord(
    double startingX, 
    double startingY, 
    double starting0, 
    Cordonne * cordonne
);

#endif // DCMOTOR_H_

