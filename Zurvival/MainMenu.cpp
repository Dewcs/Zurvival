#include "MainMenu.h"


MainMenu::MainMenu(SDL_Renderer* renderer, SpriteManager* sprMngr) {
	this->renderer = renderer;
	this->sprMngr = sprMngr;
	log(VERBOSE_DATA_CREATION, "CREATED MAINMENU");
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

void MainMenu::listen(bool &end, bool &pause, order_t &order, int &value) {
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
					else if (sprMngr->isInsideRect("menu_deathPit", event.button.x, event.button.y)) {
						order = ORDER_CHANGE_PAGE;
						value = DEATHPIT;
						return;
					}
					else if (sprMngr->isInsideRect("menu_play", event.button.x, event.button.y)) {
						order = ORDER_CHANGE_PAGE;
						value = GAME;
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
				/*switch (event.window.event) {
				case SDL_WINDOWEVENT_SHOWN:
					SDL_Log("Window %d shown", event.window.windowID);
					break;
				case SDL_WINDOWEVENT_HIDDEN:
					SDL_Log("Window %d hidden", event.window.windowID);
					break;
				case SDL_WINDOWEVENT_EXPOSED:
					SDL_Log("Window %d exposed", event.window.windowID);
					break;
				case SDL_WINDOWEVENT_MOVED:
					SDL_Log("Window %d moved to %d,%d",
						event.window.windowID, event.window.data1,
						event.window.data2);
					break;
				case SDL_WINDOWEVENT_RESIZED:
					SDL_Log("Window %d resized to %dx%d",
						event.window.windowID, event.window.data1,
						event.window.data2);
					break;
				case SDL_WINDOWEVENT_MINIMIZED:
					pause = true;
					SDL_Log("Window %d minimized", event.window.windowID);
					break;
				case SDL_WINDOWEVENT_MAXIMIZED:
					SDL_Log("Window %d maximized", event.window.windowID);
					break;
				case SDL_WINDOWEVENT_RESTORED:
					pause = false;
					SDL_Log("Window %d restored", event.window.windowID);
					break;
				case SDL_WINDOWEVENT_ENTER:
					SDL_Log("Mouse entered window %d",
						event.window.windowID);
					break;
				case SDL_WINDOWEVENT_LEAVE:
					SDL_Log("Mouse left window %d", event.window.windowID);
					break;
				case SDL_WINDOWEVENT_FOCUS_GAINED:
					SDL_Log("Window %d gained keyboard focus",
						event.window.windowID);
					break;
				case SDL_WINDOWEVENT_FOCUS_LOST:
					SDL_Log("Window %d lost keyboard focus",
						event.window.windowID);
					break;
				case SDL_WINDOWEVENT_CLOSE:
					SDL_Log("Window %d closed", event.window.windowID);
					break;
				default:
					SDL_Log("Window %d got unknown event %d",
						event.window.windowID, event.window.event);
					break;
				}
				break;*/
			default:
				break;
		}
	}
}
