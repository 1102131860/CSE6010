#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include "PSO.h"

// PSO specifications
#define ITERATION_NUM 50
#define PARTICLES_NUM 10000

// Global varibale for main.c
static double trueValues[STATE_NUM], lowerBounds[STATE_NUM], upperBounds[STATE_NUM];
static Particle particles[PARTICLES_NUM], localBestParticles[ITERATION_NUM + 1];

int main (int argc, char* argv[])
{
    // Must be exactly 3 or 4 arguments
    if (argc != 4 && argc != 5) 
    {
        printf("The invalid number of arguments\n");
        return 1;
    }
    
    // Read command-line arguments
    char* fileAddress = argv[1];
    unsigned int thread = atoi(argv[2]);
    unsigned int baseSeed = atoi(argv[3]);
    char printChar = ' ';
    if (argc == 5)
        printChar = argv[4][0];

    // Read file from file address
    FILE* file = fopen(fileAddress, "r");
    if (!file) 
    {
        printf("Invalid input file address: %s\n", fileAddress);
        return 1;
    }

    // Read requirements from the given file
    for (int i = 0; i < STATE_NUM; i++)
    {
        if (fscanf(file, "%lf %lf %lf", &trueValues[i], &lowerBounds[i], &upperBounds[i]) != 3) {
            printf("Invalid number of parameters in the file at line %d: %s\n", i + 1, fileAddress);
            break;
        }
    }

    fclose(file);

    // Iniailization
    // Generate True time series from true values a, b, c, d
    generateTrueSeries(trueValues);

    // Iterator
    int i, j;
    // Set up the thread numbers
    omp_set_num_threads(thread);

    double startTime = omp_get_wtime();
    // Initialize the particles
    #pragma omp parallel for
    for (i = 0; i < PARTICLES_NUM; i++) 
    {
        int threadNum = omp_get_thread_num();
        initializeParticle(&particles[i], lowerBounds, upperBounds, threadNum + baseSeed);
    }

    // Store the global best particle after initialization
    localBestParticles[0] = findBestParticle(particles, PARTICLES_NUM);

    // For each iteration
    for (i = 1; i < ITERATION_NUM + 1; i++)
    {   
        // Find the global best particle for each iteration
        Particle globalBestParticle = localBestParticles[i - 1];

        // Update each particle
        #pragma omp parallel for
        for (j = 0; j < PARTICLES_NUM; j++)
        {
            int threadNum = omp_get_thread_num();
            updateParticle(&particles[j], globalBestParticle, lowerBounds, upperBounds, threadNum + baseSeed);
        }

        // Find the global best particle for all particles after one updation
        localBestParticles[i] = findBestParticle(particles, PARTICLES_NUM);
    }

    double endTime = omp_get_wtime();
    double elapsedTime = endTime - startTime;
    elapsedTime /= thread;  // average time

    // The final global best particle after all iterations
    Particle globalBestParticle = localBestParticles[ITERATION_NUM];

    // Output the results
    printf("%.5f\n", globalBestParticle.bestError);
    printf("%.5f %.5f %.5f %.5f\n", globalBestParticle.bestParameters[0], globalBestParticle.bestParameters[1], 
        globalBestParticle.bestParameters[2], globalBestParticle.bestParameters[3]);
    printf("%f\n", elapsedTime);
    if (printChar == 'p')
        for (int i = 0; i < ITERATION_NUM + 1; i++)
            printf("%.5f\n", localBestParticles[i].bestError);

    return 0;
}