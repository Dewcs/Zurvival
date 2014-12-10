#include "GameOver.h"


GameOver::GameOver(SDL_Renderer* renderer, SpriteManager* sprMngr){
}


GameOver::~GameOver()
{
	renderer = NULL;
	sprMngr = NULL;
}


void GameOver::listen(bool &end, bool &pause, order_t &order, int &value){

}

void GameOver::update(unsigned delta){

}

void GameOver::draw(){
	SDL_RenderCopy(renderer, sprMngr->getTexture("bg_gameOver"), NULL, NULL);
}