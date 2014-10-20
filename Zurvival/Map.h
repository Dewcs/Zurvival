#pragma once
#include "Chunk.h"
#include <math.h>
#include <SDL.h>
#include "SpriteManager.h"
#include "Const.h"

class Map
{
	Chunk *center;
	double centerX;
	double centerY; 
	int width;
	int height;
	//Chunk *center;
public:
	Map(int w , int h);
	~Map();
	void drawMap(SDL_Renderer* renderer, SpriteManager* sprMngr);
	void setCenter(double, double);
};

