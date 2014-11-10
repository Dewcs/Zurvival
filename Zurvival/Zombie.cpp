#include "Zombie.h"


Zombie::Zombie(int x, int y, int timestamp, std::string mode)
{
	minx = maxx = this->x = x;
	miny = maxy = this->y = y;
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
	speed = 1.5;
	kills = 0;
	hp = 240;
	begin = timestamp;
	output = std::vector<float>(4,0);
	input = std::vector<float>(10);
	type = ACTOR_ZOMBIE;
	damageDealt = 0;
}


Zombie::~Zombie()
{
	if (ia!=NULL) delete ia;
	input.clear();
	output.clear();
}

void Zombie::prepare(double cx, double cy, Radar *smells, Radar * sounds) {
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
	think();
}

void Zombie::update(unsigned delta) {
	if (output[0] != 0 || output[1] != 0) {
		viewAngle = angleP2P(0, 0, output[0], output[1]);
		x += cos(viewAngle) * speed * delta / 1000.0;
		y += sin(viewAngle) * speed * delta / 1000.0;
		hp -= delta / 1000.0;
		minx = min(minx, x);
		maxx = max(maxx, x);
		miny = min(minx, y);
		maxy = max(maxx, y);
	}
	else {
		hp -= delta / 5000.0;
	}
}

Zombie* Zombie::clone(int x, int y, int timestamp) {
	Zombie *ret = new Zombie(x, y, timestamp,"empty");
	Brain *cpy = ia->copy();
	cpy->tweak();
	ret->setBrain(cpy);
	return ret;
}

double Zombie::capability() {
	double kps = 0;
	double area = (maxx - minx)*(maxy - miny);
	double af = 1.0 - 1000.0 / (1000.0 + area);
	double kf = 1.0 - 10.0 / (11 + kills);
	double df = 1.0 - 100.0 / (100.0 + damageDealt);
	kps = af * kf * df;
	return kps;
}

double Zombie::getDamage() {
	return max(10.0, hp / 10);
}