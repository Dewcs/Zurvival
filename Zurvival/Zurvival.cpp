#include "Zurvival.h"


Zurvival::Zurvival() {
	SDL_Init(SDL_INIT_VIDEO);
	TTF_Init();
	// get screen resolution
	SDL_DisplayMode current;
	for (int i = 0; i < SDL_GetNumVideoDisplays(); ++i){
		int should_be_zero = SDL_GetCurrentDisplayMode(i, &current);

		if (should_be_zero != 0) SDL_Log("Could not get display mode for video display #%d: %s", i, SDL_GetError());

		else
			SDL_Log("Display #%d: current display mode is %dx%dpx @ %dhz. \n", i, current.w, current.h, current.refresh_rate);

	}
	this->width = width;
	this->height = height;
	this->window = SDL_CreateWindow(
		"Zurvival v0.0.0.0",         //    const char* title
		SDL_WINDOWPOS_UNDEFINED,  //    int x: initial x position
		SDL_WINDOWPOS_UNDEFINED,  //    int y: initial y position
		this->width,                      //    int w: width, in pixels
		this->height,                      //    int h: height, in pixels
		SDL_WINDOW_FULLSCREEN_DESKTOP          //    Uint32 flags: window options, see docs
	);
}


Zurvival::~Zurvival() {
}

bool Zurvival::running() {
	return true;
}

void Zurvival::update() {

}
