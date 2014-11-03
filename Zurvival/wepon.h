#pragma once

class Weapon{
	float tempsRecarga;
	int tamanyCarregador;
	int balesCarregades;
	float shootsPerSecond;

public:
	Weapon();
	~Weapon();
	void pucDisparar();
	void balesCarregades();
	void dispararBala();
	void recarregarBales();
};

