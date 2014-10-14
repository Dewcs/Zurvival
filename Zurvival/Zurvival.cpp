#include "Zurvival.h"


Zurvival::Zurvival() {
	// init SDL
	SDL_Init(SDL_INIT_VIDEO);
	TTF_Init();
	IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);
	// get screen resolution
	SDL_DisplayMode current;
	for (int i = 0; i < SDL_GetNumVideoDisplays(); ++i){
		int should_be_zero = SDL_GetCurrentDisplayMode(i, &current);
		if (should_be_zero != 0) SDL_Log("Could not get display mode for video display #%d: %s", i, SDL_GetError());
		else if (i == 0) {
			width = current.w;
			height = current.h;
			fps = current.refresh_rate;
		}
	}
	// create window
	this->window = SDL_CreateWindow(
		"Zurvival v0.0.0.1",         //    const char* title
		SDL_WINDOWPOS_UNDEFINED,  //    int x: initial x position
		SDL_WINDOWPOS_UNDEFINED,  //    int y: initial y position
		this->width,                      //    int w: width, in pixels
		this->height,                      //    int h: height, in pixels
		SDL_WINDOW_FULLSCREEN_DESKTOP          //    Uint32 flags: window options, see docs
	);
	//SDL_SetWindowIcon(window, surface);
	// create renderer
	renderer = SDL_CreateRenderer(window, -1, 0);
	// init SpriteManager
	sprMngr = new SpriteManager();
	load_sprites();
	// init pages
	mainMenu = new MainMenu();

	// set current page
	page = LOADING;
	// prepare for delta
	last_time = 0;
	stop = false;
}


Zurvival::~Zurvival() {
	delete sprMngr;
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	IMG_Quit();
	TTF_Quit();
	SDL_Quit();
}

bool Zurvival::running() {
	return window != NULL && !stop;
}

void Zurvival::update() {
	unsigned now = SDL_GetTicks();
	unsigned delta = now - last_time;
	SDL_RenderClear(renderer);
	int order = 0;
	int value = 0;
	switch (page) {
		case LOADING:
			page = MAINMENU;
			break;
		case MAINMENU:
			mainMenu->listen(stop,order,value);
			if (order == 1) {
				page = (page_t)value;
			}
			mainMenu->update(delta);
			mainMenu->draw(renderer, sprMngr);
			break;
		default:
			break;
	}
	SDL_RenderPresent(renderer);
	last_time = now;
}

void Zurvival::load_sprites() {
	sprMngr->addImage(renderer, "bg", "sprites/bg.jpg", {0,0,width,height});
}

void Zurvival::end() {
	stop = true;
}

void Zurvival::make(int order, int value) {

}
