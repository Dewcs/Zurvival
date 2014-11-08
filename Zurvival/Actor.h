#pragma once
#include "Functions.h"
#include "geometry.h"

class Actor
{
protected:
	double x, y;
	double viewAngle;
	double hp;
	double speed;
	int begin;
	actor_t type;
public:
	Actor();
	~Actor();
	bool isDead();
	double getAngle();
	double getRawAngle();
	double getX();
	double getY();
	int getTime(int timestamp);
	Circle getCircle();
	void doDamage(double damage);
};

