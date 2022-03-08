/*=====================================================*/
/* programme de test de la notion de signal sous Linux */
/*-----------------------------------------------------*/
/* Jacques BOONAERT - cours AMSE */
/*=====================================================*/
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <signal.h> /* ->GESTION DES SIGNAUX */
extern char *strsignal( int );
/*............*/
/* prototypes */
/*............*/
void signal_handler( int ); /* ->routine de gestion du signal recu */
/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&*/
/* routine de gestion du signal recu */
/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&*/
void signal_handler( int signal ) /* ->code du signal recu */
{
 printf("signal recu = %d\n", signal );
 /* strsignal retourne la chaine de caracteres */
 /* qui correspond au "nom symbolique" du signal */
 printf("%s\n", (char *)(strsignal( signal )) );
}
/*#####################*/
/* programme principal */
/*#####################*/
int main( int argc, char *argv[] )
{
 struct sigaction sa; /* ->structure permettant de definir le gestionnaire */
 /* et d'y associer le signal a traiter, etc. */
 sigset_t blocked; /* ->contiendra la liste des signaux qui seront masques */
 /* on ne bloque aucun signal : blocked = vide */
 sigemptyset( &blocked );
 /* mise a jour de la structure sigaction */
 memset( &sa, 0, sizeof(struct sigaction));
 sa.sa_handler = signal_handler; /* ->precise le gestionnaire a utiliser */
 sa.sa_flags = 0; /* ->fonctionnement "normal" */
 sa.sa_mask = blocked; /* ->indique les signaux masques */
 /* installation EFFECTIVE du gestionnaire */
 sigaction( SIGUSR1, &sa, NULL ); /* ->associe signal_handler a la reception de SIGUSR1 */
 sigaction( SIGUSR2, &sa, NULL ); /* ->idem pour SIGUSR2 */
 /* attente de reception de signal */
 do
 {
 printf("attente de signal....\n");
 pause(); /* ->on ne fait rien, hormis attendre un eventuel signal */
 }
 while( 1 ); /* !!! boucle infinie */
 return( 0 );
}
