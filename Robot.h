#include <libplayerc++/playerc++.h>

using namespace PlayerCc;

#ifndef ROBOT_H
#define ROBOT_H

class Robot {
public:
//	Robot();
	Robot(int,int);
	~Robot();
	PlayerClient* getClient();
	Position2dProxy* getPP();
	RangerProxy* getRP();
	bool getActive();
	void setActive(bool);
	void updateSensors();
	char* getID();

private:
	char* id;
	PlayerClient* client;
	Position2dProxy* pp;
	RangerProxy* rp;
	bool active;
};

#endif // !ROBOT_H

