#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <vector>
#include <libplayerc++/playerc++.h>
#include "Robot.cpp"
#include "RobotCS.cpp"
#include "RobotFA.cpp"
#include "utils.cpp"

using namespace PlayerCc;

int main(int argc, char *argv[]) {
	
	/*******************************************************Init Phase*****************************************************************/
	
	/*** Candidate Solution Init ***/
	
	int nRobots=5;
	
	std::vector<RobotCS*> robots (nRobots);
		
	for(int i=0; i<nRobots; i++){
		robots[i]=new RobotCS(6665+i,i,"cs%d");
		robots[i]->updateSensors();
	}
		
	SimulationProxy* sp=new SimulationProxy(robots[0]->getClient(),0);
	
	/*** Fate Agents Init ***/
	
	int nReapers=2;
	int nCupids=2;
	int nBreeders=2;
	int nFA=nReapers+nCupids+nBreeders;

	std::vector<RobotFA*> agents (nFA);

	for(int i=0; i<nFA; i++){
		agents[i]=new RobotFA(6665+nRobots+i,i,"fa%d");
		agents[i]->updateSensors();
	}
	
	double forwardSpeed, turnSpeed;
	
	int count=0;
	
	srand(time(NULL));

	/*******************************************************Evolution Phase*****************************************************************/

	while(true) {
		count++;
	
	/*** Now the robot moves ***/

		for(int i=0; i<nRobots; i++){
			robots[i]->updateSensors();

			if(count%25==0)
				turnSpeed = rand() % 181 - 90;
			else
				turnSpeed=0;
				
			if(!checkAndSolveStall(robots[i],*sp))
				robots[i]->getPP()->SetSpeed(0.13, dtor(turnSpeed));
		}

	/*** Now it's the time of the FA! ***/

		for (int q=0; q<nFA; q++) {
			agents[q]->updateSensors();

	/*** Looking for the neighbourhood ***/

			agents[q]->searchNeighbourhood(robots,agents,*sp);
			
			if(count%25==0)
				turnSpeed = rand() % 181 - 90;
			else
				turnSpeed=0;
			
			if(!checkAndSolveStall(agents[q],*sp))
				agents[q]->getPP()->SetSpeed(0.13, dtor(turnSpeed));
		}
		
		if(count==50)
			reset(count); 
	}
}
