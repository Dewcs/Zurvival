#include "Game.h"


Game::Game(SDL_Renderer* renderer, SpriteManager* sprMngr,int width,int height)
{
	this->renderer = renderer;
	this->sprMngr = sprMngr;
	mc = new MainCharacter(0,0);
	this->width = width;
	this->height = height;
	gmap = new Map(width, height);

	zombies = std::vector<Zombie*>(0);

	humans = std::vector<Human*>(0);

	sounds = new Radar(453, 0.999, 750);
	smells = new Radar(5, 0, 5000);

	zTrainer = new Trainer("data/zombies", MAX_ZOMBIE_STORAGE, 0);
	hTrainer = new Trainer("data/humans", MAX_HUMAN_STORAGE, 0);

	itemap = new ItemMap();

	bales = new ArrayBales();

	log(VERBOSE_DATA_CREATION, "CREATED GAME");
}


Game::~Game()
{
	delete mc;
	delete gmap;
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

void Game::spawn(unsigned delta) {
	// humans
	int total_width = (width / DP_RATIO) * 2 / 3;
	int total_height = (height / DP_RATIO) * 2 / 3;
	int begin_x = -total_width / 2;
	int begin_y = -total_height / 2;
	if (humans.size() < DP_HUMAN_AMOUNT && rand() % (humans.size() + 1) == 0) {
		int x = rand() % (total_width)+begin_x;
		int y = rand() % (total_height)+begin_y;
		std::string mode = hTrainer->random();
		humans.push_back(new Human(x, y, SDL_GetTicks(), mode));
		if (mode != "random") hTrainer->remove(mode);
	}
	// zombies
	if (zombies.size() < DP_ZOMBIE_AMOUNT && rand() % (zombies.size() + 1) == 0) {
		int x = rand() % (total_width)+begin_x;
		int y = rand() % (total_height)+begin_y;
		std::string mode = zTrainer->random();
		zombies.push_back(new Zombie(x, y, SDL_GetTicks(), mode));
		if (mode != "random") zTrainer->remove(mode);
	}
	// items
	/*if (itemap->size() / double(total_width * total_height) < ITEMS_PER_SQM && rand() % (itemap->size() + 1) == 0) {
		itemap->addRandomItem((double)begin_x, (double)begin_y, (double)total_width, (double)total_height);
	}*/
}

void Game::cleanup() {
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

void Game::update(unsigned delta) {
	// update main
	// update view
	int mx, my;
	SDL_GetMouseState(&mx, &my);
	mc->setView(mx - width / 2, my - height / 2);
	// update position
	mc->update(delta, itemap);
	// update map
	gmap->setCenter(mc->getX(), mc->getY(), itemap);
	// update smell map
	smells->addPoint(mc->getX(), mc->getY(), 1, SDL_GetTicks());
	// update sounds map
	sounds->addPoint(mc->getX(), mc->getY(), 30, SDL_GetTicks());
	// update zombies
	spawn(delta);
	// update humans
	for (unsigned i = 0; i < humans.size(); ++i) {
		humans[i]->prepare(0, 0, zombies, smells, itemap);
		humans[i]->update(delta, bales);
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
	for (int i = bales->size() - 1; i >= 0; --i) {
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
							if (humans.size()< DP_HUMAN_AMOUNT * 2) humans.push_back(h->clone(zombies[j]->getX(), zombies[j]->getY(), SDL_GetTicks()));
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
			if (abs(hx - zx) < 3 && abs(hy - zy) < 3 && distP2P(hx, hy, zx, zy) <= 1.2 && zombies[j]->canAttack()) {
				double zombieDmg = zombies[j]->getDamage();
				zombies[j]->addDamageDealt(zombieDmg);
				humans[i]->doDamage(zombieDmg);
				if (humans[i]->isDead()) {
					zombies[j]->addKills(1);
					if (zombies.size()< DP_ZOMBIE_AMOUNT * 2) zombies.push_back(zombies[j]->clone(hx, hy, SDL_GetTicks()));
				}
			}
		}
	}
	// cleanup
	cleanup();
}

void Game::draw() {
	// draw bg
	gmap->drawMap(renderer, sprMngr);
	//draw items
	drawItems();
	// draw main character
	int mainw = height / 15;
	SDL_Rect mainCharacter = { width / 2 - mainw / 2, height / 2 - mainw / 2, height / 15, height / 15 };
	SDL_RenderCopyEx(renderer, sprMngr->getTexture("soldier"), NULL, &mainCharacter, mc->getAngle() - 90, NULL, SDL_FLIP_NONE);
	// draw npc
	int hwidth = height / 15;
	SDL_Rect screen = { 0, 0, width, height };
	for (int i = 0; i < humans.size(); ++i) {
		int x, y;
		gmap->getScreenPosition(humans[i]->getX(), humans[i]->getY(), x, y);
		SDL_Rect humanRect = { x - hwidth / 2, y - hwidth / 2, hwidth, hwidth };
		if (rectInsideRect(screen, humanRect)) SDL_RenderCopyEx(renderer, sprMngr->getTexture("soldier"), NULL, &humanRect, humans[i]->getAngle() - 90, NULL, SDL_FLIP_NONE);
	}
	// draw zombies
	int zwidth = height / 15;
	for (int i = 0; i < zombies.size(); ++i) {
		int x, y;
		gmap->getScreenPosition(zombies[i]->getX(), zombies[i]->getY(), x, y);
		SDL_Rect zombieRect = { x - zwidth / 2, y - zwidth / 2, zwidth, zwidth };
		if (rectInsideRect(screen, zombieRect)) SDL_RenderCopyEx(renderer, sprMngr->getTexture("zombie_anim" + std::to_string((SDL_GetTicks()/100)%8)), NULL, &zombieRect, zombies[i]->getAngle() - 90, NULL, SDL_FLIP_NONE);
	}
	// draw bullets
	for (unsigned i = 0; i < bales->size(); ++i) {
		float x0, y0, dist, angle;
		bales->getDrawInfo(i, x0, y0, dist, angle);
		int x, y;
		gmap->getScreenPosition(x0, y0, x, y);
		SDL_Point bulletPoint2 = { 0, 1 };
		SDL_Rect bulletRect = { x, y, dist*(height/15), 1 };
		SDL_RenderCopyEx(renderer, sprMngr->getTexture("pwhite"), NULL, &bulletRect, rad2deg(angle), &bulletPoint2, SDL_FLIP_NONE); // &bulletPoint rad2deg(angle)
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
	//draw backgroundGUI
	SDL_RenderCopy(renderer, sprMngr->getTexture("backgroundGUI"), NULL, &sprMngr->getRect("backgroundGUI"));
	//draw slots 
	SDL_RenderCopy(renderer, sprMngr->getTexture("pistolSlot"), NULL, &sprMngr->getRect("pistolSlot"));
	SDL_RenderCopy(renderer, sprMngr->getTexture("shotgunSlot"), NULL, &sprMngr->getRect("shotgunSlot"));
	SDL_RenderCopy(renderer, sprMngr->getTexture("hevySlot"), NULL, &sprMngr->getRect("hevySlot"));
	SDL_RenderCopy(renderer, sprMngr->getTexture("healSlot"), NULL, &sprMngr->getRect("healSlot"));
	SDL_RenderCopy(renderer, sprMngr->getTexture("retroceso"), NULL, &sprMngr->getRect("retroceso"));
	//draw pistol
	SDL_RenderCopy(renderer, sprMngr->getTexture("pistolWep"), NULL, &sprMngr->getRect("pistolWep"));
	sprMngr->drawNumber(mc->returnItems(0, true), "red_numbers", (width / 30) + (height / 8), (width / 30) + (height / 10), height / 50, ALIGN_RIGHT);
	sprMngr->drawNumber(mc->returnItems(0, false), "red_numbers", (width / 30) + (height / 9.5), (width / 30) + (height / 10), height / 50, ALIGN_LEFT);
	//first, draw heal item sprite
	SDL_RenderCopy(renderer, sprMngr->getTexture("healSprite"), NULL, &sprMngr->getRect("healSprite"));
	sprMngr->drawNumber(mc->returnItems(3, true), "red_numbers", (width / 30) + (height / 8.8), ((width / 30) * 4 + (height / 8) * 3) + (height/10 ), height / 50, ALIGN_CENTER);
	//comprovate if is necesary draw into slots
	//next, all weapons
	if (mc->returnWeapon(1) != -1){
		SDL_RenderCopy(renderer, sprMngr->getTexture("shotgunWep"), NULL, &sprMngr->getRect("shotgunWep"));
		sprMngr->drawNumber(mc->returnItems(1, true), "red_numbers", (width / 30) + (height / 8), ((width / 30) * 2 + (height / 8)) + (height / 10), height / 50, ALIGN_CENTER);
		sprMngr->drawNumber(mc->returnItems(1, false), "red_numbers", (width / 30) + (height / 9.5), ((width / 30) * 2 + (height / 8)) + (height / 10), height / 50, ALIGN_CENTER);
	}
	if (mc->returnWeapon(2) != -1){
		SDL_RenderCopy(renderer, sprMngr->getTexture("hevyWep"), NULL, &sprMngr->getRect("hevyWep"));
		sprMngr->drawNumber(mc->returnItems(2, true), "red_numbers", (width / 30) + (height / 9), ((width / 30) * 3 + (height / 8) * 2) + (height / 10), height / 50, ALIGN_CENTER);
		sprMngr->drawNumber(mc->returnItems(2, false), "red_numbers", (width / 30) + (height / 12), ((width / 30) * 3 + (height / 8) * 2) + (height / 10), height / 50, ALIGN_CENTER);
	}

}

void Game::drawItems(){
	int iwidth = height / 15;
	SDL_Rect screen = {0,0,width,height};
	for (unsigned i = 0; i < itemap->size(); ++i) {
		item *tmp = itemap->getItem(i);
		int x, y;
		gmap->getScreenPosition(tmp->x, tmp->y, x, y);
		SDL_Rect itemRect = { x - iwidth / 2, y - iwidth / 2, iwidth, iwidth };
		if (rectInsideRect(screen, itemRect)){
			switch (tmp->type)
			{
			case ITEM_HEAL:
				SDL_RenderCopy(renderer, sprMngr->getTexture("healSprite"), NULL, &itemRect);
				break;
			case ITEM_BULLETS_1:
				SDL_RenderCopy(renderer, sprMngr->getTexture("shotgunAmo"), NULL, &itemRect);
				break;
			case ITEM_BULLETS_2:
				SDL_RenderCopy(renderer, sprMngr->getTexture("pistolAmo"), NULL, &itemRect);
				break;
			case ITEM_BULLETS_3:
				SDL_RenderCopy(renderer, sprMngr->getTexture("hevyAmo"), NULL, &itemRect);
				break;
			case ITEM_WEAPON_1:
				SDL_RenderCopy(renderer, sprMngr->getTexture("pistolWep"), NULL, &itemRect);
				break;
			case ITEM_WEAPON_2:
				SDL_RenderCopy(renderer, sprMngr->getTexture("shotgunWep"), NULL, &itemRect);
				break;
			case ITEM_WEAPON_3:
				SDL_RenderCopy(renderer, sprMngr->getTexture("hevyWep"), NULL, &itemRect);
				break;
			default:
				break;
			}
		}
	}
}

