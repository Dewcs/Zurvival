#pragma once
#include <SDL.h>
#include <iostream>
#include <math.h>
#include "Functions.h"
#include "Const.h"
#include "SpriteManager.h"

class Chunk {
	public:
		int *matrix;
		int x;
		int y;
		Chunk *left;
		Chunk *right;
		Chunk *top;
		Chunk *bot;
	public:
		Chunk(int,int);
		Chunk(int, int, Chunk* r, Chunk* b, Chunk* l , Chunk* t);
		~Chunk();
		void randomChunk();
		void setChunk(int id,Chunk *chunk);
		Chunk* getChunk(int id);
		bool areDiferentChunk(int x, int y);
		void drawChunk(double centerX_M, double  centerY_M, int  width_pixels, int height_pixels, unsigned *drawn, SDL_Renderer* renderer, SpriteManager* sprMngr);
		void spawnNeighbors(SDL_Rect window);
};

