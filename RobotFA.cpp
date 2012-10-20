#include <libplayerc++/playerc++.h>
#include "RobotFA.h"
#include "RobotCS.h"
#include "CandidateSolution.h"
#include "Cupid.h"
#include "Reaper.h"
#include "Agent.h"

using namespace PlayerCc;

RobotFA::RobotFA(int port,int i,const char* c,int j): Robot(port,i,c){
	nbSize=5;
	switch(j){
		case 1:
			setAgent(new Reaper());
			break;
		case 2:
			setAgent(new Cupid());
			break;
//		case 3:
//			setAgent(new Breeder());
//			break;
	}
}

RobotFA::~RobotFA(){}

void RobotFA::searchNeighbourhood(std::vector<RobotCS*> &rcs, std::vector<RobotFA*> &rfa, SimulationProxy &sp){
	std::vector<Agent*> cs;
	std::vector<Agent*> r;
	std::vector<Agent*> c;
//	std::vector<Agent*> b(20);
	std::vector<Agent*> availableCS;
	std::vector<Agent*> availableFA;
	
	for(int i=0; i<rcs.size(); i++){
		double targetX, targetY, targetYaw;
		sp.GetPose2d(rcs[i]->getID(), targetX, targetY, targetYaw);
							
		double agentX, agentY, agentYaw;
		sp.GetPose2d(getID(), agentX, agentY, agentYaw);
				
		if((targetX>agentX-nbSize && targetX<agentX+nbSize) && (targetY>agentY-nbSize && targetY<agentY+nbSize)){
			Agent* candidate=rcs[i]->getAgent();
			if(candidate->isActive())
				cs.push_back(candidate);
			else
				availableCS.push_back(candidate);
		}
	}
	for(int i=0; i<rfa.size(); i++){
		double targetX, targetY, targetYaw;
		sp.GetPose2d(rfa[i]->getID(), targetX, targetY, targetYaw);
							
		double agentX, agentY, agentYaw;
		sp.GetPose2d(getID(), agentX, agentY, agentYaw);
				
		if((targetX>agentX-nbSize && targetX<agentX+nbSize) && (targetY>agentY-nbSize && targetY<agentY+nbSize)){
			if(getID()!=rfa[i]->getID()){
				Agent* fate=rfa[i]->getAgent();
				if(fate->isActive()){
					AgentType at=fate->getType();
					switch(at) {
						case cupid:
							c.push_back(fate);
							break;						
						case reaper:
							r.push_back(fate);
							break;						
//						case breeder:
//							b.push_back(fate);
//							break;
					}
					
				}
				else
					availableFA.push_back(fate);
			}
				
		}
	}
	
	if(cs.size()!=0 || c.size()!=0 || r.size()!=0){
		std::cout<<getID()<<"'s Neighbourhood: "<<std::endl;
		if(cs.size()!=0)
			std::cout<<cs.size()<<" CandidateSolutions "<<std::endl;
		if(c.size()!=0)
			std::cout<<c.size()<<" Cupids "<<std::endl;
		if(r.size()!=0)
			std::cout<<r.size()<<" Reapers "<<std::endl;
	}		
}

