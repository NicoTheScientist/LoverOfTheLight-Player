#ifndef REAPER_H
#define REAPER_H

#include "FateAgent.h"

const double MAX_INITIAL_SELECTION_PROBABILITIES_REAPER = 0.6;

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

