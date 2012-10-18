#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <libplayerc++/playerc++.h>
#include "Robot.cpp"

using namespace PlayerCc;

void reset(int& c,bool* ft,int ncs, bool* ftfa,int nfa){
	c=0;
	for(int i=0; i<ncs; i++){
		*(ft+i)=true;
	}
	for(int j=0; j<nfa; j++){
		*(ftfa+j)=true;
	}
}

int main(int argc, char *argv[]) {
	
	/*******************************************************Init Phase*****************************************************************/

	int nbSize=5;
	
	/*** Candidate Solution Init ***/
	
	int nRobots=5;
	
	Robot* robots[nRobots];
	
	bool firstTime[nRobots];
		
	for(int i=0; i<nRobots; i++){
		robots[i]=new Robot(6665+i,i);
		robots[i]->updateSensors();
		firstTime[i]=true;
		std::cout<<robots[i]->getID()<<std::endl;
	}
		
	SimulationProxy* sp=new SimulationProxy(robots[0]->getClient(),0);
	
	/*** Init Fate Agents ***/
	
	int nReapers=2;
	int nCupids=2;
	int nBreeders=2;
	
	int nFA=nReapers+nCupids+nBreeders;
	
	PlayerClient* agents[nFA];
	Position2dProxy* ppFA[nFA];
	
	bool firstTimeFA[nFA];

	for(int i=0; i<nFA; i++){
		agents[i]=new PlayerClient("localhost", 6665+nRobots+i);
		ppFA[i]=new Position2dProxy(agents[i],0);
		firstTimeFA[i]=true;
		agents[i]->Read();
		ppFA[i]->RequestGeom();
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
				
			robots[i]->getPP()->SetSpeed(0.13, dtor(turnSpeed));
			if(robots[i]->getPP()->GetStall() && firstTime[i]){
				double robotX, robotY, robotYaw;
				sp->GetPose2d(robots[i]->getID(), robotX, robotY, robotYaw);
				sp->SetPose2d(robots[i]->getID(), robotX, robotY, robotYaw+dtor(180));
				firstTime[i]=false;
			}
			else
				robots[i]->getPP()->SetSpeed(0.13, dtor(turnSpeed));
		}

	/*** Now it's the time of the FA! ***/

		for (int q=0; q<nFA; q++) {
			agents[q]->Read();
			ppFA[q]->RequestGeom();
			double newTurnate=0;

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
			
			if(ppFA[q]->GetStall() && firstTimeFA[q]){
				double robotX, robotY, robotYaw;
				char name[3];
				sprintf(name,"fa%d",q);
				sp->GetPose2d(name, robotX, robotY, robotYaw);
				sp->SetPose2d(name, robotX, robotY, robotYaw+dtor(180));
				firstTimeFA[q]=false;
			}
			else
				ppFA[q]->SetSpeed(0.13, dtor(turnSpeed));
			
		}
		
		if(count==50)
			reset(count,firstTime,nRobots,firstTimeFA,nFA); 
	}
}
