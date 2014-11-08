#pragma once
#include "ArrayBales.h"
#include "Functions.h"

class Weapon{
protected:
	int tempsRecarga;
	int tempsEntreBales;
	int tamanyCarregador;
	int balesCarregades2;
	float shootsPerSecond;
	int tempo;
	int quantOfBala;
	float firingRange;

public:
	Weapon();
	~Weapon();
	bool pucDisparar();
	int balesCarregades();
	void dispararBala(float x, float y, double vd, ArrayBales b);
	void recarregarBales(int pacDeBales);
	void updateDelta(int ms);
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


