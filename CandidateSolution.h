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
    void addLight();
    void shiftWindow();
    
    static const int windowSize = 10;

    private:
	
    CandidateSolutionGenome genome;
    double window[windowSize];

};

#endif

