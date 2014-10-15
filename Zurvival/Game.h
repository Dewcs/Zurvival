#pragma once
#include <SDL.h>
#include "SpriteManager.h"
#include "Const.h"
class Game
{
private:
	SDL_Renderer* renderer;
	SpriteManager* sprMngr;
public:
	Game(SDL_Renderer* renderer, SpriteManager* sprMngr);
	~Game();
	void listen(bool &end, order_t &order, int &value);
	void update(unsigned);
	void draw();
};

