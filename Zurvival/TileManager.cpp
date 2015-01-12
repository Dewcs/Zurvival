#include "TileManager.h"

TileManager::TileManager(int size)
{
	tiles = vector<Tile>(size);
	vert = vector<SDL_Texture*>(size*size, NULL);
	hori = vector<SDL_Texture*>(size*size, NULL);
	all = vector<SDL_Texture*>(size*size*size*size, NULL);
}


TileManager::~TileManager()
{
	for (unsigned i = 0; i < tiles.size(); ++i) {
		SDL_FreeSurface(tiles[i].sur);
	}
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

void TileManager::add(SDL_Texture* spr, const char *fname, int id) {
	tiles[id].tex = spr;
	tiles[id].sur = IMG_Load(fname);
	SDL_PixelFormat *fmt = tiles[id].sur->format;
	log(1, "%s %d %d %d %d %d", fname, fmt->BytesPerPixel, fmt->Bmask, fmt->Rmask, fmt->Gmask, fmt->BitsPerPixel);
}

SDL_Texture* TileManager::get(int id) {
	if (id < 0 || id >= tiles.size()) log(1, "ERROR TILES");
	return tiles[id].tex;
}

SDL_Texture* TileManager::get(int id0, int id1, int id2, int id3, SDL_Renderer *renderer) {
	if (id0 == -1 && id2 == -1) {
		return getVert(id1, id3, renderer);
	}
	else if (id1 == -1 && id3 == -1) {
		return getHori(id0, id2, renderer);
	}
	else {
		return getAll(id0, id1, id2, id3, renderer);
	}
}

SDL_Texture* TileManager::getVert(int left, int right, SDL_Renderer *renderer) {
	if (left*size + right < 0 || left*size + right >= vert.size()) log(1, "ERROR VERT");
	if (vert[left*size + right] == NULL) {
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
				if (rand() % 128 >= x) pixels[(y * tmp->w) + x] = 0xff000000 | l[3 * ((y * tmp->w) + x)] | l[3 * ((y * tmp->w) + x) + 1] << 8 | l[3 * ((y * tmp->w) + x) + 2] << 16;
				else pixels[(y * tmp->w) + x] = 0xff000000 | r[3 * ((y * tmp->w) + x)] | r[3 * ((y * tmp->w) + x) + 1] << 8 | r[3 * ((y * tmp->w) + x) + 2] << 16;
			}
		}
		SDL_Texture *tmptex = SDL_CreateTextureFromSurface(renderer, tmp);
		SDL_SetTextureBlendMode(tmptex, SDL_BLENDMODE_BLEND);
		SDL_RenderCopy(renderer, tmptex, NULL, NULL);
		SDL_FreeSurface(tmp);
		vert[left*size + right] = tmptex;
	}
	return vert[left*size + right];
}

SDL_Texture* TileManager::getHori(int top, int bot, SDL_Renderer *renderer) {
	if (top*size + bot < 0 || top*size + bot >= hori.size()) log(1, "ERROR HORI");
	if (hori[top*size + bot] == NULL) {
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
				if (rand() % 128 >= y) pixels[(y * tmp->w) + x] = 0xff000000 | l[3 * ((y * tmp->w) + x)] | l[3 * ((y * tmp->w) + x) + 1] << 8 | l[3 * ((y * tmp->w) + x) + 2] << 16;
				else pixels[(y * tmp->w) + x] = 0xff000000 | r[3 * ((y * tmp->w) + x)] | r[3 * ((y * tmp->w) + x) + 1] << 8 | r[3 * ((y * tmp->w) + x) + 2] << 16;
			}
		}
		SDL_Texture *tmptex = SDL_CreateTextureFromSurface(renderer, tmp);
		SDL_SetTextureBlendMode(tmptex, SDL_BLENDMODE_BLEND);
		SDL_RenderCopy(renderer, tmptex, NULL, NULL);
		SDL_FreeSurface(tmp);
		hori[top*size + bot] = tmptex;
	}
	return hori[top*size + bot];
}

SDL_Texture* TileManager::getAll(int id0, int id1, int id2, int id3, SDL_Renderer *renderer) {
	int id = id0*size*size*size + id1*size*size + id2*size + id3;
	if (id < 0 || id >= all.size()) log(1, "ERROR ALL");
	if (all[id] == NULL) {
		all[id] = tiles[id0].tex;
	}
	return all[id];
}