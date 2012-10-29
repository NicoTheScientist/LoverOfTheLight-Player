#ifndef FATE_AGENT_H
#define FATE_AGENT_H

#include "Agent.h"
#include "CandidateSolution.h"
#include <vector>

class Breeder;
class Cupid;
class Reaper;

using namespace std;

class FateAgent : public Agent
{
    public:
	
    FateAgent();
	~FateAgent();
	void setNeighbourhood(vector<CandidateSolution*> cs, vector<Breeder*> b, vector<Cupid*> c,
                          vector<Reaper*> r, vector<CandidateSolution*> avCS, vector<FateAgent*> avFA);
    void evaluateFitness();
    virtual void performAction() = 0;
    
	
    protected:
    
    vector<CandidateSolution*> candidateSolutions;
	vector<Breeder*> breeders;
    vector<Cupid*> cupids;
	vector<Reaper*> reapers;
    vector<CandidateSolution*> availableCS;
    vector<FateAgent*> availableFA;
};

#endif


