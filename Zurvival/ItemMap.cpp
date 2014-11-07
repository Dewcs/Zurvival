#include "ItemMap.h"


ItemMap::ItemMap() {
	items = std::vector<item>(0);
}


ItemMap::~ItemMap()
{
}

void ItemMap::addRandomItem(double x, double y, double w, double h) {
	item i;
	i.type = (item_t)(rand() % ITEM_MAX);
	i.amount = c_item[i.type][0] + (rand() % c_item[i.type][1]);
	items.push_back(i);
}
