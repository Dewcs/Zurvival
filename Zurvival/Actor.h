#pragma once
#include "Functions.h"
#include "geometry.h"

/*
Class Actor
Holds information about a moving entity inside a 2-d map also handles its vitality status and death conditions
*/

class Actor
{
protected:
	double x, y; // position inside map
	double viewAngle; // looking direction (radians)
	double hp; // current hp 0 = dead
	double speed; // moving speed of the actor
	int begin; // milliseconds when the actor was created 
	actor_t type; // type of the actor
public:
	Actor();
	~Actor();
	bool isDead(); // checks if the actor is dead or alive
	double getAngle(); // returns the angle in degrees of the actor
	double getRawAngle(); // returns the angle in radians of the actor
	double getX(); // retuns the x position of the actor
	double getY(); // returns the y position of the actor
	int getTime(int now); // returns the time the actor has been alive
	Circle getCircle(); // returns a circle object with the space that the actor occupy
	void doDamage(double damage); // deal damage to the actor
	double getLife(); // get current hp of the actor
};

