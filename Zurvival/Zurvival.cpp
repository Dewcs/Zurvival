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
		if (should_be_zero != 0) {
			log(VERBOSE_ERRORS, "Could not get display mode for video display #%d: %s", i, SDL_GetError());
			exit(1);
		}
		else  {
			width = current.w;
			height = current.h;
			fps = current.refresh_rate;
		}
	}
	// create window
	window = SDL_CreateWindow(
		"Zurvival v0.0.0.1",         //    const char* title
		SDL_WINDOWPOS_UNDEFINED,  //    int x: initial x position
		SDL_WINDOWPOS_UNDEFINED,  //    int y: initial y position
		this->width,                      //    int w: width, in pixels
		this->height,                      //    int h: height, in pixels
		SDL_WINDOW_FULLSCREEN_DESKTOP          //    Uint32 flags: window options, see docs
	);
	if (window == 0) {
		log(VERBOSE_ERRORS, "Could not create window");
		exit(1);
	}
	//SDL_SetWindowIcon(window, surface);
	// create renderer
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
	if (renderer == 0) {
		log(VERBOSE_ERRORS, "Could not create renderer");
		exit(1);
	}
	// load config
	config = new Config("config.xml");
	if (!config->isLoaded()) {
		// basic donfig
		config->setBool("show_fps", true);
	}
	// init SpriteManager
	sprMngr = new SpriteManager(renderer);
	load_sprites();
	// init pages as NULL create when needed to save space and to reset them
	mainMenu = NULL; //new MainMenu(renderer, sprMngr);
	options = NULL; // new Options(renderer, sprMngr);
	deathPit = NULL; // new DeathPit(renderer, sprMngr, width, height);
	game = NULL; // new Game(renderer, sprMngr,width,height);
	// set current page
	page = LOADING;
	// prepare for delta
	last_time = SDL_GetTicks();
	stop = false;
	pause = false;
}


Zurvival::~Zurvival() {
	log(VERBOSE_BASIC, "DELETING ZURVIVAL");
	if (mainMenu != NULL) delete mainMenu;
	if (options != NULL) delete options;
	if (game != NULL) delete game;
	if (deathPit != NULL) delete deathPit;
	log(VERBOSE_BASIC, "DELETING SPRITE MANAGER");
	delete sprMngr;
	log(VERBOSE_BASIC, "DELETING CONFIG");
	delete config;
	log(VERBOSE_BASIC, "DELETING SDL %d",&renderer);
	SDL_DestroyRenderer(renderer);
	log(VERBOSE_BASIC, "DONE DELETING RENDERER");
	SDL_DestroyWindow(window);
	log(VERBOSE_BASIC, "DONE DELETING WINDOW");
	IMG_Quit();
	log(VERBOSE_BASIC, "DONE DELETING IMG");
	TTF_Quit();
	log(VERBOSE_BASIC, "DONE DELETING TTF");
	SDL_Quit();
	log(VERBOSE_BASIC, "DONE DELETING SDL");
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
			mainMenu->listen(stop,pause,order,value);
			if (!pause) mainMenu->update(delta);
			mainMenu->draw();
			break;
		case DEATHPIT:
			deathPit->listen(stop,pause, order, value);
			// keep training even if paused
			deathPit->update(delta);
			deathPit->draw();
			break;
		case OPTIONS:
			options->listen(stop,pause, order, value);
			if (!pause) options->update(delta);
			options->draw();
			break;
		case GAME:
			game->listen(stop,pause, order, value);
			if (!pause) game->update(delta);
			game->draw();
			break;
		default:
			break;
	}
	
	// calc fps normalizing a little bit
	if (delta != 0) fps = (fps * 99 + 1000.0 / delta) / 100;
	if (config->getBool("show_fps")) {
		sprMngr->drawNumber(round(fps), "red_numbers", width, 0, height / 50, ALIGN_RIGHT);
	}
	log(VERBOSE_FRAME_DRAW, "DELTA: %d, FPS: %.02f", delta, fps);
	SDL_RenderPresent(renderer);
	doOrder(order, value);
	last_time = now;
	if (pause) SDL_Delay(50);
}

void Zurvival::load_sprites() {
	// menu sprites
	sprMngr->addImage("bg", "sprites/bg.jpg", {0,0,width,height});
	sprMngr->addText("menu_play", "PLAY", { 255, 0, 0 }, height / 10, "sprites/Gore Font II.ttf", { width * 3 / 8, height * 4 / 10, width / 4, height / 10 });
	sprMngr->addText("menu_deathPit", "DEATH PIT", { 255, 0, 0 }, height / 10, "sprites/Gore Font II.ttf", { width * 3 / 8, height * 6 / 10, width / 4, height / 10 });
	sprMngr->addText("menu_options", "OPTIONS", { 255, 0, 0 }, height / 10, "sprites/Gore Font II.ttf", { width *3 / 8, height *8 / 10, width / 4, height / 10 });
	sprMngr->addText("menu_title", "ZURVIVAL", { 255, 0, 0 }, height / 6, "sprites/Gore Font II.ttf", { width / 20, height / 8, width / 2, height / 6 });
	sprMngr->addText("back", "<- Go Back", { 255, 0, 0 }, height / 8, "sprites/Gore Font II.ttf", { width / 20, height *8 / 10, width / 8, height / 8 });
	sprMngr->addNumbers("red_numbers", { 255, 0, 0 }, height / 50, "sprites/Gore Font II.ttf");
	// game sprites
	sprMngr->addImage("grass", "sprites/grass.jpg", { 0, 0, width, height });
	sprMngr->addImage("soldier", "sprites/soldier.png", { 0, 0, width, height });
	sprMngr->addImage("zombie", "sprites/zombie.png", { 0, 0, width, height });
	sprMngr->addSpriteSheet("zombie_anim", "sprites/zombie3.png", { 0, 0, width, height },48,48,1,0xFFFFFFFF);
	// death pit sprites
	sprMngr->addImage("pred", "sprites/pred.png", { 0, 0, width, height });
	sprMngr->addImage("pgreen", "sprites/pgreen.png", { 0, 0, width, height });
}


void Zurvival::doOrder(order_t order, int value) {
	if (order == ORDER_CHANGE_PAGE) {
		log(VERBOSE_BASIC, "CHANGED TO PAGE %d", value);
		page = (page_t)value;

		// delete scenes
		if (mainMenu != NULL) {
			delete mainMenu;
			mainMenu = NULL;
		} 
		if (options != NULL) {
			delete options;
			options = NULL;
		}
		if (game != NULL) {
			delete game;
			game = NULL;
		}
		if (deathPit != NULL) {
			delete deathPit;
			deathPit = NULL;
		}

		// create needed
		switch (value)
		{
		case MAINMENU:
			mainMenu = new MainMenu(renderer, sprMngr);
			break;
		case OPTIONS:
			options = new Options(renderer, sprMngr);
			break;
		case DEATHPIT:
			deathPit = new DeathPit(renderer, sprMngr, width, height);
			break;
		case GAME:
			game = new Game(renderer, sprMngr, width, height);
			break;
		default:
			break;
		}
		// toogle cursor
		SDL_ShowCursor(int(value != GAME));
	}
}
