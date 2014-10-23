#pragma once
#include "Actor.h"
class Zombie : public Actor {
private:

public:
	Zombie(int x, int y);
	~Zombie();
	bool isDead();
};

