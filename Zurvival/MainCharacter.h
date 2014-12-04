#pragma once
#include <iostream>
#include "Const.h"
#include "Functions.h"
#include "Weapon.h"
#include "ItemMap.h"
#include "Actor.h"

class MainCharacter : public Actor{
	private:
		unsigned move; // moving direction
		double vx, vy; // view position
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
		void getItem(item*);
		int returnWeapon(int id);
		int returnItems(int id, bool yes);
		void useItem(int slot);
		void fire(bool fire);
};

