#include <libplayerc++/playerc++.h>
#include "Robot.h"
#include "RobotCS.h"

using namespace PlayerCc;

#ifndef ROBOTFA_H
#define ROBOTFA_H

class RobotFA : public Robot{
public:
	RobotFA(int,int,const char*,int);
	~RobotFA();
	void searchNeighbourhood(std::vector<RobotCS*>&, std::vector<RobotFA*>&,SimulationProxy&);

private:
	int nbSize;
};

#endif // !ROBOTFA_H

