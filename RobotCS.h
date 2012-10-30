#include <libplayerc++/playerc++.h>
#include "Robot.h"
#include "CandidateSolution.h"
#include "Controller.h"


using namespace PlayerCc;

#ifndef ROBOTCS_H
#define ROBOTCS_H

class CandidateSolution;

class RobotCS : public Robot{
    
    public:

	static const int nSensors=8;
	static const double maxSensorRange=1;
	static const double separation = 52*0.001; //52 mm

	RobotCS(int,int,const char*);
	~RobotCS();
	RangerProxy* getRP();
	void updateSensors();
	bool updateDirection(double&, double&);
    CandidateSolution* getAgent();
    void setAgent(CandidateSolution*);
	Controller getController();
	void setController(double**);
	void convertDifferential(double,double,double&,double&);

    private:
    
    CandidateSolution *agent;
    
    RangerProxy* rp;
	Controller c;
};

#endif
