#include "DeathPit.h"


DeathPit::DeathPit(SDL_Renderer* renderer, SpriteManager* sprMngr, int width, int height)
{
	this->renderer = renderer;
	this->sprMngr = sprMngr;
	this->width = width;
	this->height = height;

	zombies = std::vector<Zombie*>(0);

	humans = std::vector<Human*>(0);

	sounds = new Radar(453, 0.999, 750);
	smells = new Radar(5, 0, 5000);

	zTrainer = new Trainer("data/zombies", MAX_ZOMBIE_STORAGE, DP_RANDOM_ZOMBIE_CHANCES);
	hTrainer = new Trainer("data/humans", MAX_HUMAN_STORAGE, DP_RANDOM_HUMAN_CHANCES);

	itemap = new ItemMap();

	bales = new ArrayBales();

	log(VERBOSE_DATA_CREATION, "CREATED DEATHPIT");
}


DeathPit::~DeathPit()
{
	delete sounds;
	delete smells;

	for (unsigned i = 0; i < zombies.size(); ++i) {
		double zombieScore = zombies[i]->capability();
		if (zTrainer->is_good(zombieScore)) {
			std::string fname = zTrainer->mkFName(zombieScore);
			zombies[i]->save(fname.c_str());
			zTrainer->insert(fname, zombieScore);
		}
		delete zombies[i];
	}
	zombies.clear();

	for (unsigned i = 0; i < humans.size(); ++i) {
		double humanScore = humans[i]->capability();
		if (hTrainer->is_good(humanScore)) {
			std::string fname = hTrainer->mkFName(humanScore);
			humans[i]->save(fname.c_str());
			hTrainer->insert(fname, humanScore);
		}
		delete humans[i];
	}
	humans.clear();

	delete zTrainer;
	delete hTrainer;
	delete itemap;

	delete bales;

	renderer = NULL;
	sprMngr = NULL;
}

// TODO spawn acording to delta!!
void DeathPit::spawn(unsigned delta) {
	// humans
	int total_width = (width / DP_RATIO) * 2 / 3;
	int total_height = (height / DP_RATIO) * 2 / 3;
	int begin_x = -total_width / 2;
	int begin_y = -total_height / 2;
	if (humans.size() < DP_HUMAN_AMOUNT && rand() % (humans.size() + 1) == 0) {
		int x = rand() % (total_width) + begin_x;
		int y = rand() % (total_height) + begin_y;
		std::string mode = hTrainer->random();
		humans.push_back(new Human(x, y, SDL_GetTicks(), mode));
	}
	// zombies
	if (zombies.size() < DP_ZOMBIE_AMOUNT && rand() % (zombies.size() + 1) == 0) {
		int x = rand() % (total_width)+begin_x;
		int y = rand() % (total_height)+begin_y;
		std::string mode = zTrainer->random();
		zombies.push_back(new Zombie(x, y, SDL_GetTicks(), mode));
	}
	// items
	if (itemap->size() / double(total_width * total_height) < ITEMS_PER_SQM && rand() % (itemap->size() + 1) == 0) {
		itemap->addRandomItem((double)begin_x, (double)begin_y, (double)total_width, (double)total_height);
	}
}

void DeathPit::cleanup() {
	// remove dead and out of screen
	for (int i = humans.size() - 1; i >= 0; --i) {
		if (humans[i]->isDead() || !pointInsideRect(humans[i]->getX(), humans[i]->getY(), -width / DP_RATIO / 2, -height / DP_RATIO / 2, width / DP_RATIO, height / DP_RATIO)) {
			double humanScore = humans[i]->capability();
			if (!humans[i]->isDead()) humanScore *= 0.5;
			if (humanScore>0 && hTrainer->is_good(humanScore)) {
				std::string fname = hTrainer->mkFName(humanScore);
				humans[i]->save(fname.c_str());
				hTrainer->insert(fname, humanScore);
			}
			bales->unlinkOwner(humans[i]);
			delete humans[i];
			humans.erase(humans.begin() + i);
		}
	}
	for (int i = zombies.size() - 1; i >= 0; --i) {
		if (zombies[i]->isDead() || !pointInsideRect(zombies[i]->getX(), zombies[i]->getY(), -width / DP_RATIO / 2, -height / DP_RATIO / 2, width / DP_RATIO, height / DP_RATIO)) {
			double zombieScore = zombies[i]->capability();
			if (!zombies[i]->isDead()) zombieScore *= 0.5;
			if (zombieScore>0 && zTrainer->is_good(zombieScore)) {
				std::string fname = zTrainer->mkFName(zombieScore);
				zombies[i]->save(fname.c_str());
				zTrainer->insert(fname, zombieScore);
			}
			delete zombies[i];
			zombies.erase(zombies.begin() + i);
		}
	}
}

void DeathPit::update(unsigned delta) {
	delta *= DP_TIME_SPEED;
	spawn(delta);
	// update humans
	for (unsigned i = 0; i < humans.size(); ++i) {
		humans[i]->prepare(0, 0, zombies, smells, itemap);
		humans[i]->update(delta,bales);
		if (humans[i]->emitSmell()) smells->addPoint(humans[i]->getX(), humans[i]->getY(), 1, SDL_GetTicks());
		if (humans[i]->fired()) sounds->addPoint(humans[i]->getX(), humans[i]->getY(), 100, SDL_GetTicks());
		else if (humans[i]->moved()) sounds->addPoint(humans[i]->getX(), humans[i]->getY(), 20, SDL_GetTicks());

	}
	// update radars
	smells->update(delta);
	sounds->update(delta);
	// update zombies
	for (unsigned i = 0; i < zombies.size(); ++i) {
		zombies[i]->prepare(0, 0, smells, sounds);
		zombies[i]->update(delta);
	}
	// update bales
	bales->updateBales(delta);
	// update damages
	for (int i = bales->size()-1; i >= 0; --i) {
		Segment s = bales->getBalaSegment(i);
		Human *h = (Human*)bales->getBalaOwner(i);
		for (unsigned j = 0; j < zombies.size(); ++j) {
			if (!zombies[j]->isDead()) {
				Circle c = zombies[j]->getCircle();
				if (collide(s, c)) {
					double bulletDmg = 40;
					bales->remove(i);
					zombies[j]->doDamage(bulletDmg);
					if (h != NULL) {
						h->addHitted();
						h->addDamageDealt(bulletDmg);
						if (zombies[j]->isDead()) {
							h->addKills(1);
							humans.push_back(h->clone(zombies[j]->getX(), zombies[j]->getY(), SDL_GetTicks()));
						}
					}
					break;
				}
			}
		}
	}
	// zombie attack human
	for (unsigned i = 0; i < humans.size(); ++i) {
		double hx = humans[i]->getX();
		double hy = humans[i]->getY();
		for (unsigned j = 0; j < zombies.size() && !humans[i]->isDead(); ++j) {
			double zx = zombies[j]->getX();
			double zy = zombies[j]->getY();
			if (abs(hx - zx) < 3 && abs(hy - zy) < 3 && distP2P(hx, hy, zx, zy) <= 1.2) {
				double zombieDmg = zombies[j]->getDamage();
				zombies[j]->addDamageDealt(zombieDmg);
				humans[i]->doDamage(zombieDmg);
				if (humans[i]->isDead()) {
					zombies[j]->addKills(1);
					zombies.push_back(zombies[j]->clone(hx, hy, SDL_GetTicks()));
				}
			}
		}
	}
	// cleanup
	cleanup();
}

void DeathPit::draw() {
	// draw items
	for (unsigned i = 0; i < itemap->size(); ++i) {
		item *tmp = itemap->getItem(i);
		SDL_Rect itemRect = { round((tmp->x + width / DP_RATIO / 2)*DP_RATIO), round((tmp->y + height / DP_RATIO / 2)*DP_RATIO), DP_RATIO , DP_RATIO };
		SDL_RenderCopy(renderer, sprMngr->getTexture("pblue"), NULL, &itemRect);
	}
	// draw bullets
	for (unsigned i = 0; i < bales->size(); ++i) {
		float x0, y0, dist, angle;
		bales->getDrawInfo(i, x0, y0, dist, angle);
		SDL_Point bulletPoint = { round((x0 + width / DP_RATIO / 2)*DP_RATIO), round((y0 + height / DP_RATIO / 2)*DP_RATIO) };
		SDL_Point bulletPoint2 = { 0, 1 };
		SDL_Rect bulletRect = { bulletPoint.x, bulletPoint.y, dist*DP_RATIO, 1 };
		SDL_RenderCopyEx(renderer, sprMngr->getTexture("pwhite"), NULL, &bulletRect, rad2deg(angle), &bulletPoint2, SDL_FLIP_NONE); // &bulletPoint rad2deg(angle)
	}
	// draw humans
	for (unsigned i = 0; i < humans.size(); ++i) {
		SDL_Rect humanRect = { round((humans[i]->getX() + width / DP_RATIO / 2)*DP_RATIO), round((humans[i]->getY() + height / DP_RATIO / 2)*DP_RATIO), DP_RATIO + humans[i]->getKills(), DP_RATIO + humans[i]->getKills() };
		SDL_RenderCopy(renderer, sprMngr->getTexture("pred"), NULL, &humanRect);
	}
	// draw zombies
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