#pragma once
#include <vector>
#include "Const.h"
#include "Functions.h"
#include <SDL.h>
using namespace std;
class TileManager
{
private:
	vector<SDL_Texture*> tiles;
	int size;
	vector<SDL_Texture*> vert;
	vector<SDL_Texture*> hori;
	vector<SDL_Texture*> all;
public:
	TileManager(int size);
	~TileManager();
	void add(SDL_Texture* spr, int id);
	SDL_Texture* get(int id);
	SDL_Texture* get(int id0, int id1, int id2, int id3);
private:
	SDL_Texture* getVert(int left, int right);
	SDL_Texture* getHori(int top, int bot);
	SDL_Texture* getAll(int id0, int id1, int id2, int id3);
};