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
	float p1x = lightWidth / 2.0;
	float p1y = lightHeight / 2.0;
	float mindist = p1y * LIGHT_VIEW_MIN_RANGE;
	float maxdist = p1y * LIGHT_VIEW_MAX_RANGE;
	double angle = mc->getRawAngle();
	double angle0 = angle - LIGHT_VIEW_ANGLE / 2;
	double angle1 = angle + LIGHT_VIEW_ANGLE / 2;
	for (int i = 0; i < lightmap->h; ++i) {
		for (int j = 0; j < lightmap->w; ++j) {
			double tmpangle = angleP2P(p1x, p1y, float(j), float(i));
			float dist = distP2P(p1x, p1y, j, i);
			int ratio = mapf2i(dist, mindist, maxdist, LIGHT_BEGIN_ALPHA, LIGHT_FINAL_ALPHA);
			if (!inAngleRange(tmpangle, angle0, angle1)) ratio = LIGHT_FINAL_ALPHA;
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