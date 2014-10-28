#pragma once
#include <iostream>
#include "Const.h"
#include "Functions.h"
#include "Brain.h"
#include "Radar.h"

class Zombie {
private:
	double x, y;
	double lx, ly;
	double viewAngle;
	Brain *ia;
	int kills;
	int hp;
public:
	Zombie(int x, int y);
	~Zombie();
	bool isDead();
	void update(unsigned delta,Radar *smells,Radar * sounds);
	double getAngle();
	double getRawAngle();
	double getX();
	double getY();
	Zombie* clone(int x, int y);
	void setBrain(Brain *brain);
	void addKills(int k) { kills += k; hp += k * 100000; };
	int getKills() { return kills; };
};

