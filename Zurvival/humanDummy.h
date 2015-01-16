#pragma once
#include "Functions.h"
#include <stdlib.h>

/*
	Dummy humans class to begin train it just moves into a constant unique direction untill it gets out of the map
*/

class humanDummy
{
private:
	double x, y; // current position
	double viewAngle; // angle of view direction
	bool alive;
public:
	humanDummy(double x,double y);
	~humanDummy();
	void update(unsigned delta); // how many ms passed since last update
	double getAngle(); // return the angle in degrees
	double getRawAngle(); // return angle in radians
	double getX(); // get the x position
	double getY(); // get the y position
	void kill();
	bool isDead();
};

