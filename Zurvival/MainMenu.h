#pragma once
#include <SDL.h>
#include "SpriteManager.h"
class MainMenu {
	public:
		MainMenu();
		~MainMenu();
		void listen(bool &end,int &order,int &value);
		void update(unsigned);
		void draw(SDL_Renderer* renderer, SpriteManager* sprMngr);
};

