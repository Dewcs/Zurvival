#include "SpriteManager.h"


SpriteManager::SpriteManager() {
}


SpriteManager::~SpriteManager() {
}

void SpriteManager::addImage(SDL_Renderer *renderer, const char *fname, const std::string &key, SDL_Rect r) {
	// check if key exists 
	if (!keyExists(key)) {
		SDL_Surface * image = IMG_Load(fname);
		list[key] = Sprite(SDL_CreateTextureFromSurface(renderer, image),r);
		SDL_FreeSurface(image);
	}
	else {
	}
}

void SpriteManager::addText(SDL_Renderer *renderer, const char *text, const std::string &key, const SDL_Color &color, int ptsize, const char* fontfile, SDL_Rect r) {
	if (!keyExists(key)) {
		TTF_Font* font;
		font = TTF_OpenFont(fontfile, ptsize);
		SDL_Surface *image = TTF_RenderText_Blended(font, text, color);
		list[key] = Sprite(SDL_CreateTextureFromSurface(renderer, image), r);
		SDL_FreeSurface(image);
		TTF_CloseFont(font);
	}
	else {
		std::cerr << "Repeated sprite key" << key << std::endl;
	}
}

SDL_Texture* SpriteManager::getTexture(const std::string &key) {
	return list[key].texture;
}

SDL_Rect SpriteManager::getRect(const std::string &key) {
	return list[key].rect;
}

bool SpriteManager::keyExists(const std::string &key) {
	return list.find(key) == list.end();
}

bool SpriteManager::fileExists(const char *fname) {

}