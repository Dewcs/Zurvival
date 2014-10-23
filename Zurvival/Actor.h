#pragma once
#include "Functions.h"
class Actor {
protected:
	double x, y;
	double vx, vy;
	double viewAngle;
public:
	Actor();
	~Actor();
	void update(unsigned delta);
	double getAngle();
	double getRawAngle();
	double getX();
	double getY();
};

