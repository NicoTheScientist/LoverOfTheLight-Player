#include <libplayerc++/playerc++.h>
#include "RobotCS.h"
#include "CandidateSolution.h"
#include "Controller.h"

using namespace PlayerCc;

RobotCS::RobotCS(int port,int i,const char* id): Robot(port,i,id){
	rp = new RangerProxy(getClient(),0); 
	setAgent(new CandidateSolution());
	
	c.setWeights((((CandidateSolution*)a)->getGenome()).weights);
//	std::cout<<c.getWeights()[1][1]<<std::endl;
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

void RobotCS::readSensors(double &fwSpeed, double &angle){
	double readings[nSensors+1];
	readings[0]=1;
	for(int j=0; j<nSensors; j++)
		readings[j+1]=maxSensorRange-(rp->GetRange(j));
	double* output=c.fwd(readings);
	convertDifferential(output[0],output[1],fwSpeed,angle);
}

Controller RobotCS::getController(){
	return c;
}

void RobotCS::setController(Controller co){
	c=co;
}

void RobotCS::convertDifferential(double left,double right,double& newspeed,double& newturnrate){
	// which is the outer wheel and which is the inner?
	if (left > right) { // left is outer wheel

		// newspeed is average between inner and outer wheel speed
		newspeed = right + (left - right)/2;
		newturnrate = (right - left) / separation;
	}
	else if (right > left) { //right is outer wheel
		
		// newspeed is average between inner and outer wheel speed
		newspeed = left + (right - left)/2;
		newturnrate = (right - left) / separation;
	}
	else {
		newspeed = left; //could also be right
		newturnrate = 0;
	}

}
