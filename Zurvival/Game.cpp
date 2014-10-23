#include "Game.h"


Game::Game(SDL_Renderer* renderer, SpriteManager* sprMngr,int width,int height)
{
	this->renderer = renderer;
	this->sprMngr = sprMngr;
	mc = new MainCharacter(0,0);
	this->width = width;
	this->height = height;
	gmap = new Map(width, height);
}


Game::~Game()
{
}

void Game::update(unsigned delta) {
	// update main
	// update view
	int mx, my;
	SDL_GetMouseState(&mx, &my);
	mc->setView(mx - width / 2, my - height / 2);
	// update position
	mc->update(delta);
	// update map
	gmap->setCenter(mc->getX(), mc->getY());
}

void Game::draw() {
	// draw bg
	gmap->drawMap(renderer, sprMngr);
	// draw main character
	int mainw = height / 15;
	SDL_Rect mainCharacter = { width / 2 - mainw / 2, height / 2 - mainw / 2, height / 15, height / 15 };
	SDL_RenderCopyEx(renderer, sprMngr->getTexture("soldier"), NULL, &mainCharacter, mc->getAngle() - 90, NULL, SDL_FLIP_NONE);
	// draw npc

	// draw zombies

	// draw light
	SDL_Surface *lightmap;
	int lightWidth = width / LIGHT_REDUCTION;
	int lightHeight = height / LIGHT_REDUCTION;
	lightmap = SDL_CreateRGBSurface(0, lightWidth, lightHeight, 32, 0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000);
	Uint32 *pixels = (Uint32 *)lightmap->pixels;
	// player point
	float p1x = lightWidth / 2.0;
	float p1y = lightHeight / 2.0;
	// center of light point (mouse)
	int mx, my;
	SDL_GetMouseState(&mx, &my);
	float ldist = p1y*LIGHT_DISTANCE;
	float ldist2 = ldist*ldist;
	float p2x = (float)mx / LIGHT_REDUCTION;
	float p2y = (float)my / LIGHT_REDUCTION;
	int ldiff = LIGHT_FINAL_ALPHA - LIGHT_BEGIN_ALPHA;
	for (int i = 0; i < lightmap->h; ++i) {
		float dy1 = (p1y - i)*(p1y - i);
		float dy2 = (p2y - i)*(p2y - i);
		for (int j = 0; j < lightmap->w; ++j) {
			float dx2 = (p2x - j)*(p2x - j);
			float dist2 = dy2 + dx2;
			int ratio = LIGHT_FINAL_ALPHA;
			if (dist2 < ldist2) {
				float dx1 = (p1x - j)*(p1x - j);
				float dist1 = dy1 + dx1;
				float dist = dist1 / 8 + dist2;
				if (dist < ldist2) {
					ratio = LIGHT_BEGIN_ALPHA + int(dist / ldist2 * ldiff);
				}
			}
			pixels[(i * lightmap->w) + j] = (ratio<<24) | LIGHT_BASE_COLOR;
		}
	}
	SDL_Texture *lighttex=SDL_CreateTextureFromSurface(renderer, lightmap);
	SDL_SetTextureBlendMode(lighttex, SDL_BLENDMODE_BLEND);
	SDL_RenderCopy(renderer, lighttex, NULL, NULL);
	SDL_FreeSurface(lightmap);
	SDL_DestroyTexture(lighttex);
	// draw gui
}

void Game::listen(bool &end, order_t &order, int &value) {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type){
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym) {
				case SDLK_ESCAPE:
					end = true;
					break;
				case SDLK_BACKSPACE:
					order = ORDER_CHANGE_PAGE;
					value = MAINMENU;
					break;
				case SDLK_UP:
					mc->startMove(MOVE_TOP);
					break;
				case SDLK_RIGHT:
					mc->startMove(MOVE_RIGHT);
					break;
				case SDLK_DOWN:
					mc->startMove(MOVE_BOT);
					break;
				case SDLK_LEFT:
					mc->startMove(MOVE_LEFT);
					break;
				default:
					break;
				}
				break;
			case SDL_KEYUP:
				switch (event.key.keysym.sym) {
				case SDLK_UP:
					mc->stopMove(MOVE_TOP);
					break;
				case SDLK_RIGHT:
					mc->stopMove(MOVE_RIGHT);
					break;
				case SDLK_DOWN:
					mc->stopMove(MOVE_BOT);
					break;
				case SDLK_LEFT:
					mc->stopMove(MOVE_LEFT);
					break;
				default:
					break;
				}
				break;
			case SDL_QUIT:
				end = true;
				break;

			default:
				break;
		}
	}
}