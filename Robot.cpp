#include <libplayerc++/playerc++.h>
#include "Robot.h"

using namespace PlayerCc;

Robot::Robot(int port,int i){
	client = new PlayerClient("localhost",port);
	pp = new Position2dProxy(client,0);
	rp = new RangerProxy(client,0); 
	if(i<=9)
		id=new char[3];
	else
		if(i<=99)
			id=new char[4];
		else
			id=new char[5];
	sprintf(id,"cs%d",i);
	active=true; 
}


Robot::~Robot(){
	delete id;
	delete rp;
	delete pp;
	delete client;
}

PlayerClient* Robot::getClient(){
	return client;
}

Position2dProxy* Robot::getPP(){
	return pp;
}

RangerProxy* Robot::getRP(){
	return rp;
}

bool Robot::getActive(){
	return active;
}

void Robot::setActive(bool b){
	active=b;
}

void Robot::updateSensors(){
	client->Read();
	pp->RequestGeom();
	rp->RequestGeom();
}

char* Robot::getID(){
	return id;
}
