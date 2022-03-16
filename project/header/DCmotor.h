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
    double u; // Commandes appliquées aux moteurs respectivemennts
    double i; // Courant du moteur
    double w; // Vitesse angulaire
} DCmotor;

/*********************/
/**    initModel    **/
/*********************/
DCmotor initModel( double R,           /* ->resistance                 */
                double L,           /* ->inductance                 */
                double Ke,          /* ->constante electrique       */
                double Km,          /* ->constante moteur           */
                double f,           /* ->coefficient de frottements */
                double J,           /* ->inertie totale             */
                double Te   );       /* ->periode d'echantilonnage   */


#endif // DCMOTOR_H_