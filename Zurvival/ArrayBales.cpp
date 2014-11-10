#include "ArrayBales.h"


ArrayBales::ArrayBales(){

}


ArrayBales::~ArrayBales(){
	bales.clear();
}

void ArrayBales::createBala(float fr, float x, float y, double vd, void *owner){
	Bala b;
	b.firingRange = fr;
	b.x = b.x0 = b.x1 = x;
	b.y = b.y0 = b.y1 = y;
	b.directVector = vd;
	b.owner = owner;
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
		bales[i].x1 = bales[i].x0 + cos(bales[i].directVector) * BULLET_SPEED * delta / 1000.0;
		bales[i].y1 = bales[i].y0 + sin(bales[i].directVector) * BULLET_SPEED * delta / 1000.0;
	}
}

void ArrayBales::getDrawInfo(int i, float &x0, float &y0, float &dist, float &angle) {
	x0 = bales[i].x0;
	y0 = bales[i].y0;
	dist = distP2P(x0, y0, bales[i].x1, bales[i].y1);
	angle = bales[i].directVector;
}

Segment ArrayBales::getBalaSegment(unsigned id) {
	return Segment(Point(bales[id].x0, bales[id].y0), Point(bales[id].x1, bales[id].y1));
}

int ArrayBales::size() {
	return bales.size();
}

void* ArrayBales::getBalaOwner(unsigned id) {
	return bales[id].owner;
}

void ArrayBales::unlinkOwner(void *owner) {
	for (int i = 0; i < bales.size(); ++i) {
		if (bales[i].owner == owner) {
			bales[i].owner = NULL;
		}
	}
}

void ArrayBales::remove(unsigned id) {
	bales.erase(bales.begin() + id);
}