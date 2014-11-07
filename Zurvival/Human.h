#pragma once
#include <vector>
#include <queue> 
#include "Brain.h"
#include "Radar.h"
#include "Zombie.h"
#include "Actor.h"
#include "ActorAI.h"
#include "Functions.h"

class Human: public ActorAI {
	
public:
	Human(int x, int y, int timestamp, std::string mode);
	~Human();
	void prepare(double cx, double cy, std::vector<Zombie*> zombies, Radar *sounds);
	void update(unsigned delta);
	Human* clone(int x, int y, int timestamp);
	double capability(int timestamp);
};

