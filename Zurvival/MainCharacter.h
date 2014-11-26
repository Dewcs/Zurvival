#pragma once
#include <iostream>
#include "Const.h"
#include "Functions.h"
#include "Weapon.h"
#include "ItemMap.h"

class MainCharacter{
	private:
		unsigned move; // moving direction
		double x, y; // current position
		double vx, vy; // view position
		double viewAngle; // angle of view direction
		Weapon ** arrayWeapons;
		Weapon * mainWeapon;
		bool onFire;
		int heals;
	public:
		MainCharacter(int x,int y);
		~MainCharacter();
		void setView(int x, int y);
		void startMove(move_t move); // 0,1,2,4,8 none,north,right,south,left
		void stopMove(move_t move);
		void update(unsigned delta, ItemMap *itemap, ArrayBales *ab); // how many ms passed since last update
		double getAngle(); // return the angle in degrees
		double getRawAngle(); // return angle in radians
		double getX(); // get the x position
		double getY(); // get the y position
		void getItem(item*);
		int returnWeapon(int id);
		int returnItems(int id, bool yes);
		void useItem(int slot);
		void fire(bool fire);
};

