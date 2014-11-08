#pragma once
#include "ArrayBales.h"
#include "Functions.h"

class Weapon{
protected:
	int tempsRecarga;
	int tempsEntreDispars;
	int tamanyCarregador;
	int balesCarregador;

	int tempsPerDisparar;
	int tempsPerRecarregar;
	int balesTotals;
	
	int balesPerDispar;
	float distanciaDispar;
	
public:
	Weapon();
	~Weapon();
	bool pucDisparar();
	int balesCarregades();
	void dispararBala(float x, float y, double vd, ArrayBales *b);
	void recarregarBales();
	void updateDelta(int ms);
	int totalBales();
	void afegirBales(int quantitat);
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


