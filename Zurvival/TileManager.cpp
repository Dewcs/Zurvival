#include "TileManager.h"


TileManager::TileManager(int size)
{
	tiles = vector<SDL_Texture*>(size);
	vert = vector<SDL_Texture*>(size*size, NULL);
	hori = vector<SDL_Texture*>(size*size, NULL);
	all = vector<SDL_Texture*>(size*size*size*size, NULL);
}


TileManager::~TileManager()
{
	tiles.clear();
	for (unsigned i = 0; i < vert.size(); ++i) {
		if (vert[i]!=NULL) SDL_DestroyTexture(vert[i]);
	}
	vert.clear();
	for (unsigned i = 0; i < hori.size(); ++i) {
		if (hori[i] != NULL) SDL_DestroyTexture(hori[i]);
	}
	hori.clear();
	for (unsigned i = 0; i < all.size(); ++i) {
		if (all[i] != NULL) SDL_DestroyTexture(all[i]);
	}
	all.clear();
}

void TileManager::add(SDL_Texture* spr, int id) {
	tiles[id] = spr;
}

SDL_Texture* TileManager::get(int id) {
	if (id < 0 || id >= tiles.size()) log(1, "ERROR TILES");
	return tiles[id];
}

SDL_Texture* TileManager::get(int id0, int id1, int id2, int id3) {
	if (id0 == -1 && id2 == -1) {
		return getVert(id1, id3);
	}
	else if (id1 == -1 && id3 == -1) {
		return getHori(id0, id2);
	}
	else {
		return getAll(id0, id1, id2, id3);
	}
}

SDL_Texture* TileManager::getVert(int left, int right) {
	if (left*size + right < 0 || left*size + right >= vert.size()) log(1, "ERROR VERT");
	if (vert[left*size + right] == NULL) {
		vert[left*size + right] = tiles[left];
	}
	return vert[left*size + right];
}

SDL_Texture* TileManager::getHori(int top, int bot) {
	if (top*size + bot < 0 || top*size + bot >= hori.size()) log(1, "ERROR HORI");
	if (hori[top*size + bot] == NULL) {
		hori[top*size + bot] = tiles[top];
	}
	return hori[top*size + bot];
}

SDL_Texture* TileManager::getAll(int id0, int id1, int id2, int id3) {
	int id = id0*size*size*size + id1*size*size + id2*size + id3;
	if (id < 0 || id >= all.size()) log(1, "ERROR ALL");
	if (all[id] == NULL) {
		all[id] = tiles[id0];
	}
	return all[id];
}