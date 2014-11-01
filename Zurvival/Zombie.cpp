#include "Zombie.h"


Zombie::Zombie(int x, int y, int timestamp, std::string mode)
{
	this->x = x;
	this->y = y;
	viewAngle = 0;
	lx = 0;
	ly = 0;
	if (mode == "random") {
		ia = new Brain(8, 2);
		ia->randomize();
	}
	else if (mode == "empty") {
		ia = NULL;
	}
	else {
		ia = new Brain(8,2);
		ia->load(mode.c_str());
		ia->tweak();
	}
	kills = 0;
	hp = 120000;
	begin = timestamp;
}


Zombie::~Zombie()
{
	delete ia;
}

bool Zombie::isDead() {
	return hp<=0;
}

void Zombie::update(unsigned delta, Radar *smells, Radar * sounds) {
	double smx, smy, sox, soy;
	bool smv, sov;
	smells->getValue(x, y, smx, smy, smv);
	sounds->getValue(x, y, sox, soy, sov);
	double v1x, v1y, v2x, v2y;
	v1x = v1y = v2x = v2y = 0;
	if (smv) {
		v1x = smx - x;
		v1y = smy - y;
	}
	if (sov) {
		v2x = sox - x;
		v2y = soy - y;
	}
	float input[] = { lx,ly,x,y,v1x, v1y, v2x, v2y };
	ia->setInput(input);
	ia->evaluate();
	float *output;
	unsigned size;
	ia->getResult(output, size);
	float rx = output[0];
	float ry = output[1];
	viewAngle = angleP2P(0, 0, rx, ry);
	//viewAngle = angleP2P(0, 0, v1x+v2x, v1y+v2y);
	x += cos(viewAngle) * 2 * delta / 1000.0;
	y += sin(viewAngle) * 2 * delta / 1000.0;
	hp -= delta;
}

double Zombie::getAngle() {
	return rad2deg(viewAngle);
}

double Zombie::getRawAngle() {
	return viewAngle;
}

double Zombie::getX() {
	return x;
}

double Zombie::getY() {
	return y;
}

void Zombie::setBrain(Brain *brain) {
	if (ia!=NULL) delete ia;
	ia = brain;
}

Zombie* Zombie::clone(int x, int y, int timestamp) {
	Zombie *ret = new Zombie(x, y, timestamp,"empty");
	ret->setBrain(ia->copy());
	ia->tweak();
	return ret;
}

void Zombie::save(int timestamp) {
	if (kills > 0) {
		double kps = (double)kills / (timestamp - begin);
		ia->store(("data/zombies/" + to_string_with_precision(kps,15) + ".xml").c_str());
	}
}