#include
#include
#include
/*............*/
/* constantes */
/*............*/
#define AREA_NAME "PHRASE"
#define STR_LEN 256
/*......*/
/* main */
/*......*/
int main ( void ){
    char *szInString;
    int iShmFd;
    /**/
    /* Trying to create the zone */
    /**/
    if(shm_open(AREA_NAME, O_RDWR | O_CREAT, STR_LEN)){
        fprintf(stderr, "ERREUR: ------> calling shm_open()\n");
        fprintf(stderr, "ERREUR: ------> calling shm_open()\n")
    }

}

/*...................*/
/* NOT COMPLETE !!!! */
/*...................*/