#pragma once
#include "Const.h"
#include "Functions.h"
#include <vector>

enum item_t {
	ITEM_HEAL,
	ITEM_BULLETS_1,
	ITEM_BULLETS_2,
	ITEM_BULLETS_3,
	ITEM_WEAPON_1,
	ITEM_WEAPON_2,
	ITEM_WEAPON_3,
	ITEM_MAX
};

static const int c_item[ITEM_MAX][2] = {
	{ 20, 50 },
	{ 30, 60 },
	{ 30, 60 },
	{ 4, 10 },
	{ 1, 0 },
	{ 1, 0 },
	{ 1, 0 }
};

struct item {
	item_t type;
	int amount;
	double x, y;
};

class ItemMap {
private:
	std::vector<item*> items;
public:
	ItemMap();
	~ItemMap();
	void addRandomItem(double x, double y, double w, double h);
	item* closerToPoint(double x, double y);
	item* closerToPoint(double x, double y, item_t type);
	item* collectItem(double x, double y);
	item* getItem(int id);
	void randomItemToPos(double, double);
	int size() { return items.size(); };
};

