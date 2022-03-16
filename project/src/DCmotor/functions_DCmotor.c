/*=============================================================================*/
/* Functions for simulating the output of a DCmotor depending on the intensity */
/*-----------------------------------------------------------------------------*/
/*         Inspired by the example given by J.BOONAERT AMSE 2021-2022          */
/*                           Killian ALLAIRE fise2023                          */
/*=============================================================================*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include "DCmotor.h"

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

/*&&&&&&&&&&&&&&&&&&&&&&*/
/* aide de ce programme */
/*&&&&&&&&&&&&&&&&&&&&&&*/
DCmotor initModel( double R,           /* ->resistance                 */
                double L,           /* ->inductance                 */
                double Ke,          /* ->constante electrique       */
                double Km,          /* ->constante moteur           */
                double f,           /* ->coefficient de frottements */
                double J,           /* ->inertie totale             */
                double Te   )       /* ->periode d'echantilonnage   */
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
    res.i;
    res.Tv;
    res.u;
    res.w;
    return res;
}