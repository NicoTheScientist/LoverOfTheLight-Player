#include <libplayerc++/playerc++.h>
#include "Robot.h"

using namespace PlayerCc;

#ifndef ROBOTFA_H
#define ROBOTFA_H

class RobotFA : public Robot{
public:
	RobotFA(int,int,const char*);
	~RobotFA();
};

#endif // !ROBOTFA_H

