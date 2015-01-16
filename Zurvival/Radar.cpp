#include "Radar.h"


Radar::Radar(double speed, double decayRate, unsigned size)
{
	this->speed = speed;
	this->decayRate = decayRate;
	this->size = size;
	amount = 0;
	list = new RNode*[size];
}


Radar::~Radar()
{
	delete[] list;
}

void Radar::addPoint(double x, double y, double value, unsigned timestamp) {
	// if array is not full add a new node
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
		// remove worst node and add the new one
		RNode *tmp = new RNode();
		tmp->x = x;
		tmp->y = y;
		tmp->value = value;
		tmp->timestamp = timestamp;
		tmp->radius = 0;
		for (unsigned i = 0; i < amount; ++i) {
			if (eval(tmp, list[i])) {
				RNode *tmp2 = list[i];
				list[i] = tmp;
				delete tmp2;
				break;
			}
		}
	}
}

void Radar::getValue(double x, double y, double &ox, double &oy, bool &valid) {
	// set valid if there is a value 
	valid = false;
	for (int i = amount-1; i >= 0; --i) {
		if (list[i]->value < 1) return;
		else if (distP2P(x, y, list[i]->x, list[i]->y) < list[i]->radius) {
			ox = list[i]->x;
			oy = list[i]->y;
			valid = true;
			return;
		}
	}
	
}

void Radar::update(unsigned delta) {
	for (unsigned i = 0; i < amount; ++i) {
		list[i]->radius += delta / 1000.0 * speed;
		list[i]->value *= 1 - (decayRate * delta / 1000.0);
	}
	// now lets sort values
	bool swapped = true;
	unsigned j = 0;
	RNode *tmp;
	// bubble sort op! (should be sorted by default so no need for speed here)
	while (swapped && amount) {
		swapped = false;
		j++;
		for (unsigned i = 0; i < amount - j; i++) {
			if (eval(list[i],list[i+1])) {
				tmp = list[i];
				list[i] = list[i + 1];
				list[i + 1] = tmp;
				swapped = true;
			}
		}
	}
}

bool Radar::eval(RNode *a, RNode *b) {
	if (decayRate == 0) return a->timestamp > b->timestamp;
	else return a->value > b->value;
}