#ifndef CANDIDATE_SOLUTION_H
#define CANDIDATE_SOLUTION_H

#include "Agent.h"

class CandidateSolution : public Agent {
   
    public:
	
    CandidateSolution();
    ~CandidateSolution();
    void evaluateFitness();
	double** getGenome();

    private:
	
    double** genome;
};

#endif

