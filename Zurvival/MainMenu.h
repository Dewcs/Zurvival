#pragma once
#include <SDL.h>
#include "SpriteManager.h"
#include "Const.h"
class MainMenu {
	public:
		MainMenu();
		~MainMenu();
		void listen(bool &end,order_t &order,int &value);
		void update(unsigned);
		void draw(SDL_Renderer* renderer, SpriteManager* sprMngr);
};

