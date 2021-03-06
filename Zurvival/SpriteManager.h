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
	SDL_Texture* texture; // pointer to texture
	SDL_Rect rect; // rect of the sprice can be unset or unused
	int w, h; // width and heigh of the sprite
	double r; // width/height ratio scaling purposes
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

/*
	Class to handle sprites converting them to a string and using sdl_image allowing png, jpg, gif, bmp and also allowing text with ttf fonts and also drawing numbers with prechached fonts
*/

class SpriteManager {
	private:
		std::map<std::string, Sprite> list; // list of sprites
		std::map<std::string, SDL_Texture**> numlist; // list of numbers
		SDL_Renderer *renderer;
	public:
		SpriteManager(SDL_Renderer *renderer);
		~SpriteManager();
		void addImage(const std::string &key, const char *fname, SDL_Rect r); // assing an image to a key
		void addSpriteSheet(const std::string &key, const char *fname, SDL_Rect r, int w, int h, int border, unsigned transp); // assing a sprite sheet to a "key[0-N]"
		void addText(const std::string &key, const char *text, const SDL_Color &color, int ptsize, const char* fontfile, SDL_Rect r); // assing a key to a text
		SDL_Texture* getTexture(const std::string &key); // get a texture from sprite list
		SDL_Rect getRect(const std::string &key); // get a rectangle from a texture from sprite list
		bool isInsideRect(const std::string &key, int x, int y); // check if (x,y) is inside of the rect
		void addNumbers(const std::string &key, const SDL_Color &color, int ptsize, const char* fontfile); // create a number font
		void drawNumber(int number, const std::string &key, int x, int y, int h, align_t align); // draw a number using number font
	private:
		bool keyExists(const std::string &key); // checks if a key already exists
		bool keyNExists(const std::string &key); // checks if a key already exists
		
};

