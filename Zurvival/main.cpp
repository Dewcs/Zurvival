// Using SDL2 to create an application window
#include "Zurvival.h"
#include <iostream>

int main(int argc, char* argv[]){
	// main loop of the program
	Zurvival *game=new Zurvival();
	while (game->running()) {
		game->update();
	}
	delete game;
	return 0;
}