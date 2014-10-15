#include "MainMenu.h"


MainMenu::MainMenu()
{
}


MainMenu::~MainMenu()
{
}

void MainMenu::update(unsigned delta) {
	
}

void MainMenu::draw(SDL_Renderer* renderer, SpriteManager* sprMngr) {
	SDL_RenderCopy(renderer, sprMngr->getTexture("bg"), NULL, NULL);
	SDL_RenderCopy(renderer, sprMngr->getTexture("menu_play"), NULL, &sprMngr->getRect("menu_play"));
	SDL_RenderCopy(renderer, sprMngr->getTexture("menu_deathPit"), NULL, &sprMngr->getRect("menu_deathPit"));
	SDL_RenderCopy(renderer, sprMngr->getTexture("menu_options"), NULL, &sprMngr->getRect("menu_options"));
	SDL_RenderCopyEx(renderer, sprMngr->getTexture("menu_title"), NULL, &sprMngr->getRect("menu_title"), -15, NULL, SDL_FLIP_NONE);
}

void MainMenu::listen(bool &end, order_t &order, int &value) {
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
