#pragma once
#include <SDL.h>
#include <iostream>
#include "SpriteManager.h"
#include "Const.h"
class MainMenu {
	private:
		SDL_Renderer* renderer;
		SpriteManager* sprMngr;
	public:
		MainMenu(SDL_Renderer* renderer, SpriteManager* sprMngr);
		~MainMenu();
		void listen(bool &end,order_t &order,int &value);
		void update(unsigned);
		void draw();
};

