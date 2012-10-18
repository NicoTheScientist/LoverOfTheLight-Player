#include <libplayerc++/playerc++.h>
#include "Robot.h"

using namespace PlayerCc;

#ifndef ROBOTCS_H
#define ROBOTCS_H

class RobotCS : public Robot{
public:
	RobotCS(int,int,const char*);
	~RobotCS();
	RangerProxy* getRP();
	void updateSensors();

private:
	RangerProxy* rp;
};

#endif // !ROBOTCS_H

