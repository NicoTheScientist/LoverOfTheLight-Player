#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <libplayerc++/playerc++.h>

using namespace PlayerCc;

struct Item{
	char name[16];
	double x;
	double y;
}typedef item_t;

void reset(int& c,bool* ft,int ncs, bool* ftfa,int nfa){
	c=0;
	for(int i=0; i<ncs; i++){
		*(ft+i)=true;
	}
	for(int j=0; j<nfa; j++){
		*(ftfa+j)=true;
	}
}

void RefreshItemList(item_t *itemList, SimulationProxy &simProxy){
	int i;
	//get the poses of the light
	for(i=0;i<5;i++){
		char lightStr[] = "light%d";
		sprintf(itemList[i].name, lightStr, i+1);
		double dummy; //dummy variable, don’t need yaws.
		simProxy.GetPose2d(itemList[i].name, itemList[i].x, itemList[i].y, dummy);
	}
	return;
}

int main(int argc, char *argv[]) {
	
	/*******************************************************Init Phase*****************************************************************/

	int nbSize=5;

	int nLights=5;

	item_t itemList[nLights];
	
	/*** Candidate Solution Init ***/
	
	int nRobots=5;
	double maxSensorRange=1;
	
 	PlayerClient* robots[nRobots];
	Position2dProxy* pp[nRobots];
	RangerProxy* rp[nRobots];
	
	bool firstTime[nRobots];
		
	for(int i=0; i<nRobots; i++){
		robots[i]=new PlayerClient("localhost", 6665+i);
		pp[i]=new Position2dProxy(robots[i],0);
		rp[i]=new RangerProxy(robots[i],0);
		firstTime[i]=true;
		robots[i]->Read();
		pp[i]->RequestGeom();
		rp[i]->RequestGeom();
	}
	
	SimulationProxy* sp=new SimulationProxy(robots[0],0);
	RefreshItemList(itemList, *sp);
	
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
			robots[i]->Read();
			rp[i]->RequestGeom();
			double weightedSum=0;
			double sum=0;
			double max=-1;
			
			/*** Are there lights? ***/
			
			for(int z=0; z<rp[i]->GetRangeCount(); z++){
				double temp=maxSensorRange-(rp[i]->GetRange(z));
				double angle=(rp[i]->GetElementPose(z).pyaw)*180/3.14159265;
				weightedSum+=temp*angle;
				sum+=temp;
				if(temp>max)
					max=temp;
			}
			
			/*** Nope... ***/
			
			if(sum==0){
			    if(count%25==0)
			    	weightedSum = rand() % 181 - 90;
				else
					weightedSum=0;
			}
			
			/*** Yeah! Are them near? ***/
			
			else{
				weightedSum=weightedSum/sum;
				if(max>0.95*maxSensorRange){
					double robotX, robotY, robotYaw;
					char name[3];
					sprintf(name,"cs%d",i);
					sp->GetPose2d(name, robotX, robotY, robotYaw);
					for(int iN=0; iN<nLights; iN++){
						std::cout<<"Inspecting light: "<<iN<<std::endl;
						if(fabs(robotX-itemList[iN].x)<0.5 && fabs(robotY-itemList[iN].y)<0.5){
							std::cout<<"Found: "<<itemList[iN].name<<" x: "<<itemList[iN].x<<" y: "<<itemList[iN].y<<" "<<std::endl;
							sp->SetPose2d(itemList[iN].name,rand()%51-25,rand()%51-25,0);
							RefreshItemList(itemList, *sp);
							std::cout<<"Now: "<<itemList[iN].name<<" is in x: "<<itemList[iN].x<<" y: "<<itemList[iN].y<<" "<<std::endl;
							iN=nLights;
						}
					}
				}
			}
			
			/*** Are we stucked in a wall? ***/
			
			if(pp[i]->GetStall() && firstTime[i]){
				double robotX, robotY, robotYaw;
				char name[3];
				sprintf(name,"cs%d",i);
				sp->GetPose2d(name, robotX, robotY, robotYaw);
				sp->SetPose2d(name, robotX, robotY, robotYaw+dtor(180));
				firstTime[i]=false;
			}
			else
				pp[i]->SetSpeed(0.13, dtor(weightedSum*0.4));
		} 

	/*** Now it's the time of the FA! ***/

		for (int q=0; q<nFA; q++) {
			agents[q]->Read();
			ppFA[q]->RequestGeom();
			double newTurnate=0;

	/*** Looking for the neighbourhood ***/

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
			
			if(ppFA[q]->GetStall() && firstTimeFA[q]){
				double robotX, robotY, robotYaw;
				char name[3];
				sprintf(name,"fa%d",q);
				sp->GetPose2d(name, robotX, robotY, robotYaw);
				sp->SetPose2d(name, robotX, robotY, robotYaw+dtor(180));
				firstTimeFA[q]=false;
			}
			else{
				if(count%25==0)
			    	newTurnate = rand() % 180 - 90;
				ppFA[q]->SetSpeed(0.13, dtor(newTurnate));
			}
		}
		
		if(count==50)
			reset(count,firstTime,nRobots,firstTimeFA,nFA);
	}
}
