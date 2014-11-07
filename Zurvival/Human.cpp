#include "Human.h"


Human::Human(int x, int y, int timestamp, std::string mode)
{
	this->x = x;
	this->y = y;
	viewAngle = 0;
	if (mode == "random") {
		ia = new Brain(16, 4);
		ia->randomize();
	}
	else if (mode == "empty") {
		ia = NULL;
	}
	else {
		ia = new Brain(16, 4);
		ia->load(mode.c_str());
		ia->tweak();
	}
	kills = 0;
	speed = 2;
	begin = timestamp;
	output = std::vector<float>(4, 0);
	input = std::vector<float>(16);
	type = ACTOR_ZOMBIE;
}


Human::~Human()
{
	if (ia != NULL) delete ia;
	input.clear();
	output.clear();
}

void Human::prepare(double cx, double cy, std::vector<Zombie*> zombies, Radar *sounds) {
	// closer zombies?
	int b1, b2, b3, btmp;
	float bd1, bd2, bd3, bdtmp;
	b1 = -1;
	b2 = -1;
	b3 = -1;
	for (int i = 0; i < zombies.size(); ++i) {
		float dist = distP2P(x, y, zombies[i]->getX(), zombies[i]->getY());
		if (b1 == -1) {
			b1 = i;
			bd1 = dist;
		}
		else if (b2 == -1) {
			b2 = i;
			bd2 = dist;
		}
		else if (b3 == -1) {
			b3 = i;
			bd3 = dist;
		}
		else if (dist < bd3) {
			b3 = i;
			bd3 = dist;
		}
		// swap c to b
		if (b3 != -1 && bd3 < bd2) {
			btmp = b3;
			bdtmp = bd3;
			b3 = b2;
			bd3 = bd2;
			b2 = btmp;
			bd3 = bdtmp;
		}
		if (b2 != -1 && bd2 < bd1) {
			btmp = b2;
			bdtmp = bd2;
			b2 = b1;
			bd2 = bd1;
			b1 = btmp;
			bd1 = bdtmp;
		}
	}
	// copy previous outputs
	for (int i = 0; i < output.size(); ++i) {
		input[i] = output[i];
	}
	// current position
	input[4] = x - cx;
	input[5] = y - cx;
	if (b1 != -1) {
		input[6] = zombies[b1]->getX() - x;
		input[7] = zombies[b1]->getY() - y;
	}
	else {
		input[6] = 0;
		input[7] = 0;
	}
	if (b2 != -1) {
		input[8] = zombies[b2]->getX() - x;
		input[9] = zombies[b2]->getY() - y;
	}
	else {
		input[8] = 0;
		input[9] = 0;
	}
	if (b3 != -1) {
		input[10] = zombies[b3]->getX() - x;
		input[11] = zombies[b3]->getY() - y;
	}
	else {
		input[10] = 0;
		input[11] = 0;
	}
	double sox, soy;
	bool sov;
	// closer sound
	sounds->getValue(x, y, sox, soy, sov);
	input[12] = 0;
	input[13] = 0;
	if (sov) {
		input[12] = sox - x;
		input[13] = soy - y;
	}
	// home point
	for (int i = 0; i < input.size(); ++i) {
		if (!std::isfinite(input[i])) log(VERBOSE_ERRORS, "BAD INPUT %d", i);
	}
	think();
}

void Human::update(unsigned delta) {
	if (output[0] != 0 || output[1] != 0) {
		viewAngle = angleP2P(0, 0, output[0], output[1]);
		x += cos(viewAngle) * 2 * delta / 1000.0;
		y += sin(viewAngle) * 2 * delta / 1000.0;
	}
}
Human* Human::clone(int x, int y, int timestamp) {
	Human *ret = new Human(x, y, timestamp, "empty");
	Brain *cpy = ia->copy();
	cpy->tweak();
	ret->setBrain(cpy);
	return ret;
}
double Human::capability(int timestamp) {
	return (timestamp-begin) / 60000.0;
}