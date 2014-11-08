#include "weapon.h"


Weapon::Weapon(){

}
 

Weapon::~Weapon(){

}

bool Weapon::pucDisparar(){
	if (balesCarregades2 == 0){
		return false;
	}
	else{
		return true;
	}
}

int Weapon::balesCarregades(){
	return balesCarregades2;
}

void Weapon::dispararBala(float x, float y, double vd, ArrayBales ab){
	balesCarregades2 = balesCarregades2 - 1;
	for (int i = 0; i > quantOfBala; i++){
		ab.createBala(firingRange, x, y, vd + deg2rad(randomReal(-5, 5)));
	}
}

void Weapon::recarregarBales(int pacDeBales){
	if (balesCarregades2 < tamanyCarregador){
	
	}
}

void Weapon::updateDelta(int ms){
	if(tempo == 0){
		tempo = tempsEntreBales;
		tempo = tempo - ms;
	}
}



