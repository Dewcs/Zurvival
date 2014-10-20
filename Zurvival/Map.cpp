#include "Map.h"


Map::Map(int w,int h){
	centerX = 0;
	centerY = 0;
	width = w;
	height = h;
	Chunk *center = new Chunk(0,0);
}


Map::~Map(){
}


void Map::drawMap(SDL_Renderer* renderer, SpriteManager* sprMngr){
	unsigned *drawn = new unsigned[DRAWN_CHUNKS];
	for (int i = 0; i < DRAWN_CHUNKS; i++){
		drawn[i] = NULL;
	}
	center->drawChunk(centerX, centerY, width, height, drawn);
	SDL_Rect rect = { 0, 0, height / 10, height / 10 };
	SDL_RenderCopy(renderer, sprMngr->getTexture("grass"), NULL, &rect);
	delete drawn;
}

void Map::setCenter(double x, double y){
	int xfloor = floor(x);
	int yfloor = floor(y);
	int positionChunkX = xfloor / 32;
	int positionChunkY = yfloor / 32;
	if (center->areDiferentChunk(positionChunkX, positionChunkY)){
		
	}
	centerX = x;
	centerY = y;
}