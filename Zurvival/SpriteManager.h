#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <map>
#include <string>
#include <iostream>
#include "Functions.h"
#include "Const.h"

struct Sprite {
	SDL_Texture* texture;
	SDL_Rect rect;
	int w, h;
	double r;
	// create a new sprite with information
	Sprite(SDL_Texture* texture, SDL_Rect rect,int w,int h) {
		this->texture = texture;
		this->rect = rect;
		this->w = w;
		this->h = h;
		this->r = w / double(h);
	}
	// fix for map
	Sprite() {
		texture = NULL;
		rect = {};
		w = -1;
		h = -1;
		r = 0;
	}
};

class SpriteManager {
	private:
		std::map<std::string, Sprite> list;
		std::map<std::string, SDL_Texture**> numlist;
	public:
		SpriteManager();
		~SpriteManager();
		void addImage(SDL_Renderer *renderer, const std::string &key, const char *fname, SDL_Rect r);
		void addText(SDL_Renderer *renderer, const std::string &key, const char *text, const SDL_Color &color, int ptsize, const char* fontfile, SDL_Rect r);
		SDL_Texture* getTexture(const std::string &key);
		SDL_Rect getRect(const std::string &key);
		bool isInsideRect(const std::string &key, int x, int y);
		void addNumbers(SDL_Renderer *renderer, const std::string &key, const SDL_Color &color, int ptsize, const char* fontfile);
		void drawNumber(SDL_Renderer *renderer,int number, const std::string &key, int x, int y, int h, align_t align);
	private:
		bool keyExists(const std::string &key);
		bool keyNExists(const std::string &key);
		
};

