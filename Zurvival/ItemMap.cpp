#include "ItemMap.h"


ItemMap::ItemMap() {
	items = std::vector<item*>(0);
}


ItemMap::~ItemMap()
{
	for (int i = 0; i < items.size(); ++i) delete items[i];
	items.clear();
}

void ItemMap::addRandomItem(double x, double y, double w, double h) {
	item *i = new item();
	i->type = (item_t)(rand() % ITEM_MAX);
	i->amount = c_item[i->type][0];
	if (c_item[i->type][1]!=0) i->amount += (rand() % c_item[i->type][1]);
	i->x = randomReal(x, x + w);
	i->y = randomReal(y, y + h);
	items.push_back(i);
}

item* ItemMap::closerToPoint(double x, double y) {
	int b = -1;
	double bd;
	for (int i = 0; i < items.size(); ++i) {
		double dist = distP2P(x, y, items[i]->x, items[i]->y);
		if (b == -1 || bd>dist) {
			bd = dist;
			b = i;
		}
	}
	if (b != -1) return items[b];
	else return NULL;
}

item* ItemMap::closerToPoint(double x, double y, item_t type) {
	int b = -1;
	double bd;
	for (int i = 0; i < items.size(); ++i) {
		if (items[i]->type == type) {
			double dist = distP2P(x, y, items[i]->x, items[i]->y);
			if (b == -1 || bd>dist) {
				bd = dist;
				b = i;
			}
		}
	}
	if (b != -1) return items[b];
	else return NULL;
}

item* ItemMap::collectItem(double x, double y) {
	int b = -1;
	double bd;
	for (int i = 0; i < items.size(); ++i) {
		double dist = distP2P(x, y, items[i]->x, items[i]->y);
		if (b == -1 || bd>dist) {
			bd = dist;
			b = i;
		}
	}
	if (b != -1 && bd < 1) {
		item *tmp = items[b];
		items.erase(items.begin() + b);
		return tmp;
	} else return NULL;
}

item* ItemMap::getItem(int id) {
	return items[id];
}

void ItemMap::randomItemToPos(double humanPosX, double humanPosY){
	item *i = new item();
	i->type = (item_t)(rand() % ITEM_MAX);
	i->amount = c_item[i->type][0];
	if (c_item[i->type][1] != 0) i->amount += (rand() % c_item[i->type][1]);
	i->x = randomReal(humanPosX-1, humanPosX+1);
	i->y = randomReal(humanPosY-1, humanPosY+1);
	items.push_back(i);
}
