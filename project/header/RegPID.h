#ifndef RegPID_H_   /* Include guard */
#define RegPID_H_
/*............*/
/* constantes */
/*............*/
#define CMD_BASENAME        "COMMAND_"
#define STATE_BASENAME      "STATE_"
#define TARGET_BASENAME     "TARGET_"
#define NB_ARGS         6               /* ->nombre d'arguments a passer en ligne de commande                            */
#define REFRESH_RATE    5               /* ->nombre d'iterations a realiser pour 1 affichage de l'etat et de la commande */
#define OFFSET_W        0               /* ->offset sur la zone d'etat pour acceder a la vitesse angulaire               */
#define OFFSET_I        1               /* ->offset sur la zone d'etat pour acceder a l'intensite                        */
#define STR_LEN         64              /* ->taille des chaines par defaut                                               */

typedef struct PID{
    /*********************/
    /** Will not change **/
    /*********************/
    double  Te;                 /* ->periode d'echantillonnage                    */
    double  Kcoeff;             /* ->action proportionnelle                       */
    double  Icoeff;             /* ->action integrale                             */
    double  Dcoeff;             /* ->action derivee                               */
    /*********************/
    /**   Will change   **/
    /*********************/
    double  *lpdb_u;            /* ->pointeur sur la commande                     */
    double  *lpdb_w;            /* ->pointeur sur la vitesse angulaire            */
    double  *lpdb_Tv;           /* ->pointeur sur la consigne de vitesse          */
    double  *lpdb_i;            /* ->pointeur sur le courant                      */
    int iCount;                 /* ->comptage du nombre d'iterations              */
    double  e;                  /* ->erreur courante                              */
    double  e_prev;             /* ->erreur passee                                */
    double  De;                 /* ->derivee de l'erreur                          */
    double  Ie;                 /* ->integrale de l'erreur                        */
    double  Ie_prev;            /* ->valeur precedente de l'integrale de l'erreur */
} PID;

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&*/
/* initialisation du PID */
/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&*/
PID initPID(
    double  Te,                             /* ->periode d'echantillonnage                    */
    double  Kcoeff,                         /* ->action proportionnelle                       */
    double  Icoeff,                         /* ->action integrale                             */
    double  Dcoeff                          /* ->action derivee                               */
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

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&*/
/* mise a jour de la commande du moteur */
/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&*/
void updateCommand(PID * pid);

#endif // RegPID_H_

