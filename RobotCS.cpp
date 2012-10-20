#include <libplayerc++/playerc++.h>
#include "RobotCS.h"
#include "CandidateSolution.h"

using namespace PlayerCc;

RobotCS::RobotCS(int port,int i,const char* c): Robot(port,i,c){
	rp = new RangerProxy(getClient(),0); 
	setAgent(new CandidateSolution());
}

RobotCS::~RobotCS(){
	delete rp;
}

RangerProxy* RobotCS::getRP(){
	return rp;
}

void RobotCS::updateSensors(){
	Robot::updateSensors();
	rp->RequestGeom();
}
