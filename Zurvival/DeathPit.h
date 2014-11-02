#pragma once
#include <SDL.h>
#include <vector>
#include "SpriteManager.h"
#include "Const.h"
#include "Zombie.h"
#include "humanDummy.h"
#include "Radar.h"
#include "Trainer.h"

class DeathPit
{
private:
	SDL_Renderer* renderer;
	SpriteManager* sprMngr;

	
	Radar *sounds, *smells;
	std::vector<Zombie*> zombies;
	std::vector<humanDummy*> humans;
	int width, height;
	Trainer *zTrainer;

public:
	DeathPit(SDL_Renderer* renderer, SpriteManager* sprMngr, int width, int height);
	~DeathPit();
	void listen(bool &end, bool &pause, order_t &order, int &value);
	void update(unsigned);
	void draw();
};

