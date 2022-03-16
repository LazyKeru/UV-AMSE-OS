/*============================================================================================*/
/* functions of the kinematic model of the robot to determine its linear and angular speeds */
/*--------------------------------------------------------------------------------------------*/
/*         Inspired by the example given by J.BOONAERT AMSE 2021-2022                         */
/*                           Killian ALLAIRE fise2023                                         */
/*============================================================================================*/
#include "../../header/Kinematics.h"
/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&*/
/* initialisation du robot */
/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&*/
Robot initRobot(  double  Te,     // periode d'echantillonnage
                    double  R0,     // rayon commun des roues du robot
                    double  w      // entraxe des roues
                )
{
    Robot res;
    /***********************************/
    /**    Const - Shouldn't touch    **/
    /***********************************/
    res.Te = Te;     // periode d'echantillonnage
    res.R0 = R0;     // rayon commun des roues du robot
    res.w = w;      // entraxe des roues
    /***********************************/
    /** Var - pointers **/
    /***********************************/
    //res.wR;
    //res.wL;
    //res.wG;
    return res;
}
/*&&&&&&&&&&&&&&&&&&&&&&*/
/* aide de ce programme */
/*&&&&&&&&&&&&&&&&&&&&&&*/
void usage( char *szPgmName)
{
    if( szPgmName == NULL)
    {
        exit( -1 );
    };
    printf("%s <R0> <W> <Periode d'ech.>\n", szPgmName);
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
void updateVelocity(Robot * robot)
{
    double wL;      /* ->valeur courante de la vitesse angulaire roue gauche */
    double wR;      /* ->valeur courante de la vitesse angulaire roue droite */
    double Vc;      /* ->vitesse lineaire au centre cinematique              */
    double Wc;      /* ->vitesse angulaire au centre cinematique             */
    /* photo.. */
    wL  = *(robot->wL);
    wR  = *(robot->wR);
    /* calcul */
    Vc = 0.5 * robot->R0 * (wL + wR);
    Wc = (robot->R0/robot->w) * (wR - wL);
    /*_________________________________________________________________*/
    #if defined(USR_DBG)
        if( (iCount % REFRESH_RATE) == 0 )
        {
            printf("Vc = %lf Wc = %lf \n", Vc, Wc);
        };
        iCount++;
    #endif
    /*_________________________________________________________________*/
    /* mise a jour */
    robot->wG[OFFSET_VC] = Vc;
    robot->wG[OFFSET_WC] = Wc;
}