#include "SpriteManager.h"


SpriteManager::SpriteManager() {
}


SpriteManager::~SpriteManager() {
}

void SpriteManager::addImage(SDL_Renderer *renderer, const char *fname, const std::string &key, SDL_Rect r) {
	if (list.find(key) == list.end()) {
		SDL_Surface * image = IMG_Load(fname);
		list[key] = SDL_CreateTextureFromSurface(renderer, image);
		SDL_FreeSurface(image);
	}
}

void SpriteManager::addText(SDL_Renderer *renderer, const char *text, const std::string &key, const SDL_Color &color, int ptsize, const char* fontfile) {
	if (list.find(key) == list.end()) {
		TTF_Font* font;
		font = TTF_OpenFont(fontfile, ptsize);
		SDL_Surface *image = TTF_RenderText_Blended(font, text, color);
		list[key] = SDL_CreateTextureFromSurface(renderer, image);
		SDL_FreeSurface(image);
		TTF_CloseFont(font);
	}
}