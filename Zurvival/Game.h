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
#include "TileManager.h"
#include "ParticleMap.h"

/*
	Class to handle all the game functionality
*/

class Game
{
private:
	SDL_Renderer* renderer;
	SpriteManager* sprMngr;
	MainCharacter* mc; // main character pointer
	Map* gmap; // map pointer
	int width, height; // screen resolution
	ItemMap *itemap; // items in the map

	Radar *sounds, *smells; // sound and smell radar

	ArrayBales *bales; // bullets inside game

	std::vector<Zombie*> zombies; // zombie array
	std::vector<Human*> humans; // human array

	Trainer *zTrainer, *hTrainer; // zombie/human trainers
	bool ended; // game ended?
	int begin; // hwen did the game begin
	TileManager *tm; // tile manager
	ParticleMap *pm; // blood particle map
public:
	Game(SDL_Renderer* renderer, SpriteManager* sprMngr, int width,int height);
	~Game();
	void listen(bool &end, bool &pause, order_t &order, int &value); // basic game scene functionallity
	void update(unsigned); // basic game scene functionality
	void draw(); // draw game
	void drawGUI(); // draw gui
	bool playerTouchItem(); //  did player touch an item
	void drawItems(); // draw items
private:
	void spawn(unsigned); // spawn humans/zombies
	void cleanup(); // cleanup decayed elements
};

