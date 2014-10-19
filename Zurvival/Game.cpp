#include "Game.h"


Game::Game(SDL_Renderer* renderer, SpriteManager* sprMngr)
{
	this->renderer = renderer;
	this->sprMngr = sprMngr;
}


Game::~Game()
{
}

void Game::update(unsigned delta) {
	// update main

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
				default:
					break;
				}
			case SDL_KEYUP:
				break;
			case SDL_QUIT:
				end = true;
				break;

			default:
				break;
		}
	}
}