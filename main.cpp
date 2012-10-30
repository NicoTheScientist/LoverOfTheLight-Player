#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <vector>
#include <limits.h>
#include <libplayerc++/playerc++.h>
#include "RobotCS.h"
#include "RobotFA.h"
#include "Cupid.h"
#include "Breeder.h"
#include "Reaper.h"
#include "CandidateSolution.h"
#include "Agent.h"
#include "utils.cpp"

using namespace PlayerCc;

int main(int argc, char *argv[]) {
	
	/*******************************************************Init Phase*****************************************************************/
	
	/*** Candidate Solution Init ***/
	
	int nRobots=20;	
	std::vector<RobotCS*> robotsCS (nRobots);
		
	for(int i=0; i<nRobots; i++){
		robotsCS[i]=new RobotCS(6665+i,i,"cs%d");
		robotsCS[i]->updateSensors();
	}
		
	SimulationProxy* sp=new SimulationProxy(robotsCS[0]->getClient(),0);
	
	/*** Fate Agents Init ***/
	
	int nBreeders=6;
	int nCupids=6;
	int nReapers=8;

	int nFA=nReapers+nCupids+nBreeders;

	std::vector<RobotFA*> robotFA (nFA);
	std::vector<RobotFA*> robotB (nBreeders);
	std::vector<RobotFA*> robotC (nCupids);
	std::vector<RobotFA*> robotR (nReapers);

	for(int i=0; i<nFA; i++){
		if(i<nBreeders){
			robotFA[i]=new RobotFA(6665+nRobots+i,i,"fa%d",1);
			robotB.push_back(robotFA[i]);
		}
		else{
			if(i<nBreeders+nCupids){
				robotFA[i]=new RobotFA(6665+nRobots+i,i,"fa%d",2);
				robotC.push_back(robotFA[i]);
			}
			else{
				robotFA[i]=new RobotFA(6665+nRobots+i,i,"fa%d",3);
				robotR.push_back(robotFA[i]);
			}
		}
		robotFA[i]->updateSensors();
	}

    /*** Other Init ***/
    
    int nLights=50;
    item_t lights[nLights];
    RefreshItemList(lights, nLights, *sp);
    
	double forwardSpeed, turnSpeed;
	
    srand(time(NULL));
    

	/*******************************************************Evolution Phase*****************************************************************/

    // parameters
    int window_time_for_unit = 100;
    int FA_frequency = 5000;
    double cupidPR = 0.2;
    double reaperPR = 0.3;
    
    ulong time=0;
	while(true) {
		
        time++;
	
        /*** Now the robot moves ***/

		for(int i=0; i<nRobots; i++){
		
            // update sensors, set wheels velocity and discover if one light is catched
			robotsCS[i]->updateSensors();
			bool catched = robotsCS[i]->updateDirection(forwardSpeed,turnSpeed);
			
            // print velocity and angle for robot
//			std::cout<<robotsCS[i]->getID()<<"speed: "<<forwardSpeed<<std::endl;
//			std::cout<<robotsCS[i]->getID()<<"angle: "<<turnSpeed<<std::endl;
            
            // check if one light is catched
            if(catched) {
                // find which light is catched
                double robotX, robotY, robotYaw;
                sp->GetPose2d(robotsCS[i]->getID(), robotX, robotY, robotYaw);
                for(int iN=0; iN<nLights; iN++){
                    if(fabs(robotX-lights[iN].x)<0.5 && fabs(robotY-lights[iN].y)<0.5){
                        std::cout<<"Found: "<<lights[iN].name<<" x: "<<lights[iN].x<<" y: "<<lights[iN].y<<" "<<std::endl;
                        // update light position
                        sp->SetPose2d(lights[iN].name,rand()%51-25,rand()%51-25,0);
                        RefreshItemList(lights, nLights, *sp);
                        std::cout<<"Now: "<<lights[iN].name<<" is in x: "<<lights[iN].x<<" y: "<<lights[iN].y<<" "<<std::endl;
                        iN=nLights; // exit cycle
                    }
                }
                // increment lights counter
                (robotsCS[i]->getAgent())->addLight();
                std::cout<<robotsCS[i]->getID()<<" gained one light!"<<std::endl;
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
        
        if(time%FA_frequency==0){
        	/*** only randomly selected FA performs its action ***/
        	for(int i=0; i<robotC.size(); i++){
        		if((double)rand() / (RAND_MAX + 1.0) < cupidPR){
        			robotC[i]->updateSensors();
        			robotC[i]->searchNeighbourhood(robotsCS,robotFA,*sp);
        			Cupid* currentCupid=(Cupid*)robotC[i]->getAgent();
        			currentCupid->performAction();
        			
        			Breeder* currentBreeder=currentCupid->getRandomBreeder();
        			if(currentBreeder!=NULL){
        				bool parentFlag=true;
        				bool csFlag=true;
        				bool faFlag=true;
        				while(parentFlag && (csFlag || faFlag) ){
        					Agent** parents = currentCupid->getParents();
        					if(parents!=NULL){
								AgentType at=parents[0]->getType();
								switch(at) {
									case candidateSolution:{
										CandidateSolution* availableCS = currentCupid->getAvailableCS();
										if(availableCS!=NULL){
											currentBreeder->breed(parents,availableCS);
											availableCS->setActive(true);
											double** weights=(availableCS->getGenome()).weights;
											availableCS->getRobot()->setController(weights);
										}
										else
											csFlag=false;
										break;
									}
									case breeder:{
										Breeder* availableB = (Breeder*)currentCupid->getAvailableFA();
										if(availableB!=NULL){
											currentBreeder->breed(parents,availableB);
											availableB->setActive(true);
											robotB.push_back(availableB->getRobot());
										}
										else
											faFlag=false;
										break;
									}
									case cupid:{
										Cupid* availableC = (Cupid*)currentCupid->getAvailableFA();
										if(availableC!=NULL){
											currentBreeder->breed(parents,availableC);
											availableC->setActive(true);
											robotC.push_back(availableC->getRobot());
										}
										else
											faFlag=false;
										break;
									}
									case reaper:{
										Reaper* availableR = (Reaper*)currentCupid->getAvailableFA();
										if(availableR!=NULL){
											currentBreeder->breed(parents,availableR);
											availableR->setActive(true);
											robotR.push_back(availableR->getRobot());
										}
										else
											faFlag=false;
										break;
									}
								}
        					}
        					else
        						parentFlag=false;
        				}

        			}
        		}
        	}
        	
        	for(int i=0; i<robotR.size(); i++){
        		if((double)rand() / (RAND_MAX + 1.0) < reaperPR){
        			robotR[i]->updateSensors();
        			robotR[i]->searchNeighbourhood(robotsCS,robotFA,*sp);
        			Reaper* currentReaper=(Reaper*)robotR[i]->getAgent();
        			currentReaper->performAction();
        			bool toKillFlag=true;
        			while(toKillFlag){
        				Agent* toKill = currentReaper->getAgentToKill();
        				if(toKill!=NULL){
							toKill->setActive(false);
							AgentType at=toKill->getType();
							switch(at){
								case candidateSolution:{
									((CandidateSolution*)toKill)->getRobot()->getController().initWeights();
								}
								case breeder:{
									int k = -1;
									for(int i=0; i<robotB.size(); i++){
										if(robotB[i]==toKill->getRobot())
											k = i;		
									}
									if (k != -1)
										robotB.erase(robotB.begin() + k);
								}
								case cupid:{
									int k = -1;
									for(int i=0; i<robotC.size(); i++){
										if(robotC[i]==toKill->getRobot())
											k = i;		
									}
									if (k != -1)
										robotC.erase(robotC.begin() + k);
								}
								case reaper:{
									int k = -1;
									for(int i=0; i<robotR.size(); i++){
										if(robotR[i]==toKill->getRobot())
											k = i;		
									}
									if (k != -1)
										robotR.erase(robotR.begin() + k);
								}
							
							}
        				}
        				else
        					toKillFlag=false;
        			}
        		}
        	}
        }
        

		if(time==ULONG_MAX)
			reset(time);
	}
}
