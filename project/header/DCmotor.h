#ifndef DCMOTOR_H_   /* Include guard */
#define DCMOTOR_H_


/*********************/
/** Struct_DCmotor  **/
/*********************/
typedef struct DCmotor{
    /*********************/
    /** Will not change **/
    /*********************/
    double R;   // resistance of the DC motor
    double L;   // L'inductance of the DC Motor
    double Ke;  // Constante electrique
    double Km;  // Constante moteur
    double f;   // Coef frottement
    double J;   // Inertie total ramenée sur le Rotor
    double Te;    /* Setting the DC motor sampling period */
    /*********************/
    /**   Will change   **/
    /*********************/
    double Tv; // Vitesse de la roue
    double * u; // adresse de la Commandes appliquées aux moteurs respectivemennts
    double * i; // adresse du Courant du moteur
    double * w; // adresse de la Vitesse angulaire
} DCmotor;



/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&*/
/* Help for starting the motor Processus */
/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&*/
void usage( char *szPgmName);

/*********************/
/**    initModel    **/
/*********************/
DCmotor initModel( double R,        /* ->resistance                 */
                double L,           /* ->inductance                 */
                double Ke,          /* ->constante electrique       */
                double Km,          /* ->constante moteur           */
                double f,           /* ->coefficient de frottements */
                double J,           /* ->inertie totale             */
                double Te          /* ->periode d'echantilonnage   */
                );

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&*/
/* mise a jour de l'etat du moteur */
/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&*/
void updateState(DCmotor * dcmotor);

/*&&&&&&&&&&&&&&&&&&&&&&&&*/
/* gestionnaire de signal */
/*&&&&&&&&&&&&&&&&&&&&&&&&*/
void SignalHandler( int signal);

#endif // DCMOTOR_H_