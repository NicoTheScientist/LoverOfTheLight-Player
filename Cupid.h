#ifndef CUPID_H
#define CUPID_H

#include "FateAgent.h"

const double MAX_INITIAL_SELECTION_PROBABILITIES_CUPID = 0.6;

struct CupidGenome {
    double probCS;
    double probB;
    double probC;
    double probR;
    int maxTournamentSize;
};

class Cupid : public FateAgent
{

    public:
    
	Cupid();
	~Cupid();
    CupidGenome getGenome();
    void setGenome(CupidGenome g);
    void performSelection();
	Agent** getParents();
	Agent* getEmptyAgent();
	Breeder* getRandomBreeder();

    private:
	
    CupidGenome genome;
    vector<Breeder*> selectedBreeders;
	vector<Reaper*> selectedReapers;
	vector<Cupid*> selectedCupids;
	vector<CandidateSolution*> selectedCandidateSolutions;
    
    template <class T> void selectForType(vector<T*> selectFrom, vector<T*> selectTo, bool(*compare)(Agent*, Agent*), double probability);
};

#endif

