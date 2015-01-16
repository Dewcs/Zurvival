#pragma once
#include "Const.h"
#include "Functions.h"
#include <vector>

// item types
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

// constant values foreach items, if second value is 0 represents a major item (weapon) otherwise its a min max value for a random with the amoun of bullets/heal it gives.
static const int c_item[ITEM_MAX][2] = {
	{ 20, 50 },
	{ 30, 60 },
	{ 30, 60 },
	{ 4, 10 },
	{ 1, 0 },
	{ 1, 0 },
	{ 1, 0 }
};
// item information
struct item {
	item_t type;
	int amount;
	double x, y;
};
/*
	Class to handle items inside a map
 */
class ItemMap {
private:
	std::vector<item*> items; // array of items
public:
	ItemMap();
	~ItemMap();
	void addRandomItem(double x, double y, double w, double h); // place a random item inside a rectangle
	item* closerToPoint(double x, double y); // item closer to x,y pos
	item* closerToPoint(double x, double y, item_t type); // item closer to x,y pos with type equal type
	item* collectItem(double x, double y); // try to extract the closer item if its close enough (removes it form the item stack)
	item* getItem(int id); // returns the item in id position in order to draw it
	void randomItemToPos(double, double); // place a random item to x,y pos
	int size() { return items.size(); }; // number of items in the map
};

