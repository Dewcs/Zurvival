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
	weapon_t parent;
	float firingRange;
	float x, x0, x1;
	float y, y0, y1;
	double directVector;
	void* owner;
};

#define BULLET_SPEED 400

class ArrayBales{
	std::vector<Bala> bales;
public:
	ArrayBales();
	~ArrayBales();
	void createBala(float shootSpeed, float x, float y, double directVector, void *owner);
	void updateBales(unsigned delta);
	void getDrawInfo(int id,float &x0,float &y0, float &dist, float &angle);
	Segment getBalaSegment(unsigned id);
	int size();
	void* getBalaOwner(unsigned id);
	void unlinkOwner(void *owner);
};

