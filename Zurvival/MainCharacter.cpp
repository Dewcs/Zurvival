#include "MainCharacter.h"


MainCharacter::MainCharacter(int x,int y)
{
	this->x = x;
	this->y = y;
	move = 0;
	viewAngle = 0;
	vx = 0;
	vy = 0;
	onFire = false;
	arrayWeapons = new Weapon*[3];
	for (int i = 0; i < 3; i++){
		if (i == 0) arrayWeapons[i] = new Pistol();
		else  arrayWeapons[i] = NULL;
	}
	heals = 0;
	mainWeapon = arrayWeapons[0];
}


MainCharacter::~MainCharacter(){
	for (int i = 0; i < 3; i++){
		if (arrayWeapons[i]!=NULL) delete arrayWeapons[i];
	}
	delete[] arrayWeapons;
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
	this->move ^= move;
}

void MainCharacter::update(unsigned delta, ItemMap *itemap, ArrayBales *ab) {
	double mx = 0;
	double my = 0;
	if (move&MOVE_TOP) my -= 1;
	if (move&MOVE_RIGHT) mx += 1;
	if (move&MOVE_BOT) my += 1;
	if (move&MOVE_LEFT) mx -= 1;
	double dist = sqrt(mx*mx + my*my);
	if (dist != 0) {
		x += 2 * (mx / dist + cos(viewAngle)/2)*(delta / 1000.0);
		y += 2 * (my / dist + sin(viewAngle)/2)*(delta / 1000.0);
	}
	//give items
	item *tmp = itemap->collectItem(x, y);
	while (tmp != NULL) {
		getItem(tmp);
		delete tmp;
		tmp = itemap->collectItem(x, y);
	}
	//comprovate if player fire
	if (onFire && mainWeapon->pucDisparar()){
		mainWeapon->updateDelta(delta);
		mainWeapon->dispararBala(x, y, viewAngle, ab, this);
	}
}

double MainCharacter::getAngle() {
	return rad2deg(viewAngle);
}

double MainCharacter::getRawAngle() {
	return viewAngle;
}

double MainCharacter::getX() {
	return x;
}

double MainCharacter::getY() {
	return y;
}

void MainCharacter::getItem(item* id){
	switch (id->type){
	case ITEM_HEAL:
		//add one heal to inventory
		heals++;
		break;
	case ITEM_BULLETS_1:
		//add shotgun bullets to inventory
		if (arrayWeapons[1] != NULL) arrayWeapons[1]->afegirBales(id->amount);
		break;
	case ITEM_BULLETS_2:
		//add pistol bullets to inventory
		if (arrayWeapons[0] != NULL) arrayWeapons[0]->afegirBales(id->amount);
		break;
	case ITEM_BULLETS_3:
		//add hevymachinegun bullets to inventory
		if (arrayWeapons[2] != NULL) arrayWeapons[2]->afegirBales(id->amount);
		break;
	case ITEM_WEAPON_1:
		//add pistol to inventory or more bullets
		if (arrayWeapons[0] != NULL) arrayWeapons[0]->afegirBales(15);
		else arrayWeapons[0] = new Pistol();
		break;
	case ITEM_WEAPON_2:
		//add shotgun to inventory or more bullets
		if (arrayWeapons[1] != NULL) arrayWeapons[1]->afegirBales(8);
		else arrayWeapons[1] = new Shotgun();
		break;
	case ITEM_WEAPON_3:
		// add heavy machinegun to inventory or more bullets 
		if (arrayWeapons[2] != NULL) arrayWeapons[2]->afegirBales(100);
		else arrayWeapons[2] = new Heavymachinegun();
		break;
	default : 
		break;
	}
}

int MainCharacter::returnWeapon(int id){
	if (arrayWeapons[id] == NULL){
		return -1;
	}
	else{
		return arrayWeapons[id]->totalBales();
	}
}

int MainCharacter::returnItems(int id, bool yes){
	if (id == 2) {
		if (yes) return arrayWeapons[id]->returnBullets(true);
		else return arrayWeapons[id]->returnBullets(false);
	}
	else if (id == 0){
		if (yes) return arrayWeapons[id]->returnBullets(true);
		else return arrayWeapons[id]->returnBullets(false);
	}
	else if (id == 1){
		if (yes) return arrayWeapons[id]->returnBullets(true);
		else return arrayWeapons[id]->returnBullets(false);
	}
	else return heals;

}

void MainCharacter::useItem(int slot){
	if (slot < 4 && slot> 0){
		if (arrayWeapons[slot - 1] != NULL) mainWeapon = arrayWeapons[slot - 1];
	}else if (slot == 4){
		if (heals > 0) heals--;
	}
}

void MainCharacter::fire(bool fire){
	if (fire){
		onFire = true;
	}else{
		onFire = false;
	}
}