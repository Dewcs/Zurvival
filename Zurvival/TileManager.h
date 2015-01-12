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

class TileManager
{
private:
	vector<Tile> tiles;
	int size;
	vector<vector<SDL_Texture*> > vert;
	vector<vector<SDL_Texture*> > hori;
	vector<vector<vector<vector<SDL_Texture*> > > > all;
public:
	TileManager(int size);
	~TileManager();
	void add(SDL_Texture* spr,const char *fname, int id);
	SDL_Texture* get(int id);
	SDL_Texture* get(int id0, int id1, int id2, int id3, SDL_Renderer *renderer);
private:
	SDL_Texture* getVert(int left, int right, SDL_Renderer *renderer);
	SDL_Texture* getHori(int top, int bot, SDL_Renderer *renderer);
	SDL_Texture* getAll(int id0, int id1, int id2, int id3, SDL_Renderer *renderer);
};