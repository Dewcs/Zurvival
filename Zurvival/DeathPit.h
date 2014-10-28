#pragma once
#include <SDL.h>
#include "SpriteManager.h"
#include "Const.h"
#include "Zombie.h"
#include "humanDummy.h"
#include "Radar.h"

class DeathPit
{
private:
	SDL_Renderer* renderer;
	SpriteManager* sprMngr;

	
	Radar *sounds, *smells;
	Zombie **zombies;
	humanDummy **humans;
	int hcount;
	int zcount;
	int width, height;

public:
	DeathPit(SDL_Renderer* renderer, SpriteManager* sprMngr, int width, int height);
	~DeathPit();
	void listen(bool &end, order_t &order, int &value);
	void update(unsigned);
	void draw();
};

