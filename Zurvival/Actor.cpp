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

int Actor::getTime(int now) {
	return now - begin;
}

Circle Actor::getCircle() {
	Point p(x, y);
	Circle c(p, 0.5);
	return c;
}

void Actor::doDamage(double damage) {
	hp -= damage;
}

double Actor::getLife(){
	return hp;
}