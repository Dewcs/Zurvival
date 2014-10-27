#include "Zombie.h"


Zombie::Zombie(int x,int y)
{
	this->x = x;
	this->y = y;
	viewAngle = 0;
	vx = 0;
	vy = 0;
	ia = new Brain(4,2);
	ia->randomize();
}


Zombie::~Zombie()
{
}

bool Zombie::isDead() {
	return false;
}

void Zombie::update(unsigned delta) {
	x += cos(viewAngle) * 2 * delta / 1000.0;
	y += sin(viewAngle) * 2 * delta / 1000.0;
}

double Zombie::getAngle() {
	return rad2deg(viewAngle);
}

double Zombie::getRawAngle() {
	return viewAngle;
}

double Zombie::getX() {
	return x;
}

double Zombie::getY() {
	return y;
}