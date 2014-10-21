#pragma once
#include <SDL.h>
#include "SpriteManager.h"
#include "Const.h"
#include "MainCharacter.h"
#include "Map.h"
#include "Functions.h"

class Game
{
private:
	SDL_Renderer* renderer;
	SpriteManager* sprMngr;
	MainCharacter* mc;
	Map* gmap;
	int width, height;
public:
	Game(SDL_Renderer* renderer, SpriteManager* sprMngr, int width,int height);
	~Game();
	void listen(bool &end, order_t &order, int &value);
	void update(unsigned);
	void draw();
};

