#pragma once
#include <SDL.h>
#include <iostream>
#include <math.h>
#include <set>
#include "Functions.h"
#include "Const.h"
#include "SpriteManager.h"

class Chunk {
	private:
		int *matrix;
		int x;
		int y;
		Chunk *left;
		Chunk *right;
		Chunk *top;
		Chunk *bot;
		bool isCalled;
		std::set<unsigned> *exists;
	public:
		Chunk(int, int, std::set<unsigned>*);
		Chunk(int, int, std::set<unsigned>*, Chunk* r, Chunk* b, Chunk* l, Chunk* t);
		~Chunk();
		void randomChunk();
		void setChunk(int id,Chunk *chunk);
		Chunk* getChunk(int id);
		bool areDiferentChunk(int x, int y);
		void drawChunk(double centerX_M, double  centerY_M, int  width_pixels, int height_pixels, unsigned *drawn, SDL_Renderer* renderer, SpriteManager* sprMngr);
		void spawnNeighbors(SDL_Rect window);
		void resetCalls();
		Chunk* search(int x, int y);
		Chunk* _search(int x, int y, unsigned uid, std::set<unsigned> *visited);
	private:
		int countLinks();
		void debug();
};

