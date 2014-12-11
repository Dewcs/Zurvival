#include "GameOver.h"


GameOver::GameOver(SDL_Renderer* renderer, SpriteManager* sprMngr){
	this->renderer = renderer;
	this->sprMngr = sprMngr;
	log(VERBOSE_DATA_CREATION, "CREATED GAME OVER");
}


GameOver::~GameOver()
{
	renderer = NULL;
	sprMngr = NULL;
}


void GameOver::listen(bool &end, bool &pause, order_t &order, int &value){
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
				if (sprMngr->isInsideRect("retroceso", event.button.x, event.button.y)) {
					order = ORDER_CHANGE_PAGE;
					value = MAINMENU;
					return;
				}
			}
			break;
		default:
			break;
		}
	}
}

void GameOver::update(unsigned delta){

}

void GameOver::draw(){
	SDL_RenderCopy(renderer, sprMngr->getTexture("bg_gameOver"), NULL, NULL);
	SDL_RenderCopy(renderer, sprMngr->getTexture("retroceso"), NULL, &sprMngr->getRect("retroceso"));
}