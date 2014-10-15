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

}

void Game::draw() {
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