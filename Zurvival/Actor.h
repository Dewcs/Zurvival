#pragma once
#include "Functions.h"

class Actor
{
protected:
	double x, y;
	double viewAngle;
	double hp;
	int begin;
public:
	Actor();
	~Actor();
	bool isDead();
	double getAngle();
	double getRawAngle();
	double getX();
	double getY();
};

