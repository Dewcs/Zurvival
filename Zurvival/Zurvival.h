#pragma once
#include <SDL.h>
#include "SpriteManager.h"

class Zurvival {
	private:
		int width, height, fps;
		unsigned last_time;
		SDL_Window *window;
		SDL_Renderer *renderer;
		SpriteManager *sprMngr;
		bool stop;
	public:
		Zurvival();
		~Zurvival();
		bool running();
		void update();
	private:
		void load_sprites();
};

