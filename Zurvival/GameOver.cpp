#include "GameOver.h"


GameOver::GameOver(SDL_Renderer* renderer, SpriteManager* sprMngr, std::vector<int> *scores, int width, int height){
	this->renderer = renderer;
	this->sprMngr = sprMngr;
	this->scores = scores;
	this->width = width;
	this->height = height;
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
	// draw highscores
	SDL_Rect hsr = sprMngr->getRect("highscores");
	SDL_RenderCopy(renderer, sprMngr->getTexture("highscores"), NULL, &hsr);
	for (int i = 0; i < scores->size(); ++i) {
		sprMngr->drawNumber(i + 1, "red_numbers", width / 3, hsr.y+hsr.h*1.5+i*height/14, height / 15, ALIGN_LEFT);
		sprMngr->drawNumber(scores->at(i), "red_numbers", width*2 / 3, hsr.y + hsr.h * 1.5 + i*height / 14, height / 15, ALIGN_RIGHT);
	}
}