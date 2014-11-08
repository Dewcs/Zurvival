#include "ArrayBales.h"


ArrayBales::ArrayBales(){

}


ArrayBales::~ArrayBales(){
	bales.clear();
}

void ArrayBales::createBala(float fr, float x, float y, double vd){
	Bala b;
	b.firingRange = fr;
	b.x = b.x0 = b.x1 = x;
	b.y = b.y0 = b.y1 = y;
	b.directVector = vd;
	bales.push_back(b);
}

void ArrayBales::updateBales(unsigned delta) {
	for (int i = bales.size() - 1; i >= 0; --i) {
		if (distP2P(bales[i].x, bales[i].y, bales[i].x0, bales[i].y0)>bales[i].firingRange) {
			bales.erase(bales.begin() + i);
		}
	}
	for (int i = 0; i < bales.size(); ++i) {
		bales[i].x0 = bales[i].x1;
		bales[i].y0 = bales[i].y1;
		bales[i].x1 = bales[i].x0 + cos(bales[i].directVector) * BULLET_SPEED;
		bales[i].y1 = bales[i].y0 + sin(bales[i].directVector) * BULLET_SPEED;
	}
}

void ArrayBales::getDrawInfo(int i, float &x0, float &y0, float &dist, float &angle) {
	x0 = bales[i].x0;
	y0 = bales[i].y0;
	dist = distP2P(x0, y0, bales[i].x1, bales[i].y1);
	angle = bales[i].directVector;
}

int ArrayBales::size() {
	return bales.size();
}