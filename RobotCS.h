#include <libplayerc++/playerc++.h>
#include "Robot.h"
#include "Controller.h"
#include "CandidateSolution.h"


using namespace PlayerCc;

#ifndef ROBOTCS_H
#define ROBOTCS_H

class RobotCS : public Robot{
    
    public:

	static const int nSensors=8;
	static const double maxSensorRange=1;
	static const double separation = 52*0.001; //52 mm

	RobotCS(int,int,const char*);
	~RobotCS();
	RangerProxy* getRP();
	void updateSensors();
	bool readSensors(double&, double&);
    CandidateSolution* getAgent();
    void setAgent(CandidateSolution* a);
	Controller getController();
	void setController(Controller);
	void convertDifferential(double,double,double&,double&);

    private:
    
    CandidateSolution *agent;
    
    RangerProxy* rp;
	Controller c;
};

#endif