#include "DeathPit.h"


DeathPit::DeathPit(SDL_Renderer* renderer, SpriteManager* sprMngr, int width, int height)
{
	this->renderer = renderer;
	this->sprMngr = sprMngr;
	this->width = width;
	this->height = height;
	zombies = std::vector<Zombie*>(0);

	humans = std::vector<Human*>(0);

	sounds = new Radar(453, 0.5, 750);
	smells = new Radar(5, 0, 5000);

	zTrainer = new Trainer("data/zombies", MAX_ZOMBIE_STORAGE, DP_RANDOM_ZOMBIE_CHANCES);
	hTrainer = new Trainer("data/humans", MAX_HUMAN_STORAGE, DP_RANDOM_HUMAN_CHANCES);
	log(VERBOSE_DATA_CREATION, "CREATED DEATHPIT");
}


DeathPit::~DeathPit()
{
	delete sounds;
	delete smells;

	for (unsigned i = 0; i < zombies.size(); ++i) {
		double zombieScore = zombies[i]->capability(SDL_GetTicks());
		if (zTrainer->is_good(zombieScore)) {
			std::string fname = zTrainer->mkFName(zombieScore);
			zombies[i]->save(fname.c_str());
			zTrainer->insert(fname, zombieScore);
		}
		delete zombies[i];
	}
	zombies.clear();
	for (unsigned i = 0; i < humans.size(); ++i) {
		double humanScore = humans[i]->capability(SDL_GetTicks());
		if (hTrainer->is_good(humanScore)) {
			std::string fname = hTrainer->mkFName(humanScore);
			humans[i]->save(fname.c_str());
			hTrainer->insert(fname, humanScore);
		}
		delete humans[i];
	}
	humans.clear();
	delete zTrainer;
	renderer = NULL;
	sprMngr = NULL;
}

void DeathPit::update(unsigned delta) {
	delta *= DP_TIME_SPEED;
	// update humans
	// spawn
	if (humans.size() < DP_HUMAN_AMOUNT && rand() % (humans.size() + 1) == 0) {
		int x = rand() % (width / DP_RATIO / 2) - (width / DP_RATIO / 4);
		int y = rand() % (height / DP_RATIO / 2) - (height / DP_RATIO / 4);
		//int x = (width / DP_RATIO) / 2;
		//int y = (height / DP_RATIO) / 2;
		std::string mode = hTrainer->random();
		humans.push_back(new Human(x, y, SDL_GetTicks(), mode));
	}
	
	// update
	for (unsigned i = 0; i < humans.size(); ++i) {
		// update position
		humans[i]->update(delta,0,0,zombies,smells);
		// update smell map
		if (rand()%10==1) smells->addPoint(humans[i]->getX(), humans[i]->getY(), 1, SDL_GetTicks());
		// update sounds map
		sounds->addPoint(humans[i]->getX(), humans[i]->getY(), rand()%100+30, SDL_GetTicks());
	}
	// remove out of screen
	for (int i = humans.size() - 1; i >= 0; --i) {
		if (humans[i]->isDead() || !pointInsideRect(humans[i]->getX(), humans[i]->getY(), -width / DP_RATIO / 2, -height / DP_RATIO / 2, width / DP_RATIO, height / DP_RATIO)) {
			double humanScore = humans[i]->capability(SDL_GetTicks());
			if (hTrainer->is_good(humanScore)) {
				std::string fname = hTrainer->mkFName(humanScore);
				humans[i]->save(fname.c_str());
				hTrainer->insert(fname, humanScore);
			}
			humans.erase(humans.begin() + i);
		}
	}
	// update radars
	smells->update(delta);
	sounds->update(delta);
	// update zombies
	// create new zombies??
	if (zombies.size() < DP_ZOMBIE_AMOUNT && rand() % (zombies.size()+1) == 0) {
		//int x = (width / DP_RATIO) / 2 - 10;
		//int y = (height / DP_RATIO) / 2 - 10;
		int x = rand() % (width / DP_RATIO / 2) - (width / DP_RATIO / 4);
		int y = rand() % (height / DP_RATIO / 2) - (height / DP_RATIO / 4);
		std::string mode = zTrainer->random();
		zombies.push_back(new Zombie(x, y , SDL_GetTicks(), mode));
	}
	// update zombies
	for (unsigned i = 0; i < zombies.size(); ++i) {
		zombies[i]->update(delta,0,0, smells, sounds);
	}
	// delete zombies
	for (int i = zombies.size() - 1; i >= 0; --i) {
		if (zombies[i]->isDead() || !pointInsideRect(zombies[i]->getX(), zombies[i]->getY(), -width / DP_RATIO / 2, -height / DP_RATIO / 2, width / DP_RATIO, height / DP_RATIO)) {
			double zombieScore = zombies[i]->capability(SDL_GetTicks());
			if (zTrainer->is_good(zombieScore)) {
				std::string fname = zTrainer->mkFName(zombieScore);
				zombies[i]->save(fname.c_str());
				zTrainer->insert(fname, zombieScore);
			}
			delete zombies[i];
			zombies.erase(zombies.begin() + i);
		}
	}

	for (unsigned i = 0; i < humans.size(); ++i) {
		if (!humans[i]->isDead()) {
			double hx = humans[i]->getX();
			double hy = humans[i]->getY();
			for (unsigned j = 0; j < zombies.size(); ++j) {
				double zx = zombies[j]->getX();
				double zy = zombies[j]->getY();
				if (abs(hx - zx) < 3 && abs(hy - zy) < 3 && distP2P(hx, hy, zx, zy) <= 1.5) {
					zombies[j]->addKills(1);
					zombies.push_back(zombies[j]->clone(hx, hy, SDL_GetTicks()));
					humans[i]->kill();
					break;
				}
			}
		}
	}
}

void DeathPit::draw() {
	for (unsigned i = 0; i < humans.size(); ++i) {
		SDL_Rect humanRect = { round((humans[i]->getX() + width / DP_RATIO / 2)*DP_RATIO), round((humans[i]->getY() + height / DP_RATIO / 2)*DP_RATIO), DP_RATIO + humans[i]->minutes(SDL_GetTicks()), DP_RATIO + humans[i]->minutes(SDL_GetTicks()) };
		SDL_RenderCopy(renderer, sprMngr->getTexture("pred"), NULL, &humanRect);
	}
	for (unsigned i = 0; i < zombies.size(); ++i) {
		SDL_Rect zombieRect = { round((zombies[i]->getX() + width / DP_RATIO / 2)* DP_RATIO), round((zombies[i]->getY() + height / DP_RATIO / 2)* DP_RATIO), DP_RATIO + zombies[i]->getKills(), DP_RATIO + zombies[i]->getKills() };
		SDL_RenderCopy(renderer, sprMngr->getTexture("pgreen"), NULL, &zombieRect);
	}
}

void DeathPit::listen(bool &end, bool &pause, order_t &order, int &value) {
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