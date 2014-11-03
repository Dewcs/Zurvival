#include "Zombie.h"


Zombie::Zombie(int x, int y, int timestamp, std::string mode)
{
	this->x = x;
	this->y = y;
	viewAngle = 0;

	if (mode == "random") {
		ia = new Brain(10, 4);
		ia->randomize();
	}
	else if (mode == "empty") {
		ia = NULL;
	}
	else {
		ia = new Brain(10,4);
		ia->load(mode.c_str());
		ia->tweak();
	}
	kills = 0;
	hp = 120000;
	begin = timestamp;
	output = std::vector<float>(4,0);
	input = std::vector<float>(10);
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

void Zombie::update(unsigned delta, double cx, double cy, Radar *smells, Radar * sounds) {
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
	for (int i = 0; i < output.size(); ++i) {
		input[i] = output[i];
	}
	input[4] = x - cx;
	input[5] = y - cy;
	input[6] = v1x;
	input[7] = v1y;
	input[8] = v2x;
	input[9] = v2y;
	for (int i = 0; i < input.size(); ++i) {
		if (!std::isfinite(input[i])) log(VERBOSE_ERRORS, "BAD INPUT %d", i);
	}
	ia->setInput(input);
	ia->evaluate();
	ia->getResult(output);
	log(VERBOSE_BRAIN, "ZDATA OUTPUT %f %f", output[0], output[1]);
	if (output[0] + output[1] != 0) {
		viewAngle = angleP2P(0, 0, output[0], output[1]);
		//viewAngle = angleP2P(0, 0, v1x+v2x, v1y+v2y);
		x += cos(viewAngle) * 1.9 * delta / 1000.0;
		y += sin(viewAngle) * 1.9 * delta / 1000.0;
		hp -= delta;
	}
	else {
		hp -= delta * 2;
	}
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
	return kps*kills;
}