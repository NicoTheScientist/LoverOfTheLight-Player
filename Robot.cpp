#include <libplayerc++/playerc++.h>
#include "Robot.h"

using namespace PlayerCc;

Robot::Robot(int port,int i,const char* c){
	client = new PlayerClient("localhost",port);
	pp = new Position2dProxy(client,0);
	if(i<=9)
		id=new char[3];
	else
		if(i<=99)
			id=new char[4];
		else
			id=new char[5];
	sprintf(id, c ,i);
	active=true; 
}

Robot::~Robot(){
	delete id;
	delete pp;
	delete client;
}

PlayerClient* Robot::getClient(){
	return client;
}

Position2dProxy* Robot::getPP(){
	return pp;
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
}

char* Robot::getID(){
	return id;
}
