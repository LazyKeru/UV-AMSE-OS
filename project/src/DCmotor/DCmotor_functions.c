/*=============================================================================*/
/* Functions for simulating the output of a DCmotor depending on the intensity */
/*-----------------------------------------------------------------------------*/
/*         Inspired by the example given by J.BOONAERT AMSE 2021-2022          */
/*                           Killian ALLAIRE fise2023                          */
/*=============================================================================*/

#include "../../header/DCmotor.h"

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&*/
/* Help for starting the motor Processus */
/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&*/
void usage( char *szPgmName)
{
    if( szPgmName == NULL)
    {
        exit( -1 );
    };
    printf("%s <Resistance> <Inductance> <Cste electrique> <Cste moteur> <Coeff de frottement> <Inertie totale> <Periode d'ech.> <drive>\n", szPgmName);
    printf("   with <drive> = L | R \n");
}

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&*/
/* initialisation du model */
/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&*/
DCmotor initModel( double R,        /* ->resistance                 */
                double L,           /* ->inductance                 */
                double Ke,          /* ->constante electrique       */
                double Km,          /* ->constante moteur           */
                double f,           /* ->coefficient de frottements */
                double J,           /* ->inertie totale             */
                double Te          /* ->periode d'echantilonnage   */
                )
{
    DCmotor res;
    /***********************************/
    /**    Const - Shouldn't touch    **/
    /***********************************/
    res.R = R;      /* Setting the DC motor resistance */ 
    res.L = L;      /* Setting the DC motor inductance */
    res.Ke = Ke;    /* Setting the electric constant */
    res.Km = Km;    /* Setting the DC motor constant */
    res.f = f;      /* Setting the DC motor coefficient of friction */
    res.J = J;      /* Setting the DC motor total inertia brought back on the Rotor */
    res.Te = Te;    /* Setting the DC motor sampling period */
    /***********************************/
    /** Var - Will have a get and set **/
    /***********************************/
    //double * i, * u, * w;
    //res.i = i;
    //res.Tv;
    //res.u = u;
    //res.w = w;
    return res;
}

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&*/
/* mise a jour de l'etat du moteur */
/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&*/
void updateState(DCmotor * dcmotor)
{
    /* Taking the old value */
    double u = (double) *(dcmotor->u);
    double i = (double) *(dcmotor->i);
    double w = (double) *(dcmotor->w);
    /*....................................................*/
    /*  Motor current update - cst, should move elsewhere */
    /*....................................................*/
    // given to us by the teacher
    double z0 = exp(-(dcmotor->Te * dcmotor->R / dcmotor->L));  /* ->coeff. pour la recurrence */
    double b0 = (1 / dcmotor->R) * (1 - z0);                    /* ->coeff. pour la recurrence */
    double z1 = exp(-(dcmotor->Te * dcmotor->f / dcmotor->J));  /* ->coeff. pour la recurrence */
    double b1 = (dcmotor->Km / dcmotor->f) * (1 - z1);          /* ->coeff. pour la recurrence */
    /* updating their value */
    *(dcmotor->i) = (double) (z0 * i + -dcmotor->Ke*b0 * w + b1 * u);
    *(dcmotor->w) = (double) (b1 * i + z1 * w);
    printf("The new i of dcmotor : %f ; The new w of dcmotor : %f ;\n", *(dcmotor->i), *(dcmotor->w));
}