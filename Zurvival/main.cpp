// Using SDL2 to create an application window
#include "Zurvival.h"
#include "Functions.h"
#include <iostream>

int main(int argc, char* argv[]){
	// main loop of the program
	log(VERBOSE_BASIC, "VERBOSE LEVEL %d",VERBOSE_LEVEL);
	log(VERBOSE_BASIC, "BEGIN GAME");
	Zurvival *game=new Zurvival();
	log(VERBOSE_DATA_CREATION, "CREATED ZURVIVAL");
	while (game->running()) {
		game->update();
	}
	delete game;
	log(VERBOSE_BASIC, "END GAME");
	return 0;
}