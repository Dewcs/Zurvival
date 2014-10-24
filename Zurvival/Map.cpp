#include "Map.h"


Map::Map(int w,int h){
	centerX = 0;
	centerY = 0;
	width = w;
	height = h;
	center = new Chunk(0,0);
}


Map::~Map(){
}


void Map::drawMap(SDL_Renderer* renderer, SpriteManager* sprMngr){
	unsigned *drawn = new unsigned[DRAWN_CHUNKS];
	for (int i = 0; i < DRAWN_CHUNKS; i++){
		drawn[i] = NULL;
	}
	center->drawChunk(centerX, centerY, width, height, drawn, renderer, sprMngr);
	//SDL_Rect rect = { 0, 0, height / 10, height / 10 };
	//SDL_RenderCopy(renderer, sprMngr->getTexture("grass"), NULL, &rect);
	delete drawn;
}

void Map::setCenter(double x, double y){
	int xfloor = floor(x);
	int yfloor = floor(y);
	int positionChunkX = xfloor / CHUNK_SIZE;
	int positionChunkY = yfloor / CHUNK_SIZE;
	if (center->areDiferentChunk(positionChunkX, positionChunkY)){
		
	}
	centerX = x;
	centerY = y;
	// check neighboors
	center->spawnNeighbors(createWindow());
}

SDL_Rect Map::createWindow(){
	double sizeOnPixels = height / TILE_FOR_HEIGHT;
	//creem una variable que sigui la height de tiles
	double h_tiles = TILE_FOR_HEIGHT;
	//el mateix per la width de tiles
	double w_tiles = width / sizeOnPixels;
	//retornem el rectangle que sera la nostre finestra 
	return{ floor(centerX - (w_tiles / 2)), floor(centerY - (h_tiles / 2)), ceil(w_tiles), ceil(h_tiles) };
}