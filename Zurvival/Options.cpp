#include "Options.h"


Options::Options(SDL_Renderer* renderer, SpriteManager* sprMngr)
{
	this->renderer = renderer;
	this->sprMngr = sprMngr;
	log(VERBOSE_DATA_CREATION, "CREATED OPTIONS");
}


Options::~Options()
{
	renderer = NULL;
	sprMngr = NULL;
	log(VERBOSE_DATA_CREATION, "DELETED OPTIONS");
}

void Options::update(unsigned delta) {

}

void Options::draw() {
	SDL_RenderCopy(renderer, sprMngr->getTexture("bg"), NULL, NULL);
	SDL_RenderCopy(renderer, sprMngr->getTexture("back"), NULL, &sprMngr->getRect("back"));
}

void Options::listen(bool &end, bool &pause, order_t &order, int &value) {
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
		case SDL_MOUSEBUTTONDOWN:
			if (event.button.button == SDL_BUTTON_LEFT) {
				if (sprMngr->isInsideRect("back", event.button.x, event.button.y)) {
					order = ORDER_CHANGE_PAGE;
					value = MAINMENU;
					return;
				}
			}
			break;
		case SDL_WINDOWEVENT:
			switch (event.window.event) {
			case SDL_WINDOWEVENT_MINIMIZED:
				pause = true;
				break;
			case SDL_WINDOWEVENT_RESTORED:
				pause = false;
				break;
			default:
				break;
			}
			break;

		default:
			break;
		}
	}
}