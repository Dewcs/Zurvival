#pragma once
#include <iostream>
#include <string>
#include "Const.h"
#include "Functions.h"
#include "Brain.h"
#include "Radar.h"
#include <istream>
#include <iostream>
#include <iomanip>
#include <sstream>
#include "ActorAI.h"

/*
	Class to handle zombies
*/

class Zombie:public ActorAI {
	int now; // current time
	int dmg_time; // how many times damaged
public:
	Zombie(int x, int y, int timestamp, std::string mode);
	~Zombie();
	void prepare(double cx, double cy, Radar *smells, Radar * sounds); // prepare AI
	void update(unsigned delta); // update zombie delta time
	Zombie* clone(int x, int y, int timestamp); // clone zombie
	void addKills(int k) { kills += k; hp += 60; }; // add 1 kill
	double capability(); // returns the capability of the zombie
	double getDamage(); // gets the damage dealt by the zombie
	bool canAttack(); // can the zombie attack?
};

