#ifndef SETTV_H_   /* Include guard */
#define SETTV_H_
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
#define CMD_BASENAME    "COMMAND_"
#define NB_ARGS         3               /* ->nombre d'arguments a passer en ligne de commande                            */
#define STR_LEN         64              /* ->taille des chaines par defaut                                               */
/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&*/
/* Help for starting the motor Processus */
/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&*/
void usage( char *szPgmName);
#endif // SETTV_H_