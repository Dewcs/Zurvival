#pragma once
#include <iostream>
#include "Const.h"
#include "Functions.h"
#include "Actor.h"
class MainCharacter: public Actor{
	private:
		unsigned move;
	public:
		MainCharacter(int x,int y);
		~MainCharacter();
		void setView(int x, int y);
		void startMove(move_t move);
		void stopMove(move_t move);
		void update(unsigned delta);
		
};

