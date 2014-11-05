#include "Actor.h"


Actor::Actor()
{
}


Actor::~Actor()
{
}

bool Actor::isDead() {
	return hp<=0;
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
