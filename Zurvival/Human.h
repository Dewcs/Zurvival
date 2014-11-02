#pragma once
#include <vector>
#include "Brain.h"
#include "Radar.h"
#include "Zombie.h"
#include "Functions.h"

class Human
{
	double x, y;
	double viewAngle;
	Brain *ia;
	double steps;
	bool alive;
	int begin;
	std::vector<float> output, input;
public:
	Human(int x, int y, int timestamp, std::string mode);
	~Human();
	bool isDead();
	void update(unsigned delta,double cx,double cy, std::vector<Zombie*> zombies, Radar *sounds);
	double getAngle();
	double getRawAngle();
	double getX();
	double getY();
	Human* clone(int x, int y, int timestamp);
	void setBrain(Brain *brain);
	void save(const char *);
	double capability();
	void kill();
	unsigned minutes(unsigned timestamp) { return (timestamp-begin)/60000; };
};

