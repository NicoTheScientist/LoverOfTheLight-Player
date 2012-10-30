#ifndef BREEDER_H
#define BREEDER_H

#include "FateAgent.h"
#include <vector>

using namespace std;

const double CS_MUTATION_RATE = 0.95;
const double FATE_MUTATION_RATE = 0.95;
const double BREEDER_LEARNING_RATE = 0.25;
const double MAX_INITIAL_CS_MUTATION_SIZE = 0.05;
const double MAX_INITIAL_FATE_MUTATION_SIZE = 0.4;
const double MAX_INITIAL_TOURNAMENT_MUTATION_SIZE = 0.4;

struct BreederGenome {
    double CSMutationSize;
    double FAMutationSize;
    double tournamentMutationSize;
};

class Breeder : public FateAgent
{
    public:
	
    Breeder();
	~Breeder();
	BreederGenome getGenome();
    void setGenome(BreederGenome g);
    virtual void performAction();
    void breed(Agent** parents, Agent* emptyAgent);

    private:
    
    BreederGenome genome;
    
    void doESLikeMutation(double *gene, double learning_rate);
    void doGaussianMutation(int *gene, double mutationRate, double mutationSize, int lowerBound, int upperBound);
    void doNonUniformMutation(double* gene, double mutationRate, double mutationSize, double lowerBound, double upperBound);
    void doUniformCrossover(int geneA, int geneB, int *target);

    void doUniformCrossover(double geneA, double geneB, double* target, double ratio);
};

#endif
