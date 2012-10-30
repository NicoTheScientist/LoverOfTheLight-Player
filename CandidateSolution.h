#ifndef CANDIDATE_SOLUTION_H
#define CANDIDATE_SOLUTION_H

#include "Agent.h"
#include "RobotCS.h"

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
    void setRobot(RobotCS* r);
    virtual RobotCS* getRobot();
    
    static const int windowSize = 10;

    private:
	
    CandidateSolutionGenome genome;
    double window[windowSize];
    RobotCS* robot;

};

#endif

