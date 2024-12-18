#ifndef PSO_H
#define PSO_H

#define STATE_NUM 4                     // four states (a, b, c, d)

typedef struct Particle                 // control local variables for each particle
{
    double positions[STATE_NUM];        // pa, pb, pc, pd
    double velocities[STATE_NUM];       // va, vb, vc, vd
    double bestError;                   // local best error
    double bestParameters[STATE_NUM];   // the corresponding set of paramters
} Particle;

// Helper functions
double forwardEulerX(double a, double b, double x, double y);
double forwardEulerY(double c, double d, double x, double y);
void generateTimeSeries(double xSeries[], double ySeries[], double parameters[]);
double calculateError(double positions[]);
double calculateVelocity(double p, double v, double localBest, double globalBest, double randNum);
double calculatePosition(double p, double v);

// Main functions
void generateTrueSeries(double trueValues[]);
void initializeParticle(Particle *particle, double lowerBounds[], double upperBounds[], unsigned int seed);
void updateParticle(Particle *particle, Particle bestGlobalParticle, double lowerBounds[], double upperBounds[], unsigned int seed);
Particle findBestParticle(Particle particles[], int size);

#endif