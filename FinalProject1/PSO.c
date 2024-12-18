#include "PSO.h"
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define TIME_STEPS 100              // 100 time steps in a time series
#define DELTA_T 0.25                // delta time 0.25
#define X_0 2.5                     // initial prey number (x)
#define Y_0 3                       // initial predator number (y)
#define PHI_LOCAL 2.05              // local phi
#define PHI_GLOBAL 2.05             // global phi
#define CHI 0.73                    // chi
#define GAMMA 0.05                  // gamma

// Global variables of true prey (x) and predator(y) time series for PSO.c
static double trueX[TIME_STEPS + 1], trueY[TIME_STEPS + 1];

/*********************************************Helper funtions******************************************/
// Calculate the prey's time series via the given parameters, prey and predator
double forwardEulerX(double a, double b, double x, double y)
{
    return x + DELTA_T * (a * x - b * x * y);  
}

// Calculate the predator's time series via the given parameters, prey and predator
double forwardEulerY(double c, double d, double x, double y)
{
    return y + DELTA_T * (c * x * y - d * y);
}

// Generate the prey's and predator's time series via the given the parameters
void generateTimeSeries(double xSeries[], double ySeries[], double parameters[])
{
    xSeries[0] = X_0;
    ySeries[0] = Y_0;
    for (int i = 0; i < TIME_STEPS; i++)
    {
        xSeries[i + 1] = forwardEulerX(parameters[0], parameters[1], xSeries[i], ySeries[i]);
        ySeries[i + 1] = forwardEulerY(parameters[2], parameters[3], xSeries[i], ySeries[i]);
    }
}

// Calculate error via the given particle's position
// Generate actual time series and then sum up the differences between true and actual series 
double calculateError(double positions[])
{
    double xSeries[TIME_STEPS + 1], ySeries[TIME_STEPS + 1];    // prey's and predator's time series
    generateTimeSeries(xSeries, ySeries, positions);            // generate by the given positions

    double sum = 0;
    for (int i = 1; i < TIME_STEPS; i++)
        sum += fabs(trueX[i] - xSeries[i]) + fabs(trueY[i] - ySeries[i]);
    return sum;
}

// Calculate a new velocity from old position, velocity, local and global best parameters (position)
double calculateVelocity(double p, double v, double localBest, double globalBest, double randNum)
{
    return CHI * (v + PHI_LOCAL * (randNum * (localBest - p)) + PHI_GLOBAL * (randNum * (globalBest - p)));
}

// Calculate a new position from old position and velocity
double calculatePosition(double p, double v)
{
    return p + GAMMA * v;
}

/************************************************Main functions*********************************************/
// Generate true time series given the true parameters
void generateTrueSeries(double trueValues[])
{
    generateTimeSeries(trueX, trueY, trueValues);
}

// Initialize a single particle
void initializeParticle(Particle *particle, double lowerBounds[], double upperBounds[], unsigned int seed)
{
    // Initialize position and velocity
    for (int i = 0; i < STATE_NUM; i++)
    {
        double lowerBound = lowerBounds[i];
        double upperBound = upperBounds[i];
        particle->positions[i] = lowerBound + (upperBound - lowerBound) * ((double)rand() / RAND_MAX);
        particle->velocities[i] = (double)rand() / RAND_MAX;
    }

    // Local best error and corresponding set of parameters
    particle->bestError = calculateError(particle->positions);
    memcpy(particle->bestParameters, particle->positions, STATE_NUM * sizeof(double));
}

// Update a single particle
void updateParticle(Particle *particle, Particle bestGlobalParticle, double lowerBounds[], double upperBounds[], unsigned int seed)
{
    for (int i = 0; i < STATE_NUM; i++)
    {
        // Update velocity firstly
        particle->velocities[i] = calculateVelocity(particle->positions[i], particle->velocities[i], 
            particle->bestParameters[i], bestGlobalParticle.bestParameters[i], (double)rand() / RAND_MAX);

        // Update position later
        double position = calculatePosition(particle->positions[i], particle->velocities[i]);

        // Enfore the positions to locate in the given bounds
        double lowerBound = lowerBounds[i];
        double upperBound = upperBounds[i];
        particle->positions[i] = position >= lowerBound && position <= upperBound ? position : 
            lowerBound + (upperBound - lowerBound) * ((double)rand() / RAND_MAX);
    }

    // Calculate current error from current position
    double error = calculateError(particle->positions);

    // Update best error and corresponding parameters
    if (error < particle->bestError)
    {
        particle->bestError = error;
        memcpy(particle->bestParameters, particle->positions, STATE_NUM * sizeof(double));
    }
}

// Find and return the best particle which has the lowest error in the given particles 
Particle findBestParticle(Particle particles[], int size)
{
    int minIndex = 0;
    double minError = particles[0].bestError; 
    for (int i = 0; i < size; i++)
    {
        if (minError <= particles[i].bestError) continue;
        minError = particles[i].bestError;
        minIndex = i;
    }

    return particles[minIndex];
}
