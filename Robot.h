#include <libplayerc++/playerc++.h>

using namespace PlayerCc;

#ifndef ROBOT_H
#define ROBOT_H

class Robot {
public:
	Robot(int,int,const char*);
	~Robot();
	PlayerClient* getClient();
	Position2dProxy* getPP();
	bool getActive();
	void setActive(bool);
	void updateSensors();
	char* getID();

private:
	char* id;
	PlayerClient* client;
	Position2dProxy* pp;
	bool active;
};

#endif // !ROBOT_H

