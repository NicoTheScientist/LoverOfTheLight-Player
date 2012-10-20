#include <libplayerc++/playerc++.h>
#include "Agent.h"

using namespace PlayerCc;

#ifndef ROBOT_H
#define ROBOT_H

class Robot {
public:
	Robot(int,int,const char*);
	~Robot();
	PlayerClient* getClient();
	Position2dProxy* getPP();
	void updateSensors();
	char* getID();
	Agent* getAgent();
	void setAgent(Agent* ag);

protected:
	Agent* a;

private:
	char* id;
	PlayerClient* client;
	Position2dProxy* pp;
};

#endif // !ROBOT_H

