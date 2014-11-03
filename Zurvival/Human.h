#pragma once
#include <vector>
#include <queue> 
#include "Brain.h"
#include "Radar.h"
#include "Zombie.h"
#include "Functions.h"

struct Point {
	double x, y;
};

class Human
{
	double x, y,hx,hy;
	double viewAngle;
	Brain *ia;
	double steps;
	bool alive;
	int begin;
	std::vector<float> output, input;
	std::queue<Point> pq;
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
	double capability(int timestamp);
	void kill();
	unsigned minutes(unsigned timestamp) { return capability(timestamp)*100; };
};

