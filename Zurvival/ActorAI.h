#pragma once
#include "Actor.h"
#include "Brain.h"
class ActorAI:public Actor
{
protected:
	Brain *ia;
	std::vector<float> output, input;
	int kills;
	double damageDealt;
public:
	ActorAI();
	~ActorAI();
	void think();
	void setBrain(Brain *brain);
	void save(const char *);
	virtual void addKills(int k) { kills += k;  };
	int getKills() { return kills; };
	void addDamageDealt(double damage);
};

