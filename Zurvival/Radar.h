#pragma once
#include "Functions.h"
#include <iostream>

// radar basic information
struct RNode {
	double x, y; // center x,y coord
	double value; //strength of the signal
	double radius; // distance traveled by the wave
	unsigned timestamp; // when did the signal begin
};
/*
	Radar class place signals in a position and expand them through time, used for sound waves and smell waves

*/
class Radar {
	private:
		double speed; // speed of signals
		double decayRate; // rate at with signal value decreases
		unsigned size; // max amount of signals
		unsigned amount; // current amount of signals
		RNode **list; // signal array
	public:
		Radar(double speed,double decayRate, unsigned size);
		~Radar();
		void addPoint(double x, double y, double value, unsigned timestamp); // ad a new signal starting at x,y coord with strength value at time timestamp
		void getValue(double x, double y, double &ox, double &oy,bool &valid); // get the closer signal to point x,y
		bool eval(RNode *a, RNode *b); // return the better node (sorting purposes)
		void update(unsigned delta); // update signals delta time
	private:
};

