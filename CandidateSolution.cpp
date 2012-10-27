#include "CandidateSolution.h"
#include "Controller.h"
#include <time.h>
#include <math.h>
#include <stdlib.h>

CandidateSolution::CandidateSolution()
{
	type = candidateSolution;
    
    srand(time(NULL));
    
    genome.weights = new double*[Controller::nInputs];
    for (int i = 0; i < Controller::nInputs; i++) {
        genome.weights[i] = new double[Controller::nOutputs];
        for (int j = 0; j < Controller::nOutputs; j++) {
            genome.weights[i][j] = rand() / double(RAND_MAX+1.0);
        }
    }
    for (int i = 0; i < windowSize; i++) {
        window[i] = 0;
    }
    
}

CandidateSolution::~CandidateSolution()
{
    
}

CandidateSolutionGenome CandidateSolution::getGenome()
{
	return genome;
}

void CandidateSolution::setGenome(CandidateSolutionGenome g) {
    genome = g;
}

void CandidateSolution::evaluateFitness()
{
    fitness = 0;
    for (int i = 0; i < windowSize; i++) {
        fitness += window[i];
    }
}

void CandidateSolution::addLight()
{
    window[0]++;
}

void CandidateSolution::shiftWindow()
{
    for (int i = windowSize - 2; i >= 0; i--) {
        window[i+1] = window[i];
    }
    window[0] = 0;
}