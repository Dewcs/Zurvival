#include "Radar.h"


Radar::Radar(double speed, double decayRate, unsigned size)
{
	this->speed = speed;
	this->decayRate = decayRate;
	this->size = size;
	this->amount = 0;
	this->list = new RNode*[size];
}


Radar::~Radar()
{
}

void Radar::addPoint(double x, double y, double value, unsigned timestamp) {
	if (amount < size) {
		list[amount] = new RNode();
		list[amount]->x = x;
		list[amount]->y = y;
		list[amount]->value = value;
		list[amount]->timestamp = timestamp;
		list[amount]->radius = 0;
		++amount;
	}
	else {
		RNode *tmp = new RNode();
		tmp->x = x;
		tmp->y = y;
		tmp->value = value;
		tmp->timestamp = timestamp;
		tmp->radius = 0;
		for (int i = 0; i < amount; ++i) {
			if (eval(tmp, list[i])) {
				RNode *tmp2 = list[i];
				list[i] = tmp;
				delete tmp2;
				break;
			}
		}
	}
}

void Radar::getValue(double x, double y, double &ox, double &oy) {

}

void Radar::update(unsigned delta) {
	for (int i = 0; i < amount; ++i) {
		list[i]->radius += delta / 1000.0 * speed;
		list[i]->value *= 1 - decayRate;
	}
	// now lets sort values
}

bool Radar::eval(RNode *a, RNode *b) {
	if (decayRate == 0) return a->timestamp > b->timestamp;
	else return a->value > b->value;
}