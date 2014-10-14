#include "SpriteManager.h"


SpriteManager::SpriteManager() {
}


SpriteManager::~SpriteManager() {
	for (std::map<std::string, Sprite>::iterator it = list.begin(); it != list.end(); ++it) {
		SDL_DestroyTexture(it->second.texture);
	}
	list.clear();
}

void SpriteManager::addImage(SDL_Renderer *renderer, const std::string &key, const char *fname, SDL_Rect r) {
	// check if key exists 
	if (!keyExists(key)) {
		if (fileExists(fname)) {
			SDL_Surface * image = IMG_Load(fname);
			list[key] = Sprite(SDL_CreateTextureFromSurface(renderer, image), r);
			SDL_FreeSurface(image);
		}
		else {
			std::cerr << "File not found " << fname << std::endl;
		}
	}
	else {
		std::cerr << "Repeated sprite key " << key << std::endl;
	}
}

void SpriteManager::addText(SDL_Renderer *renderer, const std::string &key, const char *text, const SDL_Color &color, int ptsize, const char* fontfile, SDL_Rect r) {
	if (!keyExists(key)) {
		if (fileExists(fontfile)) {
			TTF_Font* font;
			font = TTF_OpenFont(fontfile, ptsize);
			SDL_Surface *image = TTF_RenderText_Blended(font, text, color);
			list[key] = Sprite(SDL_CreateTextureFromSurface(renderer, image), r);
			SDL_FreeSurface(image);
			TTF_CloseFont(font);
		}
		else {
			std::cerr << "File not found " << fontfile << std::endl;
		}
	}
	else {
		std::cerr << "Repeated sprite key " << key << std::endl;
	}
}

SDL_Texture* SpriteManager::getTexture(const std::string &key) {
	return list[key].texture;
}

SDL_Rect SpriteManager::getRect(const std::string &key) {
	return list[key].rect;
}

bool SpriteManager::keyExists(const std::string &key) {
	return !list.empty() && list.find(key) == list.end();
}

bool SpriteManager::fileExists(const char *fname) {
	SDL_RWops* file = SDL_RWFromFile(fname, "r+b");
	bool ret = (file != NULL);
	SDL_RWclose(file);
	return ret;
}