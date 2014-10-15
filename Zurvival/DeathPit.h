#pragma once
#include <SDL.h>
#include "SpriteManager.h"
#include "Const.h"
class DeathPit
{
private:
	SDL_Renderer* renderer;
	SpriteManager* sprMngr;
public:
	DeathPit(SDL_Renderer* renderer, SpriteManager* sprMngr);
	~DeathPit();
	void listen(bool &end, order_t &order, int &value);
	void update(unsigned);
	void draw();
};

