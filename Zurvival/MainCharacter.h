#pragma once
#include <iostream>
#include "Const.h"
#include "Functions.h"

class MainCharacter{
	private:
		unsigned move;
		double x, y;
		double vx, vy;
		double viewAngle;
	public:
		MainCharacter(int x,int y);
		~MainCharacter();
		void setView(int x, int y);
		void startMove(move_t move);
		void stopMove(move_t move);
		void update(unsigned delta);
		double getAngle();
		double getRawAngle();
		double getX();
		double getY();
};

