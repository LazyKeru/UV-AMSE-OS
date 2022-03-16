/*============================================================================================*/
/*                   function of regulation PID d'un moteur CC                                */
/*--------------------------------------------------------------------------------------------*/
/*         Inspired by the example given by J.BOONAERT AMSE 2021-2022                         */
/*                           Killian ALLAIRE fise2023                                         */
/*============================================================================================*/

#include "../../header/RegPID.h"

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&*/
/* initialisation du PID */
/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&*/
PID initPID(
    double  Te,                             /* ->periode d'echantillonnage                    */
    double  Kcoeff,                         /* ->action proportionnelle                       */
    double  Icoeff,                         /* ->action integrale                             */
    double  Dcoeff                          /* ->action derivee                               */
    )
{
    PID res;
    /***********************************/
    /**    Const - Shouldn't touch    **/
    /***********************************/
    res.Te = Te;                /* ->periode d'echantillonnage                    */
    res.Kcoeff = Kcoeff;        /* ->action proportionnelle                       */
    res.Icoeff = Icoeff;        /* ->action integrale                             */
    res.Icoeff = Dcoeff;        /* ->action derivee                               */
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
    printf("%s <P> <I> <D> <Periode d'ech.> <drive>\n", szPgmName);
    printf("   avec <drive> = L | R \n");
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
/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&*/
/* mise a jour de la commande du moteur */
/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&*/
void updateCommand(PID * pid)
{
    double u;       /* ->valeur courante de la commande          */
    /* init */
    pid->e = *pid->lpdb_Tv - *pid->lpdb_w;
    pid->De = (pid->e - pid->e_prev)/pid->Te;
    pid->Ie = pid->Ie_prev + pid->Te * pid->e;
    /* calcul */
    u = pid->Kcoeff * ( pid->e + pid->Icoeff * pid->Ie + pid->Dcoeff * pid->De);
    /*_________________________________________________________________*/
    #if defined(USR_DBG)
        if( (iCount % REFRESH_RATE) == 0 )
        {
            printf("Te = %lf e = %lf De = %lf Ie = %lf\n", Te, e, De, Ie);
            printf("u PID = %lf\n", u);
        };
        iCount++;
    #endif
    /*_________________________________________________________________*/
    /* mise a jour */
    pid->Ie_prev = pid->Ie;
    pid->e_prev  = pid->e;
    *pid->lpdb_u = u;
}