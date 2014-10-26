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
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
	// load config
	config = new Config("config.xml");
	if (!config->isLoaded()) {
		config->setBool("show_fps", true);
	}
	// init SpriteManager
	sprMngr = new SpriteManager();
	load_sprites();
	// init pages
	mainMenu = new MainMenu(renderer,sprMngr);
	options = new Options(renderer, sprMngr);
	deathPit = new DeathPit(renderer, sprMngr);
	game = new Game(renderer, sprMngr,width,height);
	// set current page
	page = LOADING;
	// prepare for delta
	last_time = 0;
	stop = false;
}


Zurvival::~Zurvival() {
	delete sprMngr;
	delete mainMenu;
	delete options;
	delete game;
	delete config;
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
	order_t order = ORDER_NONE;
	int value = 0;
	switch (page) {
		case LOADING:
			order = ORDER_CHANGE_PAGE;
			value = MAINMENU;
			break;
		case MAINMENU:
			mainMenu->listen(stop,order,value);
			mainMenu->update(delta);
			mainMenu->draw();
			break;
		case DEATHPIT:
			deathPit->listen(stop, order, value);
			deathPit->update(delta);
			deathPit->draw();
			break;
		case OPTIONS:
			options->listen(stop, order, value);
			options->update(delta);
			options->draw();
			break;
		case GAME:
			game->listen(stop, order, value);
			game->update(delta);
			game->draw();
			break;
		default:
			break;
	}
	doOrder(order, value);
	if (config->getBool("show_fps")) {
		int fpsr = 1000;
		if (delta != 0) fpsr = 1000 / delta;
		sprMngr->drawNumber(renderer, fpsr, "red_numbers", width, 0, height / 50, ALIGN_RIGHT);
	}
	SDL_RenderPresent(renderer);
	last_time = now;
}

void Zurvival::load_sprites() {
	// menu sprites
	sprMngr->addImage(renderer, "bg", "sprites/bg.jpg", {0,0,width,height});
	sprMngr->addText(renderer, "menu_play", "PLAY", { 255, 0, 0 }, height / 10, "sprites/Gore Font II.ttf", { width * 3 / 8, height * 4 / 10, width / 4, height / 10 });
	sprMngr->addText(renderer, "menu_deathPit", "DEATH PIT", { 255, 0, 0 }, height / 10, "sprites/Gore Font II.ttf", { width * 3 / 8, height * 6 / 10, width / 4, height / 10 });
	sprMngr->addText(renderer, "menu_options", "OPTIONS", { 255, 0, 0 }, height / 10, "sprites/Gore Font II.ttf", { width *3 / 8, height *8 / 10, width / 4, height / 10 });
	sprMngr->addText(renderer, "menu_title", "ZURVIVAL", { 255, 0, 0 }, height / 6, "sprites/Gore Font II.ttf", { width / 20, height / 8, width / 2, height / 6 });
	sprMngr->addText(renderer, "back", "<- Go Back", { 255, 0, 0 }, height / 8, "sprites/Gore Font II.ttf", { width / 20, height *8 / 10, width / 8, height / 8 });
	sprMngr->addNumbers(renderer, "red_numbers", { 255, 0, 0 }, height / 50, "sprites/Gore Font II.ttf");
	// game sprites
	sprMngr->addImage(renderer, "grass", "sprites/grass.jpg", { 0, 0, width, height });
	sprMngr->addImage(renderer, "soldier", "sprites/soldier.png", { 0, 0, width, height });
	sprMngr->addImage(renderer, "zombie", "sprites/zombie.png", { 0, 0, width, height });
	sprMngr->addSpriteSheet(renderer, "zombie_anim", "sprites/zombie3.png", { 0, 0, width, height },48,48,1,0xFFFFFFFF);
}


void Zurvival::doOrder(order_t order, int value) {
	if (order == ORDER_CHANGE_PAGE) {
		page = (page_t)value;
		if (value == GAME) SDL_ShowCursor(0);
		else SDL_ShowCursor(1);
	}
}
