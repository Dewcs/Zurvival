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

/*
	Class holding the game auto-trainer it handles the information about all zombies and humans and try to get a next generation of brains for them
*/

class DeathPit
{
private:
	int width, height; // sceen size

	SDL_Renderer* renderer;
	SpriteManager* sprMngr;

	ItemMap *itemap; // map item handler

	Radar *sounds, *smells; // sounds and smells of the game
	ArrayBales *bales; // bullets
	std::vector<Zombie*> zombies; // array of zombies
	std::vector<Human*> humans; // array of humans

	Trainer *zTrainer,*hTrainer; // huamns and zombie trainer

public:
	DeathPit(SDL_Renderer* renderer, SpriteManager* sprMngr, int width, int height);
	~DeathPit();
	void listen(bool &end, bool &pause, order_t &order, int &value); // basic menu function
	void update(unsigned); // update the state of the deathpit
	void draw(); // draw the current stats of the deathpit
private:
	void spawn(unsigned);// create new zombies and humans
	void cleanup(); // remove dead/far away humans/zombies
};

