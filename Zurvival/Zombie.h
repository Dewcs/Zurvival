#pragma once
#include <iostream>
#include "Const.h"
#include "Functions.h"

class Zombie {
private:
	double x, y;
	double vx, vy;
	double viewAngle;
public:
	Zombie(int x, int y);
	~Zombie();
	bool isDead();
	void update(unsigned delta);
	double getAngle();
	double getRawAngle();
	double getX();
	double getY();
};

