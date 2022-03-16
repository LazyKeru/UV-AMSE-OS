# src

## Image overview

![Project Overview](../doc/img/overview.png?raw=true)

## Entities:

### 2 DC Motor

### Processus

DCMotor takes output u and sets two states: w and i.
with:
- INPUT:
    - u: tension de commande
- OUTPUT:
    - w: valeur de la vitesse angulaire
    - i: valeur du courant du moteur

When first starting the Processus we need to give the following parameters:
- double R;   // resistance of the DC motor
- double L;   // L'inductance of the DC Motor
- double Ke;  // Constante electrique
- double Km;  // Constante moteur
- double f;   // Coef frottement
- double J;   // Inertie total ramenée sur le Rotor
- int Te; // période d'échantillonage of the simDCmoteur, in seconds



## Compile FiltrePremierOrdre:

```
gcc ./src/DCmotor.c -o ./bin/DCmoto.h
```