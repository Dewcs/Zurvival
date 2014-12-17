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
	gameOver = NULL; // new GameOver (renderer, sprMngr, width, height);
	// set current page
	page = LOADING;
	// prepare for delta
	last_time = SDL_GetTicks();
	stop = false;
	pause = false;
	scores = std::vector<int> (0);
	if (fileExists("scores.dat")) {
		// load scores
		std::ifstream myfile;
		myfile.open("scores.dat", std::ios::in | std::ios::binary);
		int dummy;
		while (myfile.read((char*)&dummy, sizeof(int))) {
			scores.push_back(dummy);
		}
		sort(scores.begin(), scores.end());
		myfile.close();
	}
}


Zurvival::~Zurvival() {
	log(VERBOSE_BASIC, "DELETING ZURVIVAL");
	if (mainMenu != NULL) delete mainMenu;
	if (options != NULL) delete options;
	if (game != NULL) delete game;
	if (deathPit != NULL) delete deathPit;
	if (gameOver != NULL) delete gameOver;
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
	// save scores
	std::ofstream myfile;
	myfile.open("scores.dat", std::ios::out | std::ios::binary | std::ios::trunc);
	for (int i = 0; i < scores.size(); ++i) {
		myfile.write((char*)&scores[i], sizeof(int));
	}
	myfile.close();
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
			delta = 17;
			deathPit->listen(stop,pause, order, value);
			// keep training even if paused
			deathPit->update(delta);
			deathPit->draw();
			// speed up train
			while (SDL_GetTicks() - now<95 / 6 && DP_TIME_SPEED!=1) {
				deathPit->update(delta);
			}
			while (pause && DP_TIME_SPEED != 1) {
				deathPit->listen(stop, pause, order, value);
				deathPit->update(delta);
			}
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
		case GAMEOVER: // falta plantejament del listen i draw en la classe gameover
			gameOver->listen(stop,pause,order,value);
			if (!pause)gameOver->update(delta);
			gameOver->draw();
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
	//gameover sprites
	sprMngr->addImage("bg_gameOver", "sprites/bg_gameOver.jpg", {0,0,width,height});
	// death pit sprites
	sprMngr->addImage("pred", "sprites/pred.png", { 0, 0, width, height });
	sprMngr->addImage("pgreen", "sprites/pgreen.png", { 0, 0, width, height });
	sprMngr->addImage("pblue", "sprites/pblue.png", { 0, 0, width, height });
	sprMngr->addImage("pwhite", "sprites/pwhite.png", { 0, 0, width, height });
	//backgroundGUI sprite
	sprMngr->addImage("backgroundGUI", "sprites/backgroundGUI.png", { 0, 0, width / 10 + (width / 30), height });
	//slot sprites
	sprMngr->addImage("pistolSlot", "sprites/pistolSlot.png", { width / 30, width / 30, height / 8, height / 8 });
	sprMngr->addImage("shotgunSlot", "sprites/shotgunSlot.png", { width / 30, (width / 30)*2 + (height / 8), height / 8, height / 8 });
	sprMngr->addImage("hevySlot", "sprites/hevySlot.png", { width / 30, (width / 30) * 3 + (height / 8)*2, height / 8, height / 8 });
	sprMngr->addImage("healSlot", "sprites/healSlot.png", { width / 30, (width / 30) * 4 + (height / 8)*3, height / 8, height / 8 });
	//wepon sprites
	sprMngr->addImage("pistolWep", "sprites/pistolWep.png", { (width / 30) + (width / 60), (width / 30) + (width / 60), height / 15, height / 15 });
	sprMngr->addImage("shotgunWep", "sprites/shotgunWep.png", { (width / 30) + (width / 80), (width / 30) * 2 + (height / 8) + (width / 50), height / 10, height / 20 });
	sprMngr->addImage("hevyWep", "sprites/hevyWep.png", { (width / 30) + (width / 100), (width / 30) * 3 + (height / 8) * 2 + (width / 100), height / 10, height / 15 });
	//amo sprites
	sprMngr->addImage("pistolAmo", "sprites/pistolAmo.png", { 0,0,0,0});
	sprMngr->addImage("shotgunAmo", "sprites/shotgunAmo.png", { 0, 0, 0, 0 });
	sprMngr->addImage("hevyAmo", "sprites/hevyAmo.png", { 0, 0, 0, 0 });
	//heal sprite
	sprMngr->addImage("healSprite", "sprites/healSprite.png", { (width / 30) + (width / 100), (width / 30) * 4 + (height / 8) * 3 , height / 10, height / 10 });
	// retroceso button
	sprMngr->addImage("retroceso", "sprites/retroceso.png", { width / 40, (width / 30) * 5 + (height / 8) * 4, width / 12, height / 12 });
	// highscores
	sprMngr->addText("highscores", "HIGHSCORES", { 255, 0, 0 }, height / 8, "sprites/Gore Font II.ttf", { width / 2 - width / 12, height / 8, width / 6, height / 12 });
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
		if (gameOver != NULL){
			delete gameOver;
			gameOver = NULL;
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
		case GAMEOVER:
			gameOver = new GameOver(renderer, sprMngr, &scores, width, height);
			break;
		default:
			break;
		}
		// toogle cursor
		SDL_ShowCursor(int(value != GAME));
	}
	else if (order == ORDER_SET_SCORE) {
		addScore(value);
	}
}

void Zurvival::addScore(int score) {
	scores.push_back(score);
	sort(scores.begin(), scores.end());
	std::reverse(scores.begin(), scores.end());
	if (scores.size() > 10) {
		scores.pop_back();
	}
}
