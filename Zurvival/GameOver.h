#pragma once
#include <SDL.h>
#include <vector>
#include <iostream>
#include "SpriteManager.h"
#include "Const.h"

class GameOver
{
	SDL_Renderer* renderer;
	SpriteManager* sprMngr;
	std::vector<int> *scores;
	int width, height;
public:
	GameOver(SDL_Renderer* renderer, SpriteManager* sprMngr, std::vector<int> *scores, int width, int height);
	~GameOver();
	void listen(bool &end, bool &pause, order_t &order, int &value);
	void draw(); 
	void update(unsigned delta);
};

