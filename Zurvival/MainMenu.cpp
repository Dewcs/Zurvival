#include "MainMenu.h"


MainMenu::MainMenu(SDL_Renderer* renderer, SpriteManager* sprMngr) {
	this->renderer = renderer;
	this->sprMngr = sprMngr;
}


MainMenu::~MainMenu()
{
}

void MainMenu::update(unsigned delta) {
	
}

void MainMenu::draw() {
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
			case SDL_MOUSEBUTTONDOWN:
				if (event.button.button == SDL_BUTTON_LEFT) {
					if (sprMngr->isInsideRect("menu_options", event.button.x, event.button.y)) {
						order = ORDER_CHANGE_PAGE;
						value = OPTIONS;
						return;
					}
				}
				break;
			default:
				break;
		}
	}
}
