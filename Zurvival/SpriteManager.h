#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <map>
#include <string>
#include<iostream>

struct Sprite {
	SDL_Texture* texture;
	SDL_Rect rect;
	// create a new sprite with information
	Sprite(SDL_Texture* texture, SDL_Rect rect) {
		this->texture = texture;
		this->rect = rect;
	}
	// fix for map
	Sprite() {
		texture = NULL;
		rect = {};
	}
};

class SpriteManager {
	private:
		std::map<std::string, Sprite> list;
	public:
		SpriteManager();
		~SpriteManager();
		void addImage(SDL_Renderer *renderer, const char *fname, const std::string &key, SDL_Rect r);
		void addText(SDL_Renderer *renderer, const char *text, const std::string &key, const SDL_Color &color, int ptsize, const char* fontfile, SDL_Rect r);
		SDL_Texture* getTexture(const std::string &key);
		SDL_Rect getRect(const std::string &key);
	private:
		bool keyExists(const std::string &key);
		bool fileExists(const char *fname)
		
};

