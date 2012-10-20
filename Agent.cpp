#include "Agent.h"


Agent::Agent()
{
	age = 0;
	fitness = -1;
    active = true;
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

bool Agent::isActive()
{
    return active;
}

void Agent::setActive(bool a)
{
    active = a;
}

bool Agent::compare(Agent* left, Agent* right) {
    return left->getFitness() < right->getFitness();
}
