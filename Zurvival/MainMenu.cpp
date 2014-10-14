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
}

void MainMenu::listen(bool &end, int &order, int &value) {
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
