#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <map>
#include <string>

class SpriteManager {
	private:
		std::map<std::string, SDL_Texture*> list;
	public:
		SpriteManager();
		~SpriteManager();
		void addImage(SDL_Renderer *renderer, const char *fname, const std::string &key, SDL_Rect r);
		void addText(SDL_Renderer *renderer, const char *text, const std::string &key, const SDL_Color &color, int ptsize, const char* fontfile);
	private:
		TTF_Font* loadfont(char* file, int ptsize);
};

