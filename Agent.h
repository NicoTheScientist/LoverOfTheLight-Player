#ifndef AGENT_H
#define AGENT_H

enum AgentType {candidateSolution, cupid, reaper, breeder, empty};

class Agent{
    
    public:
    
	Agent();
	double getFitness();
	int getAge();
	void increaseAge();
	AgentType getType();
	virtual void evaluateFitness() = 0;
    
    static bool compare(Agent* left, Agent* right);

    protected:
    
	double fitness;
	int age;
    AgentType type;
};

#endif

