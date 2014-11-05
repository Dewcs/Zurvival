#pragma once
#include "Actor.h"
#include "Brain.h"
class ActorAI:public Actor
{
protected:
	Brain *ia;
	std::vector<float> output, input;
public:
	ActorAI();
	~ActorAI();
	void setBrain(Brain *brain);
	void save(const char *);
};

