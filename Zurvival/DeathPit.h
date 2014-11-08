#pragma once
#include <SDL.h>
#include <vector>
#include "SpriteManager.h"
#include "Const.h"
#include "Zombie.h"
#include "humanDummy.h"
#include "Radar.h"
#include "Trainer.h"
#include "Human.h"
#include "ItemMap.h"
#include "ArrayBales.h"
#include "geometry.h"

class DeathPit
{
private:
	int width, height;

	SDL_Renderer* renderer;
	SpriteManager* sprMngr;

	ItemMap *itemap;

	Radar *sounds, *smells;

	ArrayBales *bales;

	std::vector<Zombie*> zombies;
	std::vector<Human*> humans;

	Trainer *zTrainer,*hTrainer;

public:
	DeathPit(SDL_Renderer* renderer, SpriteManager* sprMngr, int width, int height);
	~DeathPit();
	void listen(bool &end, bool &pause, order_t &order, int &value);
	void update(unsigned);
	void draw();
private:
	void spawn(unsigned);
	void cleanup();
};

