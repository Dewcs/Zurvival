#include "Chunk.h"


Chunk::Chunk(int x, int y, std::set<int>* exists){
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

	isCalled = false;

	this->exists = exists;
	std::cout << exists->count(chunkUID(x, y)) << " " << chunkUID(x, y) << std::endl;
	this->exists->insert(chunkUID(x, y));
	
}

Chunk::Chunk(int x, int y, std::set<int>* exists, Chunk *r, Chunk* b, Chunk *l, Chunk *t){
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

	isCalled = false;

	this->exists = exists;
	std::cout << exists->count(chunkUID(x, y)) << " " << chunkUID(x, y) << std::endl;
	this->exists->insert(chunkUID(x, y));
}


Chunk::~Chunk(){
	// fatal!!!!!!!
	delete left;
	delete top;
	delete right;
	delete bot;
	delete matrix;
}

void Chunk::randomChunk(){
	matrix = new int[CHUNK_SIZE * CHUNK_SIZE];
	for (int i = 0; i < CHUNK_SIZE * CHUNK_SIZE; i++){
		matrix[i] = rand() % 10;
	}
}

void Chunk::setChunk(int id, Chunk * chunk){
	if (id == 0) top=chunk;
	else if (id == 1) right = chunk;
	else if (id == 2) bot = chunk;
	else if (id == 3) left = chunk;
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

void Chunk::drawChunk(double centerX_M, double  centerY_M, int  width_pixels, int height_pixels, unsigned *drawn, SDL_Renderer* renderer, SpriteManager* sprMngr){
	//creem una variable que sigui la width i el height d'un tile en pixels
	int sizeOnPixels = height_pixels / TILE_FOR_HEIGHT;
	//creem una variable que sigui la height de tiles
	int h_tiles = TILE_FOR_HEIGHT;

	//el mateix per la width de tiles
	int w_tiles = ceil(width_pixels / (double)(height_pixels / TILE_FOR_HEIGHT));
	
	//comprovem si hem de pintar el chunk
	if (rectInsideRect(floor(centerX_M - (w_tiles / 2)), floor(centerY_M - (h_tiles / 2)), w_tiles, h_tiles, x * CHUNK_SIZE, y * CHUNK_SIZE, CHUNK_SIZE, CHUNK_SIZE)) {

		//creem un rectancle del tros de chunk que hem de pintar en pantalla
		SDL_Rect rectToDraw = rectIntersect({ x * CHUNK_SIZE, y * CHUNK_SIZE, CHUNK_SIZE, CHUNK_SIZE }, { floor(centerX_M - (w_tiles / 2)), floor(centerY_M - (h_tiles/ 2)), w_tiles, h_tiles });
		
		//crrem dues variables que ens guardin la distancia en pixels des del centre fins al vertex del rectangle que hem de pintar
		int distInPixelsX = (rectToDraw.x - centerX_M)*sizeOnPixels;
		int distInPixelsY = (rectToDraw.y - centerY_M)*sizeOnPixels;

		//creem dues variables que siguin la posicio en pixels del nostre vertex
		int vertexDrawX = (width_pixels / 2) + distInPixelsX;
		int vertexDrawY = (height_pixels / 2) + distInPixelsY;
		int relativeX = rectToDraw.x % CHUNK_SIZE;
		int relativeY = rectToDraw.y % CHUNK_SIZE;
		
		//fer dos bucles per recorrer el rectangle que hem de pintar 
		for (int i = 0; i <= rectToDraw.w; i++){
			for (int j = 0; j <= rectToDraw.h;j++){
				if (relativeX + i >= 0 && relativeX + i < CHUNK_SIZE && relativeY + j >= 0 && relativeY + j < CHUNK_SIZE) {
					SDL_Rect rect = { vertexDrawX + (i*sizeOnPixels) + 1, vertexDrawY + (j*sizeOnPixels) + 1, sizeOnPixels - 2, sizeOnPixels - 2 };
					switch (matrix[(relativeX + i)*CHUNK_SIZE + (relativeY + j)]){
					case GRASS:
						SDL_RenderCopy(renderer, sprMngr->getTexture("grass"), NULL, &rect);
						break;
					default:
						break;
					}
				}
			}
		}
		//spawnNeighbors(centerX_M, centerY_M, w_tiles, h_tiles, width_pixels, height_pixels, drawn, renderer, sprMngr);
	}
}


void Chunk::spawnNeighbors(SDL_Rect window) {
	if (!isCalled) {
		isCalled = true;
		for (int i = 0; i < 4; i++){
			if (rectInsideRect(window.x, window.y, window.w, window.h, (x + xval[i]) * CHUNK_SIZE, (y + yval[i]) * CHUNK_SIZE, CHUNK_SIZE, CHUNK_SIZE)) {
				if (exists->count(chunkUID(x + xval[i], y + yval[i])) != 0) {
					// search for it
				}
				else {
					Chunk * nei = getChunk(i);
					if (nei == NULL) {
						nei = new Chunk(x + xval[i], y + yval[i],exists);
						setChunk(i, nei);
						nei->setChunk((i + 2) % 4, this);
					}
					nei->spawnNeighbors(window);
				}
			}
		}
	}
}

void Chunk::resetCalls() {
	if (isCalled) {
		isCalled = false;
		for (int i = 0; i < 4; i++){
			Chunk * nei = getChunk(i);
			if (nei!=NULL) nei->resetCalls();
		}
	}
}