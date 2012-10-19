#include "CandidateSolution.h"
#include "Controller.h"


CandidateSolution::CandidateSolution()
{
	type = candidateSolution;
    
    srand(time(NULL));
    
    genome = new double*[Controller::nInputs];
    for (int i = 0; i < Controller::nInputs; i++) {
        genome[i] = new double[Controller::nOutputs];
        for (int j = 0; j < Controller::nOutputs; j++) {
            genome[i][j] = rand() / double(RAND_MAX);
        }
    }
    
}

CandidateSolution::~CandidateSolution()
{
	for (int i = 0; i < Controller::nInputs; i++) {
        delete [] genome[i];
    }
    delete [] genome;
}

void CandidateSolution::evaluateFitness()
{
	fitness = 0;
}

double** CandidateSolution::getGenome()
{
	return genome;
}