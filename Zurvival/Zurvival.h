#pragma once
#include "Const.h"
#include <SDL.h>
#include "SpriteManager.h"
#include "MainMenu.h"


class Zurvival {
	private:
		int width, height, fps;
		unsigned last_time;
		SDL_Window *window;
		SDL_Renderer *renderer;
		SpriteManager *sprMngr;
		MainMenu *mainMenu;
		bool stop;
		page_t page;
	public:
		Zurvival();
		~Zurvival();
		bool running();
		void update();
		void doOrder(order_t order, int value);
	private:
		void load_sprites();
};


