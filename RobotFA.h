#include <libplayerc++/playerc++.h>
#include "Robot.h"
#include "RobotCS.h"
#include "FateAgent.h"


using namespace PlayerCc;

#ifndef ROBOTFA_H
#define ROBOTFA_H

class FateAgent;

class RobotFA : public Robot{
public:
	RobotFA(int,int,const char*,int);
	~RobotFA();
    FateAgent* getAgent();
    void setAgent(FateAgent* a);
	void searchNeighbourhood(std::vector<RobotCS*>&, std::vector<RobotFA*>&,SimulationProxy&);

private:
    
    FateAgent *agent;
    
	int nbSize;
};

#endif
