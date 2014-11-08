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
#include "Weapon.h"

class Human: public ActorAI {
	int now;
	int last_smell;
	Weapon *main;
	bool fire;
public:
	Human(int x, int y, int timestamp, std::string mode);
	~Human();
	void prepare(double cx, double cy, std::vector<Zombie*> zombies, Radar *sounds, ItemMap *itemap);
	void update(unsigned delta, ArrayBales *ab);
	Human* clone(int x, int y, int timestamp);
	double capability();
	bool fired();
	bool moved();
	bool emitSmell();
	int minutes() { return (now - begin) / 60000; };
	void giveItem(item *i);
};

