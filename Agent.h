#ifndef AGENT_H
#define AGENT_H

enum AgentType {candidateSolution, cupid, reaper, breeder};

class Agent{
    
    public:
    
	Agent();
	double getFitness();
	int getAge();
	void increaseAge();
	AgentType getType();
    bool isActive();
    void setActive(bool a);
	virtual void evaluateFitness() = 0;
    
    static bool compare(Agent* left, Agent* right);

    protected:
    
	double fitness;
	int age;
    AgentType type;
    bool active;
};

#endif

