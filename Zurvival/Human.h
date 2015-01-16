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

/*
	ActorAI Children to simulate humans (weapons and different senses)
*/

class Human: public ActorAI {
	int now; // current time
	int last_smell; // last time emited smell
	Weapon *main; // main waepon pointer
	bool fire; // is the human shooting?
	int shooted, hitted; // times shooted and hitted
public:
	Human(int x, int y, int timestamp, std::string mode); // human generation to pos x,y with timestamp and mode (random or filename)
	~Human();
	void prepare(double cx, double cy, std::vector<Zombie*> zombies, Radar *sounds, ItemMap *itemap); // prepare AI
	void update(unsigned delta, ArrayBales *ab); // update human delta time
	Human* clone(int x, int y, int timestamp); // clone humans
	double capability(); // check capability factor for trainer
	bool fired(); // did the human fire the weapon
	bool moved(); // did the human move
	bool emitSmell(); // did the human emit smell?
	void giveItem(item *i); // give item i to human
	void addHitted() { ++hitted; }; // add 1 hitted
};

