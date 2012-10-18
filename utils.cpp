#include <libplayerc++/playerc++.h>

using namespace PlayerCc;

struct Item{
	char name[16];
	double x;
	double y;
}typedef item_t;

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

void reset(int& c){
	c=0;
}
