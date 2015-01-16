#include "Map.h"


Map::Map(int w,int h){
	centerX = 0;
	centerY = 0;
	width = w;
	height = h;
	center = new Chunk(0, 0, &exists);
}


Map::~Map(){
	delete center;
	exists.clear();
}


void Map::drawMap(SDL_Renderer* renderer, SpriteManager* sprMngr, TileManager *tm){
	unsigned *drawn = new unsigned[DRAWN_CHUNKS];
	for (int i = 0; i < DRAWN_CHUNKS; i++){
		drawn[i] = NULL;
	}
	center->drawChunk(centerX, centerY, width, height, drawn, renderer, sprMngr,tm);
	//SDL_Rect rect = { 0, 0, height / 10, height / 10 };
	//SDL_RenderCopy(renderer, sprMngr->getTexture("grass"), NULL, &rect);
	center->resetCalls();
	delete drawn;
}

void Map::setCenter(double x, double y, ItemMap * itemap){
	int xfloor = floor(x);
	int yfloor = floor(y);
	int positionChunkX = xfloor / CHUNK_SIZE;
	int positionChunkY = yfloor / CHUNK_SIZE;
	if (xfloor < 0) positionChunkX = positionChunkX - 1;
	if (yfloor < 0) positionChunkY = positionChunkY - 1;
	// chekc if new center is another chunk
	if (center->areDiferentChunk(positionChunkX, positionChunkY)){
		center = center->search(positionChunkX, positionChunkY);
	}
	centerX = x;
	centerY = y;
	// check neighboors
	center->spawnNeighbors(createWindow(), itemap);
	center->resetCalls();
}

SDL_Rect Map::createWindow(){
	double sizeOnPixels = height / TILE_FOR_HEIGHT;
	//creem una variable que sigui la height de tiles
	double h_tiles = TILE_FOR_HEIGHT;
	//el mateix per la width de tiles
	double w_tiles = width / sizeOnPixels;
	//retornem el rectangle que sera la nostre finestra 
	return { floor(centerX - (w_tiles / 2)), floor(centerY - (h_tiles / 2)), ceil(w_tiles), ceil(h_tiles) };
}

void Map::getScreenPosition(double x, double y, int &sx, int &sy) {
	double scale = (double)height/TILE_FOR_HEIGHT;
	sx = round(width / 2 + (x-centerX) * scale);
	sy = round(height / 2 + (y-centerY) * scale);
}