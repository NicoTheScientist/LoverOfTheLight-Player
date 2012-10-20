#ifndef CANDIDATE_SOLUTION_H
#define CANDIDATE_SOLUTION_H

#include "Agent.h"

struct CandidateSolutionGenome {
    double** weights;
};

class CandidateSolution : public Agent {
   
    public:
	
    CandidateSolution();
    ~CandidateSolution();
    void evaluateFitness();
	CandidateSolutionGenome getGenome();
    void setGenome(CandidateSolutionGenome g);

    private:
	
    CandidateSolutionGenome genome;
};

#endif

