#include <libplayerc++/playerc++.h>

using namespace PlayerCc;

struct Item{
	char name[16];
	double x;
	double y;
}typedef item_t;

void RefreshItemList(item_t *itemList,int nLights, SimulationProxy &simProxy){
	//get the poses of the light
	for(int i=0; i<nLights; i++){
		char lightStr[] = "light%d";
		sprintf(itemList[i].name, lightStr, i);
		double dummy; //dummy variable, don’t need yaws.
		simProxy.GetPose2d(itemList[i].name, itemList[i].x, itemList[i].y, dummy);
	}
	return;
}

bool checkAndSolveStall(Robot* r,SimulationProxy &sp){
	if(r->getPP()->GetStall()){
		double robotX, robotY, robotYaw;
		sp.GetPose2d(r->getID(), robotX, robotY, robotYaw);
		sp.SetPose2d(r->getID(), robotX, robotY, robotYaw+dtor(180));
		r->updateSensors();
		return true;
	}
	return false;
}

void reset(ulong& c){
	c=0;
}

int chooseRandom(int arity) {
    return rand() % arity;
}
/*
int searchByRobot(std::vector<Robot*> r,Robot* currentRobot){
	for(int i=0; i<r.size(); i++){
		if(r[i]==currentRobot)
			return i;		
	}
	return -1;
} */
