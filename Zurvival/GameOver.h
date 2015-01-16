#pragma once
#include <SDL.h>
#include <vector>
#include <iostream>
#include "SpriteManager.h"
#include "Const.h"

/*
	Class to handle highscores screen
*/

class GameOver
{
	SDL_Renderer* renderer;
	SpriteManager* sprMngr;
	std::vector<int> *scores; // array of scores
	int width, height; //screen size
public:
	GameOver(SDL_Renderer* renderer, SpriteManager* sprMngr, std::vector<int> *scores, int width, int height);
	~GameOver();
	void listen(bool &end, bool &pause, order_t &order, int &value);
	void draw(); 
	void update(unsigned delta);
};

