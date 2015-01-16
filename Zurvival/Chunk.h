#pragma once
#include <SDL.h>
#include <iostream>
#include <math.h>
#include <set>
#include "Functions.h"
#include "Const.h"
#include "SpriteManager.h"
#include "ItemMap.h"
#include "TileManager.h"

/*
	CLass to handle the data of a small piece of the map, have linkts to its neigboors

	2D bidirectional list
*/

class Chunk {
	private:
		int *matrix; //pointer matrix 
		int x; // x position
		int y; // y position
		Chunk *left; // neighbord pointer left
		Chunk *right; // neighbord pointer right
		Chunk *top; // neighbord pointer top
		Chunk *bot; // neighbord pointer bot
		bool isCalled; // boolean is called
		std::set<unsigned> *exists; // pointer set exist chunk
	public:
		Chunk(int, int, std::set<unsigned>*); //constructor
		Chunk(int, int, std::set<unsigned>*, Chunk* r, Chunk* b, Chunk* l, Chunk* t); // constructor 2
		~Chunk(); //destructor
		void randomChunk(); //randomsize all values of chunk matrix
		void setChunk(int id,Chunk *chunk); //asign neighbords of chunk 
		Chunk* getChunk(int id); //return neighbords
		bool areDiferentChunk(int x, int y); // 
		void drawChunk(double centerX_M, double  centerY_M, int  width_pixels, int height_pixels, unsigned *drawn, SDL_Renderer* renderer, SpriteManager* sprMngr, TileManager *tm); // draw chunk if i need and draw neighbords if i need
		void spawnNeighbors(SDL_Rect window, ItemMap*); // create neighbords if i need
		void resetCalls(); // return booleans isCalled as false
		Chunk* search(int x, int y); // serch chunk 
		Chunk* _search(int x, int y, unsigned uid, std::set<unsigned> *visited); // return chunk 
		int getMapValue(int x, int y); // gets the tile value of the positions x,y relative to current chunk
	private:
		int countLinks(); // how many links does the chunck have?
		void debug(); // print the chunk
};

