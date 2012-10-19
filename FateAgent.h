#ifndef FATE_AGENT_H
#define FATE_AGENT_H

#include "Agent.h"
#include "Breeder.h"
#include "CandidateSolution.h"
#include <vector>

class Cupid;
class Reaper;

using namespace std;

class FateAgent : public Agent
{
    public:
	
    FateAgent();
	~FateAgent();
	void setNeighbourhood(vector<CandidateSolution*> cs, vector<Breeder*> b,
                                    vector<Cupid*> c, vector<Reaper*> r, vector<Agent*> e);
    void evaluateFitness();
    
	
    protected:
    
    vector<CandidateSolution*> candidateSolutions;
	vector<Breeder*> breeders;
    vector<Cupid*> cupids;
	vector<Reaper*> reapers;
    vector<Agent*> empties;
};

#endif


