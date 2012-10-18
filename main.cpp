#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <libplayerc++/playerc++.h>
#include "Robot.cpp"
#include "RobotCS.cpp"
#include "RobotFA.cpp"

using namespace PlayerCc;

void reset(int& c){
	c=0;
}

int main(int argc, char *argv[]) {
	
	/*******************************************************Init Phase*****************************************************************/

	int nbSize=5;
	
	/*** Candidate Solution Init ***/
	
	int nRobots=5;
	
	RobotCS* robots[nRobots];
	bool firstTimeCS[nRobots];
		
	for(int i=0; i<nRobots; i++){
		robots[i]=new RobotCS(6665+i,i,"cs%d");
		robots[i]->updateSensors();
		firstTimeCS[i]=true;
	}
		
	SimulationProxy* sp=new SimulationProxy(robots[0]->getClient(),0);
	
	/*** Init Fate Agents ***/
	
	int nReapers=2;
	int nCupids=2;
	int nBreeders=2;

	int nFA=nReapers+nCupids+nBreeders;
	
	RobotFA* agents[nFA];
	bool firstTimeFA[nFA];

	for(int i=0; i<nFA; i++){
		agents[i]=new RobotFA(6665+nRobots+i,i,"fa%d");
		agents[i]->updateSensors();
		firstTimeFA[i]=true;
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
				
			if(robots[i]->getPP()->GetStall()){
				double robotX, robotY, robotYaw;
				sp->GetPose2d(robots[i]->getID(), robotX, robotY, robotYaw);
				sp->SetPose2d(robots[i]->getID(), robotX, robotY, robotYaw+dtor(180));
				robots[i]->updateSensors();
			}
			else
				robots[i]->getPP()->SetSpeed(0.13, dtor(turnSpeed));
		}

	/*** Now it's the time of the FA! ***/

		for (int q=0; q<nFA; q++) {
			agents[q]->updateSensors();

	/*** Looking for the neighbourhood ***/
/*
			for(int i=0; i<nRobots; i++){			
				double targetX, targetY, targetYaw;
				char targetName[3];
				sprintf(targetName,"cs%d",i);
				sp->GetPose2d(targetName, targetX, targetY, targetYaw);
							
				double agentX, agentY, agentYaw;
				char agentName[3];
				sprintf(agentName,"fa%d",q);
				sp->GetPose2d(agentName, agentX, agentY, agentYaw);
				
				if((targetX>agentX-nbSize && targetX<agentX+nbSize) && (targetY>agentY-nbSize && targetY<agentY+nbSize))
					std::cout<<targetName<<std::endl;
			}
		*/
			if(count%25==0)
				turnSpeed = rand() % 181 - 90;
			else
				turnSpeed=0;
			
			if(agents[q]->getPP()->GetStall()){
				double robotX, robotY, robotYaw;
				sp->GetPose2d(agents[q]->getID(), robotX, robotY, robotYaw);
				sp->SetPose2d(agents[q]->getID(), robotX, robotY, robotYaw+dtor(180));
				agents[q]->updateSensors();
			}
			else
				agents[q]->getPP()->SetSpeed(0.13, dtor(turnSpeed));
			
		}
		
		if(count==50)
			reset(count); 
	}
}
