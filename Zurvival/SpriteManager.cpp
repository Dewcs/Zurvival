#include "SpriteManager.h"


SpriteManager::SpriteManager() {
}


SpriteManager::~SpriteManager() {
	for (std::map<std::string, Sprite>::iterator it = list.begin(); it != list.end(); ++it) {
		SDL_DestroyTexture(it->second.texture);
	}
	list.clear();
	for (std::map<std::string, SDL_Texture**>::iterator it = numlist.begin(); it != numlist.end(); ++it) {
		for (int i = 0; i < 10; ++i) {
			SDL_DestroyTexture(it->second[i]);
		}
		delete[] it->second;
	}
	numlist.clear();
}

void SpriteManager::addImage(SDL_Renderer *renderer, const std::string &key, const char *fname, SDL_Rect r) {
	// check if key exists 
	if (!keyExists(key)) {
		if (fileExists(fname)) {
			SDL_Surface * image = IMG_Load(fname);
			list[key] = Sprite(SDL_CreateTextureFromSurface(renderer, image), r,image->w,image->h);
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

void SpriteManager::addSpriteSheet(SDL_Renderer *renderer, const std::string &key, const char *fname, SDL_Rect r, int w, int h, int border,unsigned transp) {
	if (fileExists(fname)) {
		SDL_Surface * image = IMG_Load(fname);
		int iw = image->w;
		int ih = image->h;
		SDL_Surface * tmp = SDL_CreateRGBSurface(0, w, h, 32, 0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000);
		Uint32 *pixels = (Uint32 *)tmp->pixels;
		Uint32 *origpixels = (Uint32 *)image->pixels;
		int c = 0;
		for (int i = border; i < iw-w; i += w + border) {
			for (int j = border; j < ih-h; j += h + border) {
				for (int x = 0; x < w; ++x) {
					for (int y = 0; y < h; ++y) {
						if ((origpixels[(j + y)*iw + (i + x)] ^ transp) == 0) pixels[x + y*w] = 0;
						else pixels[x + y*w] = origpixels[(j + y)*iw + (i + x)];
					}
				}
				std::string tmpkey = key + std::to_string(c);
				++c;
				if (!keyExists(tmpkey)) {
					list[tmpkey] = Sprite(SDL_CreateTextureFromSurface(renderer, tmp), r, w, h);
				}
				else {
					std::cerr << "Repeated sprite key " << key << std::endl;
				}
			}
		}
		
	}
	else {
		std::cerr << "File not found " << fname << std::endl;
	}
}

void SpriteManager::addText(SDL_Renderer *renderer, const std::string &key, const char *text, const SDL_Color &color, int ptsize, const char* fontfile, SDL_Rect r) {
	if (!keyExists(key)) {
		if (fileExists(fontfile)) {
			TTF_Font* font;
			font = TTF_OpenFont(fontfile, ptsize);
			SDL_Surface *image = TTF_RenderText_Blended(font, text, color);
			list[key] = Sprite(SDL_CreateTextureFromSurface(renderer, image), r, image->w, image->h);
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
	if (!keyExists(key)) std::cerr << "Unknown sprite key " << key << std::endl;
	return list[key].texture;
}

SDL_Rect SpriteManager::getRect(const std::string &key) {
	if (!keyExists(key)) std::cerr << "Unknown sprite key " << key << std::endl;
	return list[key].rect;
}

bool SpriteManager::isInsideRect(const std::string &key,int x, int y) {
	if (!keyExists(key)) std::cerr << "Unknown sprite key " << key << std::endl;
	SDL_Rect r = getRect(key);
	return r.x <= x && x <= r.x + r.w && r.y <= y && y <= r.y + r.h;
}

void SpriteManager::addNumbers(SDL_Renderer *renderer, const std::string &key, const SDL_Color &color, int ptsize, const char* fontfile) {
	if (!keyNExists(key)) {
		if (fileExists(fontfile)) {
			TTF_Font* font;
			font = TTF_OpenFont(fontfile, ptsize);
			SDL_Texture **tmp = new SDL_Texture*[10];
			for (int i = 0; i < 10; ++i) {
				std::string s(1, ('0' + i));
				SDL_Surface *image = TTF_RenderText_Blended(font, s.c_str(), color);
				tmp[i] = SDL_CreateTextureFromSurface(renderer, image);
				SDL_FreeSurface(image);
			}
			TTF_CloseFont(font);
			numlist[key] = tmp;
		}
		else {
			std::cerr << "File not found " << fontfile << std::endl;
		}
	}
	else {
		std::cerr << "Repeated number key " << key << std::endl;
	}
}
/*
	draws number into renderer using numbers defined in key onto (x,y) with h as height and align (ALIGN_RIGHT,ALIGN_LEFT,ALIGN_CENTER)
*/
void SpriteManager::drawNumber(SDL_Renderer *renderer, int number, const std::string &key, int x, int y, int h, align_t align) {
	switch (align) {
		case ALIGN_LEFT:
			if (number < 10) {
				int wi, he;
				SDL_QueryTexture(numlist[key][number], NULL, NULL, &wi, &he);
				int w = (wi*h) / he;
				SDL_Rect r = { x, y, w, h };
				SDL_RenderCopy(renderer, numlist[key][number], NULL, &r);
			}
			else {
				int tnumber = 0;
				while (tnumber > 0) {
					int ac = tnumber % 10;
					tnumber /= 10;
					int wi, he;
					SDL_QueryTexture(numlist[key][ac], NULL, NULL, &wi, &he);
					int w = (wi*h) / he + 1;
					x += w;
				}
				drawNumber(renderer, number, key, x, y, h, ALIGN_RIGHT);
			}
			
			break;
		case ALIGN_RIGHT:
			if (number < 10) {
				int wi, he;
				SDL_QueryTexture(numlist[key][number], NULL, NULL, &wi, &he);
				int w = (wi*h) / he;
				SDL_Rect r = {x - w, y, w, h };
				SDL_RenderCopy(renderer, numlist[key][number], NULL, &r);
			}
			else {
				while (number > 0) {
					int ac = number % 10;
					number /= 10;

					int wi, he;
					SDL_QueryTexture(numlist[key][ac], NULL, NULL, &wi, &he);
					int w = (wi*h) / he;
					SDL_Rect r = { x - w, y, w, h };
					SDL_RenderCopy(renderer, numlist[key][ac], NULL, &r);
					x -= w+1;
				}
			}
			break;
		case ALIGN_CENTER:
			if (number < 10) {
				int wi, he;
				SDL_QueryTexture(numlist[key][number], NULL, NULL, &wi, &he);
				int w = (wi*h) / he;
				SDL_Rect r = { x-w/2, y, w, h };
				SDL_RenderCopy(renderer, numlist[key][number], NULL, &r);
			}
			else {
				int tnumber = 0;
				int tw = 0;
				while (tnumber > 0) {
					int ac = tnumber % 10;
					tnumber /= 10;
					int wi, he;
					SDL_QueryTexture(numlist[key][ac], NULL, NULL, &wi, &he);
					int w = (wi*h) / he + 1;
					tw += w;
				}
				drawNumber(renderer, number, key, x + tw/2, y, h, ALIGN_RIGHT);
			}
			break;
		default:
			break;
	}
}

bool SpriteManager::keyExists(const std::string &key) {
	return list.find(key) != list.end();
}

bool SpriteManager::keyNExists(const std::string &key) {
	return numlist.find(key) != numlist.end();
}