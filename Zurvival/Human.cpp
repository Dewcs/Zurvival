#include "Human.h"

Human::Human(int x, int y, int timestamp, std::string mode)
{
	this->x = x;
	this->y = y;
	viewAngle = 0;
	if (mode == "random") {
		ia = new Brain(21, 6);
		ia->randomize();
	}
	else if (mode == "empty") {
		ia = NULL;
	}
	else {
		ia = new Brain(21, 6);
		ia->load(mode.c_str());
		ia->tweak();
	}
	hp = 100;
	kills = 0;
	speed = 2;
	begin = timestamp;
	output = std::vector<float>(6, 0);
	input = std::vector<float>(21);
	type = ACTOR_HUMAN;
	now = timestamp;
	last_smell = -1000;
	fire = false;
	main = new Pistol();
	minx = maxx = this->x = x;
	miny = maxy = this->y = y;
	shooted = 0;
	hitted = 0;
}


Human::~Human()
{
	delete main;
	if (ia != NULL) delete ia;
	input.clear();
	output.clear();
}

void Human::prepare(double cx, double cy, std::vector<Zombie*> zombies, Radar *sounds, ItemMap *itemap) {
	// give items
	item *tmp = itemap->collectItem(x, y);
	while (tmp != NULL) {
		giveItem(tmp);
		delete tmp;
		tmp = itemap->collectItem(x, y);
	}
	// closer zombies?
	int b1, b2, b3, btmp;
	float bd1, bd2, bd3, bdtmp;
	b1 = -1;
	b2 = -1;
	b3 = -1;
	for (int i = 0; i < zombies.size(); ++i) {
		float dist = distP2P(x, y, zombies[i]->getX(), zombies[i]->getY());
		if (b1 == -1) {
			b1 = i;
			bd1 = dist;
		}
		else if (b2 == -1) {
			b2 = i;
			bd2 = dist;
		}
		else if (b3 == -1) {
			b3 = i;
			bd3 = dist;
		}
		else if (dist < bd3) {
			b3 = i;
			bd3 = dist;
		}
		// swap c to b
		if (b3 != -1 && bd3 < bd2) {
			btmp = b3;
			bdtmp = bd3;
			b3 = b2;
			bd3 = bd2;
			b2 = btmp;
			bd3 = bdtmp;
		}
		if (b2 != -1 && bd2 < bd1) {
			btmp = b2;
			bdtmp = bd2;
			b2 = b1;
			bd2 = bd1;
			b1 = btmp;
			bd1 = bdtmp;
		}
	}
	// copy previous outputs
	for (int i = 0; i < output.size(); ++i) {
		input[i] = output[i];
	}
	// current position
	input[6] = x - cx;
	input[7] = y - cx;
	if (b1 != -1) {
		input[8] = zombies[b1]->getX() - x;
		input[9] = zombies[b1]->getY() - y;
	}
	else {
		input[8] = 0;
		input[9] = 0;
	}
	if (b2 != -1) {
		input[10] = zombies[b2]->getX() - x;
		input[11] = zombies[b2]->getY() - y;
	}
	else {
		input[10] = 0;
		input[11] = 0;
	}
	if (b3 != -1) {
		input[12] = zombies[b3]->getX() - x;
		input[13] = zombies[b3]->getY() - y;
	}
	else {
		input[12] = 0;
		input[13] = 0;
	}
	double sox, soy;
	bool sov;
	// closer sound
	sounds->getValue(x, y, sox, soy, sov);
	input[14] = 0;
	input[15] = 0;
	if (sov) {
		input[14] = sox - x;
		input[15] = soy - y;
	}
	// bullets
	input[16] = main->totalBales();
	// closer heal
	item* heal=itemap->closerToPoint(x, y, ITEM_HEAL);
	if (heal != NULL) {
		input[17] = heal->x - x;
		input[18] = heal->y - y;
	}
	else {
		input[17] = 0;
		input[18] = 0;
	}
	// close ammo
	item* bullets = itemap->closerToPoint(x, y, ITEM_BULLETS_2);
	if (bullets != NULL) {
		input[19] = bullets->x - x;
		input[20] = bullets->y - y;
	}
	else {
		input[19] = 0;
		input[20] = 0;
	}
	for (int i = 0; i < input.size(); ++i) {
		if (!std::isfinite(input[i])) log(VERBOSE_ERRORS, "BAD INPUT %d", i);
	}
	think();
}

void Human::update(unsigned delta, ArrayBales *ab) {
	main->updateDelta(delta);
	now += delta;
	// update view angle
	if (output[0] != 0 || output[1] != 0) {
		viewAngle = angleP2P(0, 0, output[0], output[1]);
	}
	// update position
	if (output[2] != 0) {
		x += cos(viewAngle) * speed * delta / 1000.0;
		y += sin(viewAngle) * speed * delta / 1000.0;
		minx = min(minx, x);
		maxx = max(maxx, x);
		miny = min(minx, y);
		maxy = max(maxx, y);
	}
	// fire ??
	if (output[3] != 0 && main->pucDisparar()) {
		fire = true;
		++shooted;
		main->dispararBala(x, y, viewAngle, ab, this);
	}
	else {
		fire = false;
	}
}
Human* Human::clone(int x, int y, int timestamp) {
	Human *ret = new Human(x, y, timestamp, "empty");
	Brain *cpy = ia->copy();
	cpy->tweak();
	ret->setBrain(cpy);
	return ret;
}
double Human::capability() {
	double kps = 0;
	double area = (maxx - minx)*(maxy - miny);
	double af = 1.0 - 1000.0 / (1000.0 + area);
	double kf = 1.0 - 10.0 / (11 + kills);
	double df = 1.0 - 100.0 / (100.0 + damageDealt);
	double hf = 0;
	if (shooted>0) hf = hitted / shooted;
	kps = af * kf * df * hf;
	return kps;
}

bool Human::fired() {
	return fire;
}
bool Human::moved() {
	return output[0] != 0 || output[1] != 0;
}

bool Human::emitSmell() {
	if (now - last_smell > 250) {
		last_smell = now;
		return true;
	} else return false;
}

void Human::giveItem(item *i) {
	if (i->type == ITEM_HEAL) {
		hp = max(100, hp + i->amount);
	}
	else if (i->type == ITEM_BULLETS_2) {
		main->afegirBales(i->amount);
	}
}