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
#include "MainCharacter.h"
#include "Map.h"

class Game
{
private:
	SDL_Renderer* renderer;
	SpriteManager* sprMngr;
	MainCharacter* mc;
	Map* gmap;
	int width, height;
	ItemMap *itemap;

	Radar *sounds, *smells;

	ArrayBales *bales;

	std::vector<Zombie*> zombies;
	std::vector<Human*> humans;

	Trainer *zTrainer, *hTrainer;
public:
	Game(SDL_Renderer* renderer, SpriteManager* sprMngr, int width,int height);
	~Game();
	void listen(bool &end, bool &pause, order_t &order, int &value);
	void update(unsigned);
	void draw();
	void drawGUI();
	bool playerTouchItem();
private:
	void spawn(unsigned);
	void cleanup();
};

