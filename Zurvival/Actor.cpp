#include "Actor.h"


Actor::Actor()
{
}


Actor::~Actor()
{
}

void Actor::update(unsigned delta) {
	x += cos(viewAngle) * 2 * delta / 1000.0;
	y += sin(viewAngle) * 2 * delta / 1000.0;
}

double Actor::getAngle() {
	return rad2deg(viewAngle);
}

double Actor::getRawAngle() {
	return viewAngle;
}

double Actor::getX() {
	return x;
}

double Actor::getY() {
	return y;
}