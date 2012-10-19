#include <libplayerc++/playerc++.h>
#include "RobotFA.h"
#include "RobotCS.h"

using namespace PlayerCc;

RobotFA::RobotFA(int port,int i,const char* c): Robot(port,i,c){
	nbSize=5;
}

RobotFA::~RobotFA(){}

void RobotFA::searchNeighbourhood(std::vector<RobotCS*> &rcs, std::vector<RobotFA*> &rfa, SimulationProxy &sp){
	std::cout<<"Neighbourhood of "<<getID()<<": "<<std::endl;
	for(int i=0; i<rcs.size(); i++){
		double targetX, targetY, targetYaw;
		sp.GetPose2d(rcs[i]->getID(), targetX, targetY, targetYaw);
							
		double agentX, agentY, agentYaw;
		sp.GetPose2d(getID(), agentX, agentY, agentYaw);
				
		if((targetX>agentX-nbSize && targetX<agentX+nbSize) && (targetY>agentY-nbSize && targetY<agentY+nbSize))
			std::cout<<rcs[i]->getID()<<std::endl;
	}
	for(int i=0; i<rfa.size(); i++){			
		double targetX, targetY, targetYaw;
		sp.GetPose2d(rfa[i]->getID(), targetX, targetY, targetYaw);
							
		double agentX, agentY, agentYaw;
		sp.GetPose2d(getID(), agentX, agentY, agentYaw);
				
		if((targetX>agentX-nbSize && targetX<agentX+nbSize) && (targetY>agentY-nbSize && targetY<agentY+nbSize))
			if(getID()!=rfa[i]->getID())
				std::cout<<rfa[i]->getID()<<std::endl;
	}
}

