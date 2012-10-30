#include "FateAgent.h"
#include "RobotFA.h"

FateAgent::FateAgent()
{
    
}

FateAgent::~FateAgent()
{
    
}

void FateAgent::setNeighbourhood(vector<CandidateSolution*> cs, vector<Breeder*> b, vector<Cupid*> c,
                                 vector<Reaper*> r, vector<CandidateSolution*> avCS, vector<FateAgent*> avFA)
{
	candidateSolutions = cs;
	breeders = b;
    cupids = c;
	reapers = r;
    availableCS = avCS;
    availableFA = avFA;
    
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

void FateAgent::setRobot(RobotFA* r){
	robot=r;
}
    
RobotFA* FateAgent::getRobot(){
	return robot;
}
