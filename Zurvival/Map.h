#pragma once
#include "Chunk.h"
#include <math.h>
#include <SDL.h>
#include <set>
#include "SpriteManager.h"
#include "ItemMap.h"
#include "Const.h"
#include "TileManager.h"

/*
	Parent class of map being it a graph formed by chunks 

*/

class Map
{
	Chunk *center; // chunk in the center of the screen
	double centerX; // center position of the screen x coord
	double centerY; // center position of the screen y coord
	int width; // screen width
	int height; // screen height
	std::set<unsigned> exists; // existing chunks
public:
	Map(int w , int h);
	~Map();
	void drawMap(SDL_Renderer* renderer, SpriteManager* sprMngr, TileManager *tm); // draw map
	void setCenter(double, double, ItemMap*); // set center of the map
	SDL_Rect createWindow(); // get the window rect using map coordinates
	void getScreenPosition(double x, double y, int &sx, int &sy); // convert map x,y into screen x,y
};

