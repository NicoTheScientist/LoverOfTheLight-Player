#include "FateAgent.h"

FateAgent::FateAgent()
{
    
}

FateAgent::~FateAgent()
{
    
}

void FateAgent::setNeighbourhood(vector<CandidateSolution*> cs, vector<Breeder*> b,
                                     vector<Cupid*> c, vector<Reaper*> r, vector<Agent*> e)
{
	candidateSolutions = cs;
	breeders = b;
    cupids = c;
	reapers = r;
    empties = e;
    
    evaluateFitness();
}

void FateAgent::evaluateFitness()
{
    double max = -1.0;
	for (int i = 0; i < candidateSolutions.size(); i++)
	{
        if (candidateSolutions.at(i)->getFitness() > max)
        {
            max = candidateSolutions.at(i)->getFitness();
        }
	}
    fitness = max;
}