#include "humanDummy.h"


humanDummy::humanDummy(double x,double y)
{
	this->x = x;
	this->y = y;
	viewAngle = angleP2P(0, 0, rand() - RAND_MAX / 2, rand() - RAND_MAX / 2);
	alive = true;
}


humanDummy::~humanDummy()
{
}

void humanDummy::update(unsigned delta) {
	x += cos(viewAngle) * 1 * delta / 1000.0;
	y += sin(viewAngle) * 1 * delta / 1000.0;
}

double humanDummy::getAngle() {
	return rad2deg(viewAngle);
}

double humanDummy::getRawAngle() {
	return viewAngle;
}

double humanDummy::getX() {
	return x;
}

double humanDummy::getY() {
	return y;
}

void humanDummy::kill() {
	alive = false;
}

bool humanDummy::isDead() {
	return !alive;
}