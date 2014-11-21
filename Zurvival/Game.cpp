#include "Game.h"


Game::Game(SDL_Renderer* renderer, SpriteManager* sprMngr,int width,int height)
{
	this->renderer = renderer;
	this->sprMngr = sprMngr;
	mc = new MainCharacter(0,0);
	this->width = width;
	this->height = height;
	gmap = new Map(width, height);
	zombies = new Zombie*[ZOMBIE_AMOUNT];
	zcount = 0;
	sounds = new Radar(453, 0.5, 100);
	smells = new Radar(5, 0, 1000);

	log(VERBOSE_DATA_CREATION, "CREATED GAME");
}


Game::~Game()
{
	delete mc;
	delete gmap;
	delete [] zombies;
	delete smells;
	delete sounds;

	renderer = NULL;
	sprMngr = NULL;
}

void Game::update(unsigned delta) {
	// update main
	// update view
	int mx, my;
	SDL_GetMouseState(&mx, &my);
	mc->setView(mx - width / 2, my - height / 2);
	// update position
	mc->update(delta);
	// update map
	gmap->setCenter(mc->getX(), mc->getY());
	// update smell map
	smells->addPoint(mc->getX(), mc->getY(), 1, SDL_GetTicks());
	// update sounds map
	sounds->addPoint(mc->getX(), mc->getY(), 30, SDL_GetTicks());
	smells->update(delta);
	sounds->update(delta);
	// update zombies
	// create new zombies??
	if (zcount < ZOMBIE_AMOUNT && rand()%100==0) {
		int x = 0;
		int y = 1;
		zombies[zcount] = new Zombie(x, y, SDL_GetTicks(),"random");
		++zcount;
	}
	// update zombies
	for (int i = 0; i < zcount; ++i) {
		zombies[i]->prepare(mc->getX(), mc->getY(), smells, sounds);
		zombies[i]->update(delta);
	}
	// delete zombies
	for (int i = zcount - 1; i >= 0; --i) {
		if (zombies[i]->isDead()) {
			delete zombies[i];
			for (int j = i + 1; j < zcount; ++j) {
				zombies[j - 1] = zombies[j];
			}
			--zcount;
		}
	}
}

void Game::draw() {
	// draw bg
	gmap->drawMap(renderer, sprMngr);
	// draw main character
	int mainw = height / 15;
	SDL_Rect mainCharacter = { width / 2 - mainw / 2, height / 2 - mainw / 2, height / 15, height / 15 };
	SDL_RenderCopyEx(renderer, sprMngr->getTexture("soldier"), NULL, &mainCharacter, mc->getAngle() - 90, NULL, SDL_FLIP_NONE);
	// draw npc

	// draw zombies
	int zwidth = height / 15;
	SDL_Rect screen = { 0, 0, width, height };
	for (int i = 0; i < zcount; ++i) {
		int x, y;
		gmap->getScreenPosition(zombies[i]->getX(), zombies[i]->getY(), x, y);
		SDL_Rect zombieRect = { x - zwidth / 2, y - zwidth / 2, zwidth, zwidth };
		if (rectInsideRect(screen, zombieRect)) SDL_RenderCopyEx(renderer, sprMngr->getTexture("zombie_anim" + std::to_string((SDL_GetTicks()/100)%8)), NULL, &zombieRect, zombies[i]->getAngle() - 90, NULL, SDL_FLIP_NONE);
	}
	// draw light
	SDL_Surface *lightmap;
	int lightWidth = width / LIGHT_REDUCTION;
	int lightHeight = height / LIGHT_REDUCTION;
	lightmap = SDL_CreateRGBSurface(0, lightWidth, lightHeight, 32, 0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000);
	Uint32 *pixels = (Uint32 *)lightmap->pixels;
	// player point
	float p1x = (float)lightWidth / 2;
	float p1y = (float)lightHeight / 2;
	// center of light point (mouse)
	int mx, my;
	SDL_GetMouseState(&mx, &my);
	float ldist = p1y*(float)LIGHT_DISTANCE;
	float ldist2 = ldist*ldist;
	float p2x = (float)mx / LIGHT_REDUCTION;
	float p2y = (float)my / LIGHT_REDUCTION;
	int ldiff = LIGHT_FINAL_ALPHA - LIGHT_BEGIN_ALPHA;
	for (int i = 0; i < lightmap->h; ++i) {
		// precalc y diffs
		float dy1 = (p1y - i)*(p1y - i);
		float dy2 = (p2y - i)*(p2y - i);
		for (int j = 0; j < lightmap->w; ++j) {
			float dx2 = (p2x - j)*(p2x - j);
			float dist2 = dy2 + dx2;
			int ratio = LIGHT_FINAL_ALPHA;
			// if can be close
			if (dist2 < ldist2) {
				float dx1 = (p1x - j)*(p1x - j);
				float dist1 = dy1 + dx1;
				// we want a cone so lets give less power to the origin and more to the target
				float dist = dist1 / 8 + dist2;
				// if is still close
				if (dist < ldist2) {
					ratio = LIGHT_BEGIN_ALPHA + int(dist / ldist2 * ldiff);
				}
			}
			// add the pixel
			pixels[(i * lightmap->w) + j] = (ratio<<24) | LIGHT_BASE_COLOR;
		}
	}
	SDL_Texture *lighttex=SDL_CreateTextureFromSurface(renderer, lightmap);
	SDL_SetTextureBlendMode(lighttex, SDL_BLENDMODE_BLEND);
	SDL_RenderCopy(renderer, lighttex, NULL, NULL);
	SDL_FreeSurface(lightmap);
	SDL_DestroyTexture(lighttex);
	// draw gui
	drawGUI();
}

void Game::listen(bool &end, bool &pause, order_t &order, int &value) {
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
				case SDLK_UP:
					mc->startMove(MOVE_TOP);
					break;
				case SDLK_RIGHT:
					mc->startMove(MOVE_RIGHT);
					break;
				case SDLK_DOWN:
					mc->startMove(MOVE_BOT);
					break;
				case SDLK_LEFT:
					mc->startMove(MOVE_LEFT);
					break;
				case SDLK_w:
					mc->startMove(MOVE_TOP);
					break;
				case SDLK_d:
					mc->startMove(MOVE_RIGHT);
					break;
				case SDLK_s:
					mc->startMove(MOVE_BOT);
					break;
				case SDLK_a:
					mc->startMove(MOVE_LEFT);
					break;
				default:
					break;
				}
				break;
			case SDL_KEYUP:
				switch (event.key.keysym.sym) {
				case SDLK_UP:
					mc->stopMove(MOVE_TOP);
					break;
				case SDLK_RIGHT:
					mc->stopMove(MOVE_RIGHT);
					break;
				case SDLK_DOWN:
					mc->stopMove(MOVE_BOT);
					break;
				case SDLK_LEFT:
					mc->stopMove(MOVE_LEFT);
					break;
				case SDLK_w:
					mc->stopMove(MOVE_TOP);
					break;
				case SDLK_d:
					mc->stopMove(MOVE_RIGHT);
					break;
				case SDLK_s:
					mc->stopMove(MOVE_BOT);
					break;
				case SDLK_a:
					mc->stopMove(MOVE_LEFT);
					break;
				default:
					break;
				}
				break;
			case SDL_QUIT:
				end = true;
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

void Game::drawGUI(){
//draw slots 
	int margi = width / 30;
	int slotRect = height / 12;
	SDL_Rect pistolSlot = {margi, margi, slotRect, slotRect};
	SDL_Rect shotgunSlot = { margi, margi * 2 + slotRect, slotRect, slotRect };
	SDL_Rect hevySlot = { margi, margi * 3 + slotRect*2, slotRect, slotRect };
	SDL_Rect healSlot = { margi, margi * 4 + slotRect * 3, slotRect, slotRect };
	SDL_RenderCopy(renderer, sprMngr->getTexture("pistolSlot"), NULL, &pistolSlot);
	SDL_RenderCopy(renderer, sprMngr->getTexture("shotgunSlot"), NULL, &shotgunSlot);
	SDL_RenderCopy(renderer, sprMngr->getTexture("hevySlot"), NULL, &hevySlot);
	SDL_RenderCopy(renderer, sprMngr->getTexture("healSlot"), NULL, &healSlot);
}