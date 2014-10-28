#pragma once
#include "Functions.h"
#include <iostream>

struct RNode {
	double x, y;
	double value;
	double radius;
	unsigned timestamp;
};
class Radar {
	private:
		double speed;
		double decayRate;
		unsigned size;
		unsigned amount;
		RNode **list;
	public:
		Radar(double speed,double decayRate, unsigned size);
		~Radar();
		void addPoint(double x, double y, double value, unsigned timestamp);
		void getValue(double x, double y, double &ox, double &oy,bool &valid);
		bool eval(RNode *a, RNode *b);
		void update(unsigned delta);
	private:
};

