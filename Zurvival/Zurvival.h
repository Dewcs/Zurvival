#pragma once
#include <iostream>
#include <vector>
#include <fstream>
#include "Const.h"
#include <SDL.h>
#include "SpriteManager.h"
#include "MainMenu.h"
#include "Options.h"
#include "DeathPit.h"
#include "Game.h"
#include "Config.h"
#include "GameOver.h"

/*
	Global class of the came containing all elements and menus
*/

class Zurvival {
	private:
		int width, height; // screen info
		float fps; // game frames per second
		unsigned last_time; // delta purposes
		SDL_Window *window;
		SDL_Renderer *renderer;
		SpriteManager *sprMngr;
		MainMenu *mainMenu;
		Options *options;
		DeathPit *deathPit;
		Config *config;
		Game *game;
		GameOver *gameOver;
		bool stop,pause; // did the game end
		page_t page; // current game page
		std::vector<int> scores; // score array
	public:
		Zurvival();
		~Zurvival();
		bool running(); // returns true if game didn't end
		void update(); // updates the game at constant fps rate
		void doOrder(order_t order, int value); // perform an order onto the game
	private:
		void load_sprites(); // load game sprites
		void addScore(int); // adds a score to the array
};


