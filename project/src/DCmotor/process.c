#include <> /* -> function simulate running motor */
#include <> /* -> struct motor state */
/*######*/
/* main */
/*######*/
int main(){
    struct sigaction sa; /* ->structure permettant de definir le gestionnaire */
    /* et d'y associer le signal a traiter, etc. */
    sigset_t blocked; /* ->contiendra la liste des signaux qui seront masques */
    /* on ne bloque aucun signal : blocked = vide */
    

}