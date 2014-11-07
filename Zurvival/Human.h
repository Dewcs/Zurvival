#pragma once
#include <vector>
#include <queue> 
#include "Brain.h"
#include "Radar.h"
#include "Zombie.h"
#include "Actor.h"
#include "ActorAI.h"
#include "Functions.h"
#include "ItemMap.h"

class Human: public ActorAI {
	int now;
	int last_smell;
public:
	Human(int x, int y, int timestamp, std::string mode);
	~Human();
	void prepare(double cx, double cy, std::vector<Zombie*> zombies, Radar *sounds, item *closer);
	void update(unsigned delta);
	Human* clone(int x, int y, int timestamp);
	double capability();
	bool moved();
	bool emitSmell();
	void doDamage(double damage);
	int minutes() { return (now - begin) / 60000; };
};

