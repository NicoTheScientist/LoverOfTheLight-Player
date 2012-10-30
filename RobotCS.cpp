#include <libplayerc++/playerc++.h>
#include "RobotCS.h"
#include "CandidateSolution.h"
#include "Controller.h"

using namespace PlayerCc;

RobotCS::RobotCS(int port,int i,const char* id): Robot(port,i,id){
	rp = new RangerProxy(getClient(),0); 
	setAgent(new CandidateSolution());
	agent->setRobot(this);
	c.setWeights((agent->getGenome()).weights);
//	std::cout<<c.getWeights()[1][1]<<std::endl;
}

RobotCS::~RobotCS(){
	delete rp;
    delete agent;
}

RangerProxy* RobotCS::getRP(){
	return rp;
}

void RobotCS::updateSensors(){
	Robot::updateSensors();
	rp->RequestGeom();
}

bool RobotCS::updateDirection(double &fwSpeed, double &angle){
	// read sensors and remember max value
    double readings[nSensors+1];
	readings[0]=1; // bias
	double max = -1;
    for(int j=0; j<nSensors; j++) {
        double sensorValue = maxSensorRange-(rp->GetRange(j));
		readings[j+1] = sensorValue;
//		std::cout<<"read "<<j<<":"<<sensorValue<<std::endl;
        if (sensorValue > max) {
            max = sensorValue;
        }
    }
    // set velocity and angle using controller (nn)
    double* output=c.fwd(readings);
	convertDifferential(output[0],output[1],fwSpeed,angle);
    // catch the light if max sensor value is big enough
    bool catched = false;
    if(max>0.88*maxSensorRange) {
        catched = true;
    }
    return catched;
}

Controller RobotCS::getController(){
	return c;
}

void RobotCS::setController(double** w){
	c.setWeights(w);
}

CandidateSolution* RobotCS::getAgent(){
    return agent;
}
 
void RobotCS::setAgent(CandidateSolution* a){
    agent=a;
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
