#pragma once
#include "Const.h"
#include <vector>
struct item {
	item_t type;
	int amount;
	double x, y;
};

class ItemMap {
private:
	std::vector<item> items;
public:
	ItemMap();
	~ItemMap();
	void addRandomItem(double x, double y, double w, double h);
};

