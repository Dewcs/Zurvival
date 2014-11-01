#pragma once
#include <SDL.h>
#include "SpriteManager.h"
#include "Const.h"
class Options
{
private:
	SDL_Renderer* renderer;
	SpriteManager* sprMngr;
public:
	Options(SDL_Renderer* renderer, SpriteManager* sprMngr);
	~Options();
	void listen(bool &end, bool &pause, order_t &order, int &value);
	void update(unsigned);
	void draw();
};

