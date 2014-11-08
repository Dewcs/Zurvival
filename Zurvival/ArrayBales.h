#pragma once
#include <vector>
#include "Functions.h"

#define BULLET_SPEED 10

enum weapon_t{
	WEAPON_PISTOL,
	WEAPON_SHOTGUN,
	WEAPON_HEAVY_MACHINEGUN
};

struct Bala{
	weapon_t parent;
	float firingRange;
	float x,x0,x1;
	float y,y0,y1;
	double directVector;
};

class ArrayBales{
	std::vector<Bala> bales;
public:
	ArrayBales();
	~ArrayBales();
	void createBala(float shootSpeed, float x, float y, double directVector);
	void updateBales(unsigned delta);
	void getDrawInfo(int i,float &x0,float &y0, float &dist, float &angle);
	int size();
};

