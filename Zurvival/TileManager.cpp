#include "TileManager.h"

TileManager::TileManager(int size)
{
	tiles = vector<Tile>(size);
	vert = vector<vector<SDL_Texture*> >(size, vector<SDL_Texture*>(size, NULL));
	hori = vector<vector<SDL_Texture*> >(size, vector<SDL_Texture*>(size, NULL));
	all = vector<vector<vector<vector<SDL_Texture*> > > > (size, vector<vector<vector<SDL_Texture*> > >(size, vector<vector<SDL_Texture*> > (size,vector<SDL_Texture*> (size, NULL))));
}


TileManager::~TileManager()
{
	for (unsigned i = 0; i < tiles.size(); ++i) {
		SDL_FreeSurface(tiles[i].sur);
	}
	tiles.clear();
	for (unsigned i = 0; i < vert.size(); ++i) {
		for (unsigned j = 0; j < vert[i].size(); ++j) {
			if (vert[i][j] != NULL) SDL_DestroyTexture(vert[i][j]);
		}
	}
	vert.clear();
	for (unsigned i = 0; i < hori.size(); ++i) {
		for (unsigned j = 0; j < hori[i].size(); ++j) {
			if (hori[i][j] != NULL) SDL_DestroyTexture(hori[i][j]);
		}
	}
	hori.clear();
	for (unsigned i = 0; i < all.size(); ++i) {
		for (unsigned j = 0; j < all[i].size(); ++j) {
			for (unsigned k = 0; k < all[i][j].size(); ++k) {
				for (unsigned l = 0; l < all[i][j][k].size(); ++l) {
					if (all[i][j][k][l] != NULL) SDL_DestroyTexture(all[i][j][k][l]);
				}
			}
		}
	}
	all.clear();
}

void TileManager::add(SDL_Texture* spr, const char *fname, int id) {
	tiles[id].tex = spr;
	tiles[id].sur = IMG_Load(fname);
}

SDL_Texture* TileManager::get(int id) {
	if (id < 0 || id >= tiles.size()) log(1, "ERROR TILES");
	return tiles[id].tex;
}

SDL_Texture* TileManager::get(int id0, int id1, int id2, int id3, SDL_Renderer *renderer) {
	if (id0 == TILE_MAX && id2 == TILE_MAX) {
		return getVert(id3, id1, renderer);
	}
	else if (id1 == TILE_MAX && id3 == TILE_MAX) {
		return getHori(id0, id2, renderer);
	}
	else {
		return getAll(id0, id1, id2, id3, renderer);
	}
}

SDL_Texture* TileManager::getVert(int left, int right, SDL_Renderer *renderer) {
	if (left < 0 || left >= vert.size() || right < 0 || right >= vert.size()) log(1, "ERROR VERT %d %d",left,right);
	if (left == right) return tiles[left].tex; // same texture
	if (vert[left][right] == NULL) {
		// merged texture
		SDL_Surface *tmp;
		Uint8 *l, *r;
		l = (Uint8 *)tiles[left].sur->pixels;
		r = (Uint8 *)tiles[right].sur->pixels;
		int width = 128;
		int height = 128;
		tmp = SDL_CreateRGBSurface(0, width, height, 32, 0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000);
		Uint32 *pixels = (Uint32 *)tmp->pixels;
		for (int x = 0; x < width; ++x) {
			for (int y = 0; y < height; ++y) {
				// use random with chances equaling the cube of the distance
				double rnd = randomReal(0, 1);
				double p1, p2;
				p1 = 1.0 / ((128 - x)*(128 - x)*(128 - x));
				p2 = 1.0 / ((x + 1)*(x + 1)*(x + 1));
				if (p1 / (p1 + p2) <= rnd) pixels[(y * tmp->w) + x] = 0xff000000 | l[3 * ((y * tmp->w) + x)] | l[3 * ((y * tmp->w) + x) + 1] << 8 | l[3 * ((y * tmp->w) + x) + 2] << 16;
				else pixels[(y * tmp->w) + x] = 0xff000000 | r[3 * ((y * tmp->w) + x)] | r[3 * ((y * tmp->w) + x) + 1] << 8 | r[3 * ((y * tmp->w) + x) + 2] << 16;
			}
		}
		SDL_Texture *tmptex = SDL_CreateTextureFromSurface(renderer, tmp);
		SDL_FreeSurface(tmp);
		vert[left][right] = tmptex;
	}
	return vert[left][right];
}

SDL_Texture* TileManager::getHori(int top, int bot, SDL_Renderer *renderer) {
	if (top < 0 || top >= vert.size() || bot < 0 || bot >= vert.size()) log(1, "ERROR HORI %d %d",top,bot);
	if (top == bot) return tiles[top].tex;
	if (hori[top][bot] == NULL) {
		SDL_Surface *tmp;
		Uint8 *l, *r;
		l = (Uint8 *)tiles[top].sur->pixels;
		r = (Uint8 *)tiles[bot].sur->pixels;
		int width = 128;
		int height = 128;
		tmp = SDL_CreateRGBSurface(0, width, height, 32, 0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000);
		Uint32 *pixels = (Uint32 *)tmp->pixels;
		for (int x = 0; x < width; ++x) {
			for (int y = 0; y < height; ++y) {
				// use random with chances equaling the cube of the distance
				double rnd = randomReal(0, 1);
				double p1, p2;
				p1 = 1.0 / ((128 - y)*(128 - y)*(128 - y));
				p2 = 1.0 / ((y + 1)*(y + 1)*(y + 1));
				if (p1/(p1+p2)<=rnd) pixels[(y * tmp->w) + x] = 0xff000000 | l[3 * ((y * tmp->w) + x)] | l[3 * ((y * tmp->w) + x) + 1] << 8 | l[3 * ((y * tmp->w) + x) + 2] << 16;
				else pixels[(y * tmp->w) + x] = 0xff000000 | r[3 * ((y * tmp->w) + x)] | r[3 * ((y * tmp->w) + x) + 1] << 8 | r[3 * ((y * tmp->w) + x) + 2] << 16;
			}
		}
		SDL_Texture *tmptex = SDL_CreateTextureFromSurface(renderer, tmp);
		SDL_FreeSurface(tmp);
		hori[top][bot] = tmptex;
	}
	return hori[top][bot];
}

SDL_Texture* TileManager::getAll(int id0, int id1, int id2, int id3, SDL_Renderer *renderer) {
	if (id0 < 0 || id0 >= vert.size() || id1 < 0 || id1 >= vert.size() || id2 < 0 || id2 >= vert.size() || id3 < 0 || id3 >= vert.size()) log(1, "ERROR HORI %d %d %d %d", id0,id1,id2,id3);
	if (id0==id1 && id1==id2 && id2==id3) return tiles[id0].tex;
	if (all[id0][id1][id2][id3] == NULL) {
		SDL_Surface *tmp;
		Uint8 *l, *r, *t, *b;
		t = (Uint8 *)tiles[id0].sur->pixels;
		r = (Uint8 *)tiles[id1].sur->pixels;
		b = (Uint8 *)tiles[id2].sur->pixels;
		l = (Uint8 *)tiles[id3].sur->pixels;
		int width = 128;
		int height = 128;
		tmp = SDL_CreateRGBSurface(0, width, height, 32, 0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000);
		Uint32 *pixels = (Uint32 *)tmp->pixels;
		for (int x = 0; x < width; ++x) {
			for (int y = 0; y < height; ++y) {
				// use random with chances equaling the cube of the distance
				double rnd = randomReal(0, 1);
				double p1, p2;
				p1 = 1.0 / ((128 - y)*(128 - y)*(128 - y));
				p2 = 1.0 / ((y + 1)*(y + 1)*(y + 1));
				if (p1 / (p1 + p2) <= rnd) {
					p1 = 1.0 / ((128 - x)*(128 - x)*(128 - x));
					p2 = 1.0 / ((x + 1)*(x + 1)*(x + 1));
					if (p1 / (p1 + p2) <= rnd) {
						pixels[(y * tmp->w) + x] = 0xff000000 | t[3 * ((y * tmp->w) + x)] | t[3 * ((y * tmp->w) + x) + 1] << 8 | t[3 * ((y * tmp->w) + x) + 2] << 16;
					}
					else {
						pixels[(y * tmp->w) + x] = 0xff000000 | r[3 * ((y * tmp->w) + x)] | r[3 * ((y * tmp->w) + x) + 1] << 8 | r[3 * ((y * tmp->w) + x) + 2] << 16;
					}
				}
				else {
					p1 = 1.0 / ((128 - x)*(128 - x)*(128 - x));
					p2 = 1.0 / ((x + 1)*(x + 1)*(x + 1));
					if (p1 / (p1 + p2) <= rnd) {
						pixels[(y * tmp->w) + x] = 0xff000000 | l[3 * ((y * tmp->w) + x)] | l[3 * ((y * tmp->w) + x) + 1] << 8 | l[3 * ((y * tmp->w) + x) + 2] << 16;
					}
					else {
						pixels[(y * tmp->w) + x] = 0xff000000 | b[3 * ((y * tmp->w) + x)] | b[3 * ((y * tmp->w) + x) + 1] << 8 | b[3 * ((y * tmp->w) + x) + 2] << 16;
					}
				}
			}
		}
		SDL_Texture *tmptex = SDL_CreateTextureFromSurface(renderer, tmp);
		SDL_FreeSurface(tmp);
		all[id0][id1][id2][id3] = tmptex;
	}
	return all[id0][id1][id2][id3];
}