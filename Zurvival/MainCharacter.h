#pragma once
#include "Const.h"
#include "Functions.h"
class MainCharacter {
	private:
		double x, y;
		int vx, vy;
		double viewAngle;
		unsigned move;
	public:
		MainCharacter(int x,int y);
		~MainCharacter();
		void setView(int x, int y);
		void startMove(move_t move);
		void stopMove(move_t move);
		void update(unsigned delta);
};

