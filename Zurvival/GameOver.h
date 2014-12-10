#pragma once
#include <SDL.h>
#include <iostream>
#include "SpriteManager.h"
#include "Const.h"

class GameOver
{
	SDL_Renderer* renderer;
	SpriteManager* sprMngr;
public:
	GameOver(SDL_Renderer* renderer, SpriteManager* sprMngr);
	~GameOver();
	void listen(bool &end, bool &pause, order_t &order, int &value);
	void draw(); 
	void update(unsigned delta);
};

