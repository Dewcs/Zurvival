#include "ArrayBales.h"


ArrayBales::ArrayBales(int numBales){
	this->numBales = numBales;
	bales = new Bala[this->numBales];
}


ArrayBales::~ArrayBales(){

}

void ArrayBales::createBala(float sp, float x, float y, float vd, Bala b){
	b.shootSpeed = sp;
	b.x = x;
	b.y = y;
	b.directVector = vd;
}

void ArrayBales::ompleCargador(){
	for (int i = 0; i < numBales;i++){
	}
}
