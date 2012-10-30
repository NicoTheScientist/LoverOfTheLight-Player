#include <libplayerc++/playerc++.h>
#include "RobotFA.h"
#include "RobotCS.h"
#include "CandidateSolution.h"
#include "Cupid.h"
#include "Reaper.h"
#include "Breeder.h"
#include "FateAgent.h"

using namespace PlayerCc;

RobotFA::RobotFA(int port,int i,const char* c,int j): Robot(port,i,c){
	nbSize=5;
	switch(j){
		case 1:
			setAgent(new Breeder());
			agent->setRobot(this);
			break;
		case 2:
			setAgent(new Cupid());
			agent->setRobot(this);
			break;
		case 3:
			setAgent(new Reaper());
			agent->setRobot(this);
			break;
	}
}

RobotFA::~RobotFA(){
    delete agent;
}

FateAgent* RobotFA::getAgent(){
    return agent;
}

void RobotFA::setAgent(FateAgent* a){
    agent=a;
}

void RobotFA::searchNeighbourhood(std::vector<RobotCS*> &rcs, std::vector<RobotFA*> &rfa, SimulationProxy &sp){
	std::vector<CandidateSolution*> cs;
	std::vector<Reaper*> r;
	std::vector<Cupid*> c;
	std::vector<Breeder*> b;
	std::vector<CandidateSolution*> availableCS;
	std::vector<FateAgent*> availableFA;
	
	for(int i=0; i<rcs.size(); i++){
		double targetX, targetY, targetYaw;
		sp.GetPose2d(rcs[i]->getID(), targetX, targetY, targetYaw);
							
		double agentX, agentY, agentYaw;
		sp.GetPose2d(getID(), agentX, agentY, agentYaw);
				
		if((targetX>agentX-nbSize && targetX<agentX+nbSize) && (targetY>agentY-nbSize && targetY<agentY+nbSize)){
			CandidateSolution* candidate=(CandidateSolution*)(rcs[i]->getAgent());
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
				FateAgent* fate=rfa[i]->getAgent();
				if(fate->isActive()){
					AgentType at=fate->getType();
					switch(at) {
						case breeder:
							b.push_back((Breeder*)fate);
							break;
						case cupid:
							c.push_back((Cupid*)fate);
							break;
						case reaper:
							r.push_back((Reaper*)fate);
							break;
					}
					
				}
				else
					availableFA.push_back(fate);
			}
				
		}
	}
    
    // set relative Fate Agent neighbourhood
    agent->setNeighbourhood(cs,b,c,r,availableCS, availableFA);
    
	
/*	if(cs.size()!=0 || c.size()!=0 || r.size()!=0 || b.size()!=0){
		std::cout<<getID()<<"'s Neighbourhood: "<<std::endl;
		if(cs.size()!=0)
			std::cout<<cs.size()<<" CandidateSolutions "<<std::endl;
		if(c.size()!=0)
			std::cout<<c.size()<<" Cupids "<<std::endl;
		if(r.size()!=0)
			std::cout<<r.size()<<" Reapers "<<std::endl;
		if(b.size()!=0)
			std::cout<<b.size()<<" Breeders "<<std::endl;
	}	*/	
}

