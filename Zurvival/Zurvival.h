#pragma once
#include <SDL.h>
#include "SpriteManager.h"

class Zurvival {
	private:
		int width, height;
		SDL_Window *window;
	public:
		Zurvival();
		~Zurvival();
		bool running();
		void update();
};

