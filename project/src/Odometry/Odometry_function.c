/*============================================================================================*/
/*                                        Odometry tool                                       */
/*--------------------------------------------------------------------------------------------*/
/*         Inspired by the example given by J.BOONAERT AMSE 2021-2022                         */
/*                           Killian ALLAIRE fise2023                                         */
/*============================================================================================*/

#include "../../header/Odometry.h"

/*&&&&&&&&&&&&*/
/* init robot */
/*&&&&&&&&&&&&*/
Cordonne initCordonne( 
    double  Te // periode d'echantillonnage                  
){
    Cordonne res;
    res.Te = Te;
    return res;
};

/*&&&&&&&&&&&&&&&&&&&&&&*/
/* aide de ce programme */
/*&&&&&&&&&&&&&&&&&&&&&&*/
void usage( char *szPgmName){
    if( szPgmName == NULL)
    {
        exit( -1 );
    };
    printf("%s <Periode d'ech.> <Starting x> <Starting y> <Starting orientation>\n", szPgmName);
}


/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&*/
/* creation ou lien a une zone de memoire */
/* partagee.                              */
/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&*/
void *Link2SharedMem(   char *szAreaName,           /* ->nom de la zone partagee        */
                        int iSize,                  /* ->taille de la zone (en octets)  */
                        int *iFd,                   /* ->descripteur associe a la zone  */
                        int iCreate            )    /* ->cree la zone (1) si necessaire */
{
    void *vAddr;             /* ->pointeur sur la zone partagee creee / liee */
    /*.......*/
    /* check */
    /*.......*/
    if( szAreaName == NULL )
    {
        fprintf(stderr,"Link2SharedMem() : ERREUR ---> pointeur NULL passe en argument #1\n");
        return( NULL );
    };
    if( iFd == NULL )
    {
        fprintf(stderr,"Link2SharedMem() : ERREUR ---> pointeur NULL passe en argument #3\n");
        return( NULL );
    };
    /*..................................................*/
    /* lien a / creation de la zone de memoire partagee */
    /*..................................................*/
    if(( *iFd = shm_open(szAreaName, O_RDWR, 0600)) < 0 )
    {
        if( iCreate > 0 )
        {
            if(( *iFd = shm_open(szAreaName, O_RDWR | O_CREAT, 0600)) < 0)
            {
                fprintf(stderr,"Link2SharedMem() :  ERREUR ---> appel a shm_open() pour creation \n");
                fprintf(stderr,"                    code = %d (%s)\n", errno, (char *)(strerror(errno)));
                return( NULL );
            };
        }
        else
        {
            fprintf(stderr,"Link2SharedMem() :  ERREUR ---> appel a shm_open() pour lien \n");
            fprintf(stderr,"                    code = %d (%s)\n", errno, (char *)(strerror(errno)));
            return( NULL );
        };
    };
    /* ajustement de la taille (en octets) */
    if( ftruncate(*iFd, iSize) < 0 )
    {
        fprintf(stderr,"Link2SharedMem() :  ERREUR ---> appel a ftruncate() \n");
        fprintf(stderr,"                    code = %d (%s)\n", errno, (char *)(strerror(errno)));
        return( NULL );
    };
    /* mapping dans la memoire du processus */
     if((vAddr = mmap(NULL, iSize, PROT_READ | PROT_WRITE, MAP_SHARED, *iFd, 0)) == MAP_FAILED )
    {
        fprintf(stderr,"Link2SharedMem() :  ERREUR ---> appel a mmap() #1\n");
        fprintf(stderr,"                    code = %d (%s)\n", errno, (char *)(strerror(errno)));
        return( NULL );
    };
    /* fini */
    return( vAddr );
}

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&*/
/* mise a jour des vitesses au centre cinematique */
/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&*/
void updateOrientation( Cordonne * cordonne ){
    /* Old values */
    double _x = (double) *(cordonne->x);
    double _y;
    double _O;
    double _Te = (double) cordonne->Te;
    double _vc = (double) (cordonne->lpdb_vel[OFFSET_VC]);
    double _wc = (double) (cordonne->lpdb_vel[OFFSET_WC]);
    /* Updating values */
    *(cordonne->x) = _x - _vc * _Te * sin(_O);
    *(cordonne->y) = _y + _vc * _Te * cos(_O);
    *(cordonne->O) = _O * _Te * _wc;
    printf("The new cords x : %f ; y : %f ; O : %f ; \n", *(cordonne->x), *(cordonne->y), *(cordonne->O));
}

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&*/
/* SettingCord */
/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&*/
void setCord(
    double startingX, 
    double startingY, 
    double starting0, 
    Cordonne * cordonne
){
    *cordonne->x = startingX;
    *cordonne->y = startingY;
    *cordonne->O = starting0;
}