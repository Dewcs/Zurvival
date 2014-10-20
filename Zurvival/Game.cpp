#include "Game.h"


Game::Game(SDL_Renderer* renderer, SpriteManager* sprMngr,int width,int height)
{
	this->renderer = renderer;
	this->sprMngr = sprMngr;
	mc = new MainCharacter();
	this->width = width;
	this->height = height;
}


Game::~Game()
{
}

void Game::update(unsigned delta) {
	// update main
	int mx, my;
	SDL_GetMouseState(&mx, &my);
	mc->setView(mx - width / 2, my - height / 2);
}

void Game::draw() {
	// draw bg
	SDL_RenderCopy(renderer, sprMngr->getTexture("grass"), NULL, NULL);
	// draw main character
	SDL_RenderCopy(renderer, sprMngr->getTexture("soldier"), NULL, NULL);
	// draw npc

	// draw zombies

	// draw light

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