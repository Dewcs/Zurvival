#include "ArrayBales.h"


ArrayBales::ArrayBales(int numBales){
	this->numBales = numBales;
	bales = new Bala[this->numBales];
}


ArrayBales::~ArrayBales(){

}

void ArrayBales::createBala(float fr, float x, float y, double vd){
	Bala b;
	b.firingRange = fr;
	b.x = x;
	b.y = y;
	b.directVector = vd;
}

void ArrayBales::ompleCargador(){
	for (int i = 0; i < numBales;i++){
	}
}
