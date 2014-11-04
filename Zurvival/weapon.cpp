#include "wepon.h"


Weapon::Weapon(){
	if (type == 0){
		tempsRecarga = 20.0;
		tamanyCarregador = 12;
		shootsPerSecond = 0.8;
	}else if (type == 1){
		tempsRecarga = 20.0;
		tamanyCarregador = 5;
		shootsPerSecond = 0.8;
	}else {
		tempsRecarga = 20.0;
		tamanyCarregador = 12;
		shootsPerSecond = 0.8;
	}
}
 

Weapon::~Weapon(){

}

bool Weapon::pucDisparar(){
	if (balesCarregades == 0){
		return false;
	}
	else{
		return true;
	}
}

int Weapon::balesCarregades(){
	
}

void Weapon::dispararBala(){

}

void Weapon::recarregarBales(){

}
