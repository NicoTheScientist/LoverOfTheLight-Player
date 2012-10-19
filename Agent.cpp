#include "Agent.h"


Agent::Agent()
{
	age = 0;
	fitness = -1;
}

int Agent::getAge()
{
	return age;
}

double Agent::getFitness()
{
	return fitness;
}

void Agent::increaseAge()
{
	age++;
}

AgentType Agent::getType()
{
	return type;
}
