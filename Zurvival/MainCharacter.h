#pragma once
#include <iostream>
#include "Const.h"
#include "Functions.h"
#include "Weapon.h"
#include "ItemMap.h"
#include "Actor.h"

/*
	Class of the main chaacter controled by keyboard and mouse
*/
class MainCharacter : public Actor{
	private:
		unsigned move; // moving direction
		double vx, vy; // view position
		Weapon ** arrayWeapons; // array of weapon pointer
		Weapon * mainWeapon; // weapon in usage
		bool onFire; // is firing?
		int heals; // times it can heal
	public:
		MainCharacter(int x,int y);
		~MainCharacter();
		void setView(int x, int y); // place the main character lokking at x,y
		void startMove(move_t move); // 0,1,2,4,8 none,north,right,south,left
		void stopMove(move_t move); // 0,1,2,4,8 none,north,right,south,left
		void update(unsigned delta, ItemMap *itemap, ArrayBales *ab); // how many ms passed since last update
		void getItem(item*); // give item to hero
		int returnWeapon(int id); // get the weapon bullets
		int returnItems(int id, bool yes); // get an item value
		void useItem(int slot); // use item in slot slot
		void fire(bool fire); // set firing value
		int getWeapon(); // get current weapon
		void reload(); // reload gun
		bool isFiring() { return onFire; } // is the player firing?
		bool isMoving() { return move != 0; } // is the player moving?
};

