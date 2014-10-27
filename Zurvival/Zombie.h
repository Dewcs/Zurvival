#pragma once
#include <iostream>
#include "Const.h"
#include "Functions.h"
#include "Brain.h"

class Zombie {
private:
	double x, y;
	double vx, vy;
	double viewAngle;
	Brain *ia;
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

