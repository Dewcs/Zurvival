#pragma once

class Weapon{
protected:
	float tempsRecarga;
	int tamanyCarregador;
	int balesCarregades2;
	float shootsPerSecond;

public:
	Weapon();
	~Weapon();
	bool pucDisparar();
	int balesCarregades();
	void dispararBala();
	void recarregarBales();
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


