#include "weapon.h"


Weapon::Weapon(){

}
 

Weapon::~Weapon(){

}

bool Weapon::pucDisparar(){
	if (balesCarregador > 0 && tempsPerDisparar==0 && tempsPerRecarregar == 0){
		return true;
	}
	else{
		return false;
	}
}

int Weapon::balesCarregades(){
	return balesCarregador;
}

void Weapon::dispararBala(float x, float y, double vd, ArrayBales *ab, void *owner){
	--balesCarregador;
	tempsPerDisparar = tempsEntreDispars;
	for (int i = 0; i < balesPerDispar; i++){
		ab->createBala(distanciaDispar, x, y, vd + deg2rad(randomReal(-2, 2)),owner);
	}
	if (balesCarregador == 0 && balesTotals>0){
		tempsPerRecarregar = tempsRecarga;
	}
}

void Weapon::recarregarManual(){
	if (balesCarregador != tamanyCarregador && balesTotals != 0 && tempsPerRecarregar == 0) {
		tempsPerRecarregar = tempsRecarga;
	}
}


void Weapon::recarregarBales() {
	balesTotals = balesTotals + balesCarregador;
	balesCarregador = min(tamanyCarregador, balesTotals);
	balesTotals -= balesCarregador;
}

void Weapon::updateDelta(int ms){
	if (tempsPerDisparar>0) {
		tempsPerDisparar -= ms;
		if (tempsPerDisparar < 0) tempsPerDisparar = 0;
	}
	if (tempsPerRecarregar>0) {
		tempsPerRecarregar -= ms;
		if (tempsPerRecarregar <= 0) {
			tempsPerRecarregar = 0;
			recarregarBales();
		}
	}
}

int Weapon::totalBales() {
	return balesCarregador + balesTotals;
}

void Weapon::afegirBales(int quantitat) {
	balesTotals += quantitat;
	if (balesCarregador == 0 && tempsPerRecarregar == 0) tempsPerRecarregar = tempsRecarga;
}

int Weapon::returnBullets(bool carregador){
	if (carregador) return balesCarregador;
	else return balesTotals;
}

Pistol::Pistol() {
	tempsRecarga=2000;
	tempsEntreDispars=300;
	tamanyCarregador=15;
	balesCarregador=15;

	tempsPerDisparar=0;
	tempsPerRecarregar=0;
	balesTotals=30;

	balesPerDispar = 1;
	distanciaDispar=50;
}
Pistol::~Pistol() {

}

Shotgun::Shotgun(){
	tempsRecarga = 8000;
	tempsEntreDispars = 1000;
	tamanyCarregador = 8;
	balesCarregador = 8;

	tempsPerDisparar = 0;
	tempsPerRecarregar = 0;
	balesTotals = 16;

	balesPerDispar = 5;
	distanciaDispar = 15;
}

Shotgun::~Shotgun(){

}

Heavymachinegun::Heavymachinegun(){
	tempsRecarga = 15000;
	tempsEntreDispars = 50;
	tamanyCarregador = 100;
	balesCarregador = 100;

	tempsPerDisparar = 0;
	tempsPerRecarregar = 0;
	balesTotals = 200;

	balesPerDispar = 1;
	distanciaDispar = 25;
}

Heavymachinegun::~Heavymachinegun(){

}

