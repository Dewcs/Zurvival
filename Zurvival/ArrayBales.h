#pragma once
#include <vector>
#include "Functions.h"
#include "geometry.h"

enum weapon_t{
	WEAPON_PISTOL,
	WEAPON_SHOTGUN,
	WEAPON_HEAVY_MACHINEGUN
};


struct Bala{
	weapon_t parent; // who shoot the bullet
	float firingRange; // max distance of the bullet
	float x, x0, x1; // start_x, previous_x, next_x
	float y, y0, y1; // start_y, previous_y, next_y
	double directVector; // angle in radians with the direction of the bullet
	void* owner; // who shoot the bullet? dont use it only to compare ownership! can be a Human or a MainCharacter
};
// traveling speed of bullets

#define BULLET_SPEED 400 

/*
	Class to have all information about all bullets in the air inside the game
*/

class ArrayBales{
	std::vector<Bala> bales; // array of bullets
public:
	ArrayBales();
	~ArrayBales();
	void createBala(float shootSpeed, float x, float y, double directVector, void *owner); // add a new bullet
	void updateBales(unsigned delta); // update all bullets delta time
	void getDrawInfo(int id,float &x0,float &y0, float &dist, float &angle); // get information to draw bullet id
	Segment getBalaSegment(unsigned id); // get the last segment traveled by the bullet id
	int size(); // returns how many bullets are in the air
	void* getBalaOwner(unsigned id); // returns a pointer of the actor who shoot the bullet
	void unlinkOwner(void *owner); // removes the owner of a bullet (in case of death)
	void remove(unsigned id); // removes bullet id
};

