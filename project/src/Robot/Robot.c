#include "../DCmotor/DCmotor.h";

typedef struct robot{
    // Left DC Motor
    DCmoteur * LEFT_DCMOTOR;
    // Right DC Motor
    DCmoteur * RIGHT_DCMOTOR;
    //Kinematics
    Velocity * VELOCITY;
    //Odometry
    Position * POSITION;

} Robot;

typedef struct position{
    // Abscisse
    double x;
    // Ordonné
    double y;
    // Orientation
    double q;
} Position

typedef struct velocity{
    // Vitesse linéaire
    double vL;
    // Vitesse linéaire
    double vA;
} Velocity