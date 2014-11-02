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
	output = std::vector<float>(2,0);
	input = std::vector<float>(8);
}


Zombie::~Zombie()
{
	if (ia!=NULL) delete ia;
	input.clear();
	output.clear();
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
	std::vector<float> input(8);
	input[0] = output[0];
	input[1] = output[1];
	input[2] = x;
	input[3] = y;
	input[4] = v1x;
	input[5] = v1y;
	input[6] = v2x;
	input[7] = v2y;
	ia->setInput(input);
	ia->evaluate();
	ia->getResult(output);
	log(VERBOSE_BRAIN, "ZDATA OUTPUT %f %f", output[0], output[1]);
	viewAngle = angleP2P(0, 0, output[0], output[1]);
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
	Brain *cpy = ia->copy();
	cpy->tweak();
	ret->setBrain(cpy);
	return ret;
}

void Zombie::save(const char * fname) {
	/*if (kills > 0) {
		double kps = (double)kills / (timestamp - begin);
		ia->store(("data/zombies/" + to_string_with_precision(kps,15) + ".xml").c_str());
	}*/
	ia->store(fname);
}

double Zombie::capability(int timestamp) {
	double kps = 0;
	if (kills > 0) kps = (double)kills / (timestamp - begin);
	return kps;
}