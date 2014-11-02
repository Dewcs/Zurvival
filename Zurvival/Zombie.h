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

class Zombie {
private:
	double x, y;
	double lx, ly;
	double viewAngle;
	Brain *ia;
	int kills;
	int hp;
	int begin;
	std::vector<float> output,input;
public:
	Zombie(int x, int y, int timestamp, std::string mode);
	~Zombie();
	bool isDead();
	void update(unsigned delta,Radar *smells,Radar * sounds);
	double getAngle();
	double getRawAngle();
	double getX();
	double getY();
	Zombie* clone(int x, int y, int timestamp);
	void setBrain(Brain *brain);
	void addKills(int k) { kills += k; hp += k * 100000; };
	int getKills() { return kills; };
	void save(const char *);
	double capability(int timestamp);
};

