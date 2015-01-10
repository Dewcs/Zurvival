#pragma once
#include "Chunk.h"
#include <math.h>
#include <SDL.h>
#include <set>
#include "SpriteManager.h"
#include "ItemMap.h"
#include "Const.h"
#include "TileManager.h"

class Map
{
	Chunk *center;
	double centerX;
	double centerY; 
	int width;
	int height;
	std::set<unsigned> exists;
public:
	Map(int w , int h);
	~Map();
	void drawMap(SDL_Renderer* renderer, SpriteManager* sprMngr, TileManager *tm);
	void setCenter(double, double, ItemMap*);
	SDL_Rect createWindow();
	void getScreenPosition(double x, double y, int &sx, int &sy);
};

