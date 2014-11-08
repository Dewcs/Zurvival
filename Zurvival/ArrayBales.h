#pragma once

enum weapon_t{
	WEAPON_PISTOL,
	WEAPON_SHOTGUN,
	WEAPON_HEAVY_MACHINEGUN
};

struct Bala{
	weapon_t parent;
	float firingRange;
	float x;
	float y;
	double directVector;
};

class ArrayBales{
	Bala *bales;
	int numBales;

public:
	ArrayBales(int numBales);
	~ArrayBales();
	void createBala(float shootSpeed, float x, float y, double directVector);
	void ompleCargador();
};

