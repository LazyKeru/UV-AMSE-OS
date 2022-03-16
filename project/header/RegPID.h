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
                                               */

#endif // RegPID_H_

