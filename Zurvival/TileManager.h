#pragma once
#include <vector>
#include <SDL_image.h>
#include "Const.h"
#include "Functions.h"
#include <SDL.h>
using namespace std;

struct Tile {
	SDL_Texture *tex;
	SDL_Surface *sur;
};

/*
	Class to handle intersections between tiles merging them
*/

class TileManager
{
private:
	vector<Tile> tiles; // basic tiles forminf the tilemap
	int size;
	vector<vector<SDL_Texture*> > vert; // vertical intersections
	vector<vector<SDL_Texture*> > hori; // horitzontal intersections
	vector<vector<vector<vector<SDL_Texture*> > > > all; // 4 axis intersections
public:
	TileManager(int size);
	~TileManager();
	void add(SDL_Texture* spr,const char *fname, int id); // add a new sprite to basic tiles
	SDL_Texture* get(int id); // get the basic tile with id id
	SDL_Texture* get(int id0, int id1, int id2, int id3, SDL_Renderer *renderer); //get the tile with neigboors id0,1,2,3 - top, right, bot, left
private:
	SDL_Texture* getVert(int left, int right, SDL_Renderer *renderer); // get vertical texture merging left and right
	SDL_Texture* getHori(int top, int bot, SDL_Renderer *renderer); // get horitzontal texture merging top and bot
	SDL_Texture* getAll(int id0, int id1, int id2, int id3, SDL_Renderer *renderer); // get texture formed by 4 sprites
};