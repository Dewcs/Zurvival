#include "DeathPit.h"


DeathPit::DeathPit(SDL_Renderer* renderer, SpriteManager* sprMngr, int width, int height)
{
	this->renderer = renderer;
	this->sprMngr = sprMngr;
	this->width = width;
	this->height = height;
	zombies = new Zombie*[DP_ZOMBIE_AMOUNT];
	zcount = 0;

	humans = new humanDummy*[DP_HUMAN_AMOUNT];
	hcount = 0;
	sounds = new Radar(453, 0.5, 750);
	smells = new Radar(5, 0, 5000);
}


DeathPit::~DeathPit()
{
	
}

void DeathPit::update(unsigned delta) {
	// update humans
	// spawn
	if (hcount < DP_HUMAN_AMOUNT && rand() % 20 == 0) {
		int x = rand()%(width/DP_RATIO);
		int y = rand() % (height / DP_RATIO);
		humans[hcount] = new humanDummy(x, y);
		++hcount;
	}
	// update
	for (int i = 0; i < hcount; ++i) {
		// update position
		humans[i]->update(delta);
		// update smell map
		if (rand()%10==1) smells->addPoint(humans[i]->getX(), humans[i]->getY(), 1, SDL_GetTicks());
		// update sounds map
		sounds->addPoint(humans[i]->getX(), humans[i]->getY(), rand()%100+30, SDL_GetTicks());
	}
	// remove out of screen
	for (int i = hcount - 1; i >= 0; --i) {
		if (humans[i]->isDead() || !pointInsideRect(humans[i]->getX(), humans[i]->getY(), 0, 0, width / DP_RATIO, height / DP_RATIO)) {
			delete humans[i];
			for (int j = i + 1; j < hcount; ++j) {
				humans[j - 1] = humans[j];
			}
			--hcount;
		}
	}
	// update radars
	smells->update(delta);
	sounds->update(delta);

	// update zombies
	// create new zombies??
	if (zcount < DP_ZOMBIE_AMOUNT/2 && rand() % 10 == 0) {
		int x = (width / DP_RATIO) / 2 - 10;
		int y = (height / DP_RATIO) / 2 - 10;
		zombies[zcount] = new Zombie(x, y);
		++zcount;
	}
	// update zombies
	for (int i = 0; i < zcount; ++i) {
		zombies[i]->update(delta, smells, sounds);
	}
	// delete zombies
	for (int i = zcount - 1; i >= 0; --i) {
		if (zombies[i]->isDead() || !pointInsideRect(zombies[i]->getX(), zombies[i]->getY(), 0, 0, width / DP_RATIO, height / DP_RATIO)) {
			delete zombies[i];
			for (int j = i + 1; j < zcount; ++j) {
				zombies[j - 1] = zombies[j];
			}
			--zcount;
		}
	}
	for (int i = 0; i < hcount; ++i) {
		if (!humans[i]->isDead()) {
			double hx = humans[i]->getX();
			double hy = humans[i]->getY();
			for (int j = 0; j < zcount; ++j) {
				double zx = zombies[j]->getX();
				double zy = zombies[j]->getY();
				if (abs(hx - zx) < 3 && abs(hy - zy) < 3 && distP2P(hx, hy, zx, zy) <= 1.5) {
					zombies[j]->addKills(1);
					SDL_Log("KILLED HUMAN BY ZC: %d", zombies[j]->getKills());
					zombies[zcount] = new Zombie(hx, hy);
					zombies[zcount]->addKills(zombies[j]->getKills());
					++zcount;
					humans[i]->kill();
					break;
				}
			}
		}
	}
}

void DeathPit::draw() {
	for (int i = 0; i < zcount; ++i) {
		SDL_Rect zombieRect = { round(zombies[i]->getX()*DP_RATIO), round(zombies[i]->getY()*DP_RATIO), DP_RATIO + zombies[i]->getKills(), DP_RATIO + zombies[i]->getKills() };
		SDL_RenderCopy(renderer, sprMngr->getTexture("pgreen"), NULL, &zombieRect);
	}
	for (int i = 0; i < hcount; ++i) {
		SDL_Rect zombieRect = { round(humans[i]->getX()*DP_RATIO), round(humans[i]->getY()*DP_RATIO), DP_RATIO, DP_RATIO };
		SDL_RenderCopy(renderer, sprMngr->getTexture("pred"), NULL, &zombieRect);
	}
}

void DeathPit::listen(bool &end, order_t &order, int &value) {
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

		default:
			break;
		}
	}
}