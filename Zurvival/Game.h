#pragma once
#include <SDL.h>
#include "SpriteManager.h"
#include "Const.h"
#include "MainCharacter.h"
#include "Map.h"
#include "Functions.h"
#include "Zombie.h"
#include "Radar.h"

class Game
{
private:
	SDL_Renderer* renderer;
	SpriteManager* sprMngr;
	MainCharacter* mc;
	Zombie **zombies;
	int zcount;
	Map* gmap;
	int width, height;
	Radar *sounds, *smells;
public:
	Game(SDL_Renderer* renderer, SpriteManager* sprMngr, int width,int height);
	~Game();
	void listen(bool &end, bool &pause, order_t &order, int &value);
	void update(unsigned);
	void draw();
	void drawGUI();
};

