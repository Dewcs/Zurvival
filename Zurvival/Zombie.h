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

class Zombie:public ActorAI {
public:
	Zombie(int x, int y, int timestamp, std::string mode);
	~Zombie();
	void prepare(double cx, double cy, Radar *smells, Radar * sounds);
	void update(unsigned delta);
	Zombie* clone(int x, int y, int timestamp);
	void addKills(int k) { kills += k; hp += 60; };
	double capability(int timestamp);
};

