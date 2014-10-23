#include "Zombie.h"


Zombie::Zombie(int x,int y)
{
	this->x = x;
	this->y = y;
	viewAngle = 0;
	vx = 0;
	vy = 0;
}


Zombie::~Zombie()
{
}

bool Zombie::isDead() {
	return false;
}
