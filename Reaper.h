#ifndef REAPER_H
#define REAPER_H

#include "FateAgent.h"

struct ReaperGenome {
    double probCS;
    double probB;
    double probC;
    double probR;
    int maxTournamentSize;
};

class Reaper : public FateAgent
{
    public:
	
    Reaper();
	~Reaper();
    ReaperGenome getGenome();
    void setGenome(ReaperGenome g);
	void performSelection();
	Agent* getAgentToKill();

    private:
    
    ReaperGenome genome;
    vector<Breeder*> selectedBreeders;
	vector<Reaper*> selectedReapers;
	vector<Cupid*> selectedCupids;
	vector<CandidateSolution*> selectedCandidateSolutions;
	
    template <class T> void selectForType(vector<T*> selectFrom, vector<T*> selectTo, bool(*compare)(Agent*, Agent*), double probability);
};

#endif

