#include "Chunk.h"


Chunk::Chunk(int x, int y){
	//save x and y cordinates
	this->x = x;
	this->y = y;
	//create a matrix of int's of the chunk
	matrix = new int[CHUNK_SIZE * CHUNK_SIZE];
	for (int i = 0; i < CHUNK_SIZE * CHUNK_SIZE; i++){
		matrix[i] = 0;
	}
	//initializate all pointers of NULL
	right = NULL;
	bot= NULL;
	left = NULL;
	top = NULL;

}

Chunk::Chunk(int x, int y, Chunk *r, Chunk* b, Chunk *l, Chunk *t){
	//save x and y cordinates
	this->x = x;
	this->y = y;
	//create a matrix of int's of the chunk
	matrix = new int[CHUNK_SIZE * CHUNK_SIZE];
	for (int i = 0; i < CHUNK_SIZE * CHUNK_SIZE; i++){
		matrix[i] = 0;
	}
	//initializate all pointers
	right = r;
	bot = b;
	left = l;
	top = t;
}


Chunk::~Chunk(){
}

void Chunk::randomChunk(){
	matrix = new int[CHUNK_SIZE * CHUNK_SIZE];
	for (int i = 0; i < CHUNK_SIZE * CHUNK_SIZE; i++){
		matrix[i] = rand() % 10;
	}
}

void Chunk::setChunk(int id, Chunk * chunk){

}

Chunk* Chunk::getChunk(int id){
	if (id == 0) return top;
	else if (id == 1) return right;
	else if (id == 2) return bot;
	else if(id == 3) return left;
	return NULL;
}

bool Chunk::areDiferentChunk(int x, int y){
	if (this->x != x || this->y != y){
		return true;
	}
	else return false;
}

void Chunk::drawChunk(double centerX,  double  centerY,int  width,int height ,unsigned *drawn){
	int h = TILE_FOR_HEIGHT;
	int w = width / (height / TILE_FOR_HEIGHT);
	if (rectInsideRect(centerX - (w / 2), centerY - (h / 2), w, h, x * CHUNK_SIZE, y * CHUNK_SIZE, CHUNK_SIZE, CHUNK_SIZE)){
		SDL_Rect rectToDraw = rectIntersect({ x * CHUNK_SIZE, y * CHUNK_SIZE, CHUNK_SIZE, CHUNK_SIZE }, { centerX - (w / 2), centerY - (h/ 2), w, h });
		int relativeX = rectToDraw.x % CHUNK_SIZE;
		int relativeY = rectToDraw.y % CHUNK_SIZE;
		for (int i = 0; i < rectToDraw.w ;i++){
			for (int j = 0; j < rectToDraw.h ;j++){
				switch (matrix[(relativeX+i)*CHUNK_SIZE+(relativeY+j)]){
				case GRASS:
					break;
				default:
					break;
				}
			}
		}
	}
}
