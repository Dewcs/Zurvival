#pragma once
#include "Actor.h"
#include "Brain.h"

/*
	Children class of actor add basic information to use AI functions (brain.cpp)
*/

class ActorAI:public Actor
{
protected:
	Brain *ia; // pointer to the brain structure
	std::vector<float> output, input; // input and output handlers of the AI
	int kills; // number of kills of the actor
	double damageDealt; // total damage dealt by the actor
	double minx, miny, maxx, maxy; // bounding rectangle of the movement done by the actor
public:
	ActorAI();
	~ActorAI();
	void think(); // evaluates brain
	void setBrain(Brain *brain); // assigns a brain to the actor
	void save(const char *); // stores the brain inside a file
	virtual void addKills(int k) { kills += k;  }; // add k kills to the actor
	int getKills() { return kills; }; // returns the kills of the actor
	void addDamageDealt(double damage); // add damage to damagedealt
};

