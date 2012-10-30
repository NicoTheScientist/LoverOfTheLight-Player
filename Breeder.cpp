#include "Breeder.h"
#include "CandidateSolution.h"
#include "Cupid.h"
#include "Reaper.h"
#include "Agent.h"
#include <math.h>
#include <stdlib.h>

// Random number from a normal distribution, taken from: http://www.dreamincode.net/code/snippet1446.htm 
double randn_notrig(double mu=0.0, double sigma=1.0) {
	static bool deviateAvailable=false;        //        flag
	static float storedDeviate;                        //        deviate from previous calculation
	double polar, rsquared, var1, var2;

	//        If no deviate has been stored, the polar Box-Muller transformation is
	//        performed, producing two independent normally-distributed random
	//        deviates.  One is stored for the next round, and one is returned.
	if (!deviateAvailable) {

		//        choose pairs of uniformly distributed deviates, discarding those
		//        that don't fall within the unit circle
		do {
			var1=2.0*( double(rand())/double(RAND_MAX) ) - 1.0;
			var2=2.0*( double(rand())/double(RAND_MAX) ) - 1.0;
			rsquared=var1*var1+var2*var2;
		} while ( rsquared>=1.0 || rsquared == 0.0);

		//        calculate polar tranformation for each deviate
		polar=sqrt(-2.0*log(rsquared)/rsquared);

		//        store first deviate and set flag
		storedDeviate=var1*polar;
		deviateAvailable=true;

		//        return second deviate
		return var2*polar*sigma + mu;
	}

	//        If a deviate is available from a previous call to this function, it is
	//        returned, and the flag is set to false.
	else {
		deviateAvailable=false;
		return storedDeviate*sigma + mu;
	}
}


Breeder::Breeder()
{
	genome.CSMutationSize = -1.0;
	genome.FAMutationSize = -1.0;
	genome.tournamentMutationSize = -1.0;
	type = breeder;
}

Breeder::~Breeder()
{
    
}

BreederGenome Breeder::getGenome() {
    return genome;
}

void Breeder::setGenome(BreederGenome g) {
    genome = g;
}

void Breeder::performAction() {
    
}

void Breeder::breed(Agent** parents, Agent* emptyAgent)
{
	switch (parents[0]->getType())
	{
	case candidateSolution:
		{
            CandidateSolutionGenome newGenome;
            for (int i = 0; i < Controller::nInputs; i++) {
				for (int j = 0; j < Controller::nOutputs; j++) {
					double gene0 = ((CandidateSolution*)parents[0])->getGenome().weights[i][j];
					double gene1 = ((CandidateSolution*)parents[1])->getGenome().weights[i][j];
					doUniformCrossover(gene0, gene1, &(newGenome.weights[i][j]), 1.0);
				}
			}
			for (int i = 0; i < Controller::nInputs; i++) {
				for (int j = 0; j < Controller::nOutputs; j++) {
					doNonUniformMutation(&(newGenome.weights[i][j]), CS_MUTATION_RATE, genome.CSMutationSize, 0.0, 1.0);
				}
			}
            emptyAgent = new CandidateSolution();
            ((CandidateSolution*)emptyAgent)->setGenome(newGenome);
			break;
		}
	case cupid:
		{
            CupidGenome newGenome = ((Cupid*)parents[0])->getGenome();
			doNonUniformMutation(&(newGenome.probCS), FATE_MUTATION_RATE, genome.FAMutationSize, 0.0, 1.0);
            doNonUniformMutation(&(newGenome.probB), FATE_MUTATION_RATE, genome.FAMutationSize, 0.0, 1.0);
            doNonUniformMutation(&(newGenome.probC), FATE_MUTATION_RATE, genome.FAMutationSize, 0.0, 1.0);
            doNonUniformMutation(&(newGenome.probR), FATE_MUTATION_RATE, genome.FAMutationSize, 0.0, 1.0);
            CupidGenome genomeA = ((Cupid*)parents[0])->getGenome();
            CupidGenome genomeB = ((Cupid*)parents[1])->getGenome();
			doUniformCrossover(genomeA.maxTournamentSize, genomeB.maxTournamentSize, &(newGenome.maxTournamentSize));
			doGaussianMutation(&(newGenome.maxTournamentSize), FATE_MUTATION_RATE, genome.FAMutationSize, 1, 1);
            emptyAgent = new Cupid();
            ((Cupid*)emptyAgent)->setGenome(newGenome);
			break;
		}
	case reaper:
		{
            ReaperGenome newGenome = ((Reaper*)parents[0])->getGenome();
			doNonUniformMutation(&(newGenome.probCS), FATE_MUTATION_RATE, genome.FAMutationSize, 0.0, 1.0);
            doNonUniformMutation(&(newGenome.probB), FATE_MUTATION_RATE, genome.FAMutationSize, 0.0, 1.0);
            doNonUniformMutation(&(newGenome.probC), FATE_MUTATION_RATE, genome.FAMutationSize, 0.0, 1.0);
            doNonUniformMutation(&(newGenome.probR), FATE_MUTATION_RATE, genome.FAMutationSize, 0.0, 1.0);
            ReaperGenome genomeA = ((Reaper*)parents[0])->getGenome();
            ReaperGenome genomeB = ((Reaper*)parents[1])->getGenome();
			doUniformCrossover(genomeA.maxTournamentSize, genomeB.maxTournamentSize, &(newGenome.maxTournamentSize));
			doGaussianMutation(&(newGenome.maxTournamentSize), FATE_MUTATION_RATE, genome.FAMutationSize, 1, 1);
            emptyAgent = new Reaper();
            ((Reaper*)emptyAgent)->setGenome(newGenome);
			break;
		}
	case breeder:
		{
            BreederGenome newGenome = ((Breeder*)parents[0])->getGenome();
            doESLikeMutation(&(newGenome.CSMutationSize), BREEDER_LEARNING_RATE);
            doESLikeMutation(&(newGenome.FAMutationSize), BREEDER_LEARNING_RATE);
            doESLikeMutation(&(newGenome.tournamentMutationSize), BREEDER_LEARNING_RATE);
            emptyAgent = new Breeder();
            ((Breeder*)emptyAgent)->setGenome(newGenome);
			break;
		}
	default:
		{
			break;
		}
	}
}

void Breeder::doESLikeMutation(double *gene, double learning_rate)
{
    *(gene) *= exp(learning_rate * randn_notrig());
}

void Breeder::doGaussianMutation(int *gene, double mutationRate, double mutationSize, int lowerBound, int upperBound )
{
    if ((double)rand() / (RAND_MAX + 1.0) < mutationRate)
    {
        *(gene) += (int)((randn_notrig() * mutationSize) + 0.5);
        if (*(gene) > upperBound)
        {
            *(gene) = upperBound;
        }
        if (*(gene) < lowerBound)
        {
            *(gene) = lowerBound;
        }
    }
}

void Breeder::doNonUniformMutation(double* gene, double mutationRate, double mutationSize, double lowerBound, double upperBound )
{
    if ((double)rand() / (RAND_MAX + 1.0) < mutationRate)
    {
        *(gene) += randn_notrig() * mutationSize;
    }
    if (*(gene) > upperBound)
    {
        *(gene) = upperBound;
    }
    if (*(gene) < lowerBound)
    {
        *(gene) = lowerBound;
    }
}

void Breeder::doUniformCrossover( int geneA, int geneB, int *target )
{
    if ((double)rand() / (RAND_MAX + 1.0) < 0.5)
    {
        *(target) = geneA;
    }
    else
    {
        *(target) = geneB;
    }
}

void Breeder::doUniformCrossover( double geneA, double geneB, double* target, double ratio )
{
	if ((double)rand() / (RAND_MAX + 1.0) < 0.5)
	{
		*(target) = (geneA * ratio) + (geneB * (1 - ratio));
	}
	else
	{
		*(target) = (geneB * ratio) + (geneA * (1 - ratio));
	}
}
