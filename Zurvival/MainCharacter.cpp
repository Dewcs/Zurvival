#include "MainCharacter.h"


MainCharacter::MainCharacter(int x,int y)
{
	this->x = x;
	this->y = y;
	move = 0;
	viewAngle = 0;
	vx = 0;
	vy = 0;
}


MainCharacter::~MainCharacter()
{
}

void MainCharacter::setView(int x, int y) {
	vx = x;
	vy = y;
	if (vx == 0 && vy == 0) viewAngle = 0;
	else viewAngle = angleP2P(0, 0, x, y);
}

void MainCharacter::startMove(move_t move) {
	this->move |= move;
}

void MainCharacter::stopMove(move_t move) {
	this->move &= !(unsigned)move;
}

void MainCharacter::update(unsigned delta) {
	int mx = 0;
	int my = 0;
	if (move&MOVE_TOP) mx += 1;
	if (move&MOVE_RIGHT) mx += 1;
	if (move&MOVE_BOT) mx -= 1;
	if (move&MOVE_LEFT) mx -= 1;
	double dist = sqrt(mx*mx + my*my);
	if (dist != 0) {
		x += dist*mx*delta / 1000;
		y += dist*my*delta / 1000;
	}
}