#ifndef DCMOTOR_H_   /* Include guard */
#define DCMOTOR_H_


typedef struct DCmotor{
    // Vitesse de la roue
    double Tv;
    // Commandes appliquées aux moteurs respectivemennts
    double u;
    // Courant du moteur
    double i;
    // Vitesse angulaire
    double w;
} DCmotor;


#endif // DCMOTOR_H_