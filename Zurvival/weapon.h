#pragma once
#include "ArrayBales.h"
#include "Functions.h"

/*
	Weapon class handler
*/

class Weapon{
protected:
	int tempsRecarga; // time to reloaed
	int tempsEntreDispars; // time between shots
	int tamanyCarregador; // how many bullets per charger
	int balesCarregador; // current bullets in charger

	int tempsPerDisparar; // time to be able to fire again
	int tempsPerRecarregar; // time to reload the weapon
	int balesTotals; // total unloaded bullets
	
	int balesPerDispar; // how many bullets per shot
	float distanciaDispar; // how far the bullets go
	
public:
	Weapon();
	~Weapon();
	bool pucDisparar(); // can i fire the gun?
	int balesCarregades(); // how many bullets in the charger
	void dispararBala(float x, float y, double vd, ArrayBales *b, void *owner); // fire 1 bullet
	void recarregarBales(); // reload bullets
	void recarregarManual(); // manual reload
	void updateDelta(int ms); // update gun
	int totalBales(); // how many bullets do i have
	void afegirBales(int quantitat); // add bullets
	int returnBullets(bool id); // get charger bullets
};

class Pistol :public Weapon{
public:
	Pistol();
	~Pistol();
};
class Shotgun :public Weapon{
public:
	Shotgun();
	~Shotgun();
};

class Heavymachinegun :public Weapon{
public:
	Heavymachinegun();
	~Heavymachinegun();
};


