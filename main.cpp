#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <vector>
#include <libplayerc++/playerc++.h>
#include "RobotCS.h"
#include "RobotFA.h"
#include "utils.cpp"

using namespace PlayerCc;

int main(int argc, char *argv[]) {
	
	/*******************************************************Init Phase*****************************************************************/
	
	/*** Candidate Solution Init ***/
	
	int nRobots=5;	
	std::vector<RobotCS*> robotsCS (nRobots);
		
	for(int i=0; i<nRobots; i++){
		robotsCS[i]=new RobotCS(6665+i,i,"cs%d");
		robotsCS[i]->updateSensors();
	}
		
	SimulationProxy* sp=new SimulationProxy(robotsCS[0]->getClient(),0);
	
	/*** Fate Agents Init ***/
	
	int nBreeders=2;
	int nCupids=2;
	int nReapers=2;

	int nFA=nReapers+nCupids+nBreeders;

	std::vector<RobotFA*> robotFA (nFA);

	for(int i=0; i<nFA; i++){
		if(i<nBreeders)
			robotFA[i]=new RobotFA(6665+nRobots+i,i,"fa%d",1);
		else
			if(i<nBreeders+nCupids)
				robotFA[i]=new RobotFA(6665+nRobots+i,i,"fa%d",2);
			else
				robotFA[i]=new RobotFA(6665+nRobots+i,i,"fa%d",3);
		robotFA[i]->updateSensors();
	}
	
    /*** Other Init ***/
    
    int nLights=5;
    item_t lights[nLights];
    RefreshItemList(lights, *sp);
    
	double forwardSpeed, turnSpeed;
	
    srand(time(NULL));
    

	/*******************************************************Evolution Phase*****************************************************************/

    // parameters
    int window_time_for_unit = 100;
    int FA_frequency = 5000;
    
    int time=0;
	while(true) {
		
        time++;
	
        /*** Now the robot moves ***/

		for(int i=0; i<nRobots; i++){
		
            // update sensors, set wheels velocity and discover if one light is catched
			robotsCS[i]->updateSensors();
			bool catched = robotsCS[i]->readSensors(forwardSpeed,turnSpeed);
			
            // print velocity and angle for robot
			std::cout<<robotsCS[i]->getID()<<"speed: "<<forwardSpeed<<std::endl;
			std::cout<<robotsCS[i]->getID()<<"angle: "<<turnSpeed<<std::endl;
            
            // check if one light is catched
            if(catched) {
                // find which light is catched
                double robotX, robotY, robotYaw;
                char name[3];
                sprintf(name,"cs%d",i);
                sp->GetPose2d(name, robotX, robotY, robotYaw);
                for(int iN=0; iN<nLights; iN++){
                    std::cout<<"Inspecting light: "<<iN<<std::endl;
                    if(fabs(robotX-lights[iN].x)<0.5 && fabs(robotY-lights[iN].y)<0.5){
                        std::cout<<"Found: "<<lights[iN].name<<" x: "<<lights[iN].x<<" y: "<<lights[iN].y<<" "<<std::endl;
                        // update light position
                        sp->SetPose2d(lights[iN].name,rand()%51-25,rand()%51-25,0);
                        RefreshItemList(lights, *sp);
                        std::cout<<"Now: "<<lights[iN].name<<" is in x: "<<lights[iN].x<<" y: "<<lights[iN].y<<" "<<std::endl;
                        iN=nLights; // exit cycle
                    }
                }
                // increment lights counte
                (robotsCS[i]->getAgent())->addLight();
            }
            
            // shift the window if window time for unit is reached
            if (time % window_time_for_unit == 0) {
                (robotsCS[i]->getAgent())->shiftWindow();
            }
			
			if(!checkAndSolveStall(robotsCS[i],*sp))
				robotsCS[i]->getPP()->SetSpeed(0.13+forwardSpeed,turnSpeed);
		}

        /*** Now it's the time of the FA! ***/

        
        /*** each FA has to move ramdomly ***/
        
		for (int q=0; q<nFA; q++) {
            
			robotFA[q]->updateSensors();
			
			if(time % 25 == 0)
				turnSpeed = rand() % 181 - 90;
			else
				turnSpeed=0;
			
			if(!checkAndSolveStall(robotFA[q],*sp))
				robotFA[q]->getPP()->SetSpeed(0.13, dtor(turnSpeed));
		}
        
        
        /*** only randomly selected FA performs its action ***/
        
        int selectedFA = chooseRandom(nFA);
        
        /*** Looking for the neighbourhood ***/
        robotFA[selectedFA]->updateSensors();
        robotFA[selectedFA]->searchNeighbourhood(robotsCS,robotFA,*sp);
        robotFA[selectedFA]->getAgent()->performAction();
		
		//if(time==50)
		//  reset(count);
	}
}
