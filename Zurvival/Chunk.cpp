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
	int xval[4] = {1,0,-1,0};
	int yval[4] = { 0, 1, 0, -1 };
	for (int i = 0; i < 4; i++){
		if (rectInsideRect(window.x, window.y, window.w, window.h, (x + xval[i]) * CHUNK_SIZE, (y + yval[i]) * CHUNK_SIZE, CHUNK_SIZE, CHUNK_SIZE)) {
			if (getChunk(i) == NULL) {
				setChunk(i, new Chunk(x + xval[i], y + yval[i]));
				getChunk(i)->setChunk((i + 2) % 4, this);
			} 
			getChunk(i)->spawnNeighbors(window);
		}
	}
		/*if (i == 0){
			if () {
				Chunk *le = new Chunk((x - 1), y);
				le->left = right;
				right = le->left;
				le->drawNeighbord((x - 1), y, centerX_M, centerY_M, width_pixels, height_pixels, w_tiles, h_tiles, drawn, renderer, sprMngr );
				//le->spawnNeighbors(centerX_M, centerY_M, w_tiles, h_tiles, width_pixels, height_pixels, drawn, renderer, sprMngr);
			}
		}else if (i == 1){
			if (rectInsideRect(floor(centerX_M - (w_tiles / 2)), floor(centerY_M - (h_tiles / 2)), w_tiles, h_tiles, x * CHUNK_SIZE, (y-1) * CHUNK_SIZE, CHUNK_SIZE, CHUNK_SIZE)) {
				Chunk *to = new Chunk(x ,(y - 1));
				to->top = bot;
				bot = to->top;
				to->drawNeighbord(x, (y - 1), centerX_M, centerY_M, width_pixels, height_pixels, w_tiles, h_tiles, drawn, renderer, sprMngr);
				//to->spawnNeighbors(centerX_M, centerY_M, w_tiles, h_tiles, width_pixels, height_pixels, drawn, renderer, sprMngr);
			}
		}else if (i == 2){
			if (rectInsideRect(floor(centerX_M - (w_tiles / 2)), floor(centerY_M - (h_tiles / 2)), w_tiles, h_tiles, (x+1) * CHUNK_SIZE, y * CHUNK_SIZE, CHUNK_SIZE, CHUNK_SIZE)) {
				Chunk *ri = new Chunk((x + 1), y);
				ri->right = left;
				left = ri->right;
				ri->drawNeighbord((x + 1), y, centerX_M, centerY_M, width_pixels, height_pixels, w_tiles, h_tiles, drawn, renderer, sprMngr);
				//ri->spawnNeighbors(centerX_M, centerY_M, w_tiles, h_tiles, width_pixels, height_pixels, drawn, renderer, sprMngr);
			}
		}else{
			if (rectInsideRect(floor(centerX_M - (w_tiles / 2)), floor(centerY_M - (h_tiles / 2)), w_tiles, h_tiles, x * CHUNK_SIZE, (y+1) * CHUNK_SIZE, CHUNK_SIZE, CHUNK_SIZE)) {
				Chunk *bo = new Chunk(x, (y + 1));
				bo->bot = top;
				top = bo->bot;
				bo->drawNeighbord(x, (y + 1), centerX_M, centerY_M, width_pixels, height_pixels, w_tiles, h_tiles, drawn, renderer, sprMngr);
				//bo->spawnNeighbors(centerX_M, centerY_M, w_tiles, h_tiles, width_pixels, height_pixels, drawn, renderer, sprMngr);
			}
		}
	}*/
}

/*void Chunk::drawNeighbord(int xN, int yN, double centerX_M, double  centerY_M, int  width_pixels, int height_pixels, int w_tiles, int  h_tiles, unsigned *drawn, SDL_Renderer* renderer, SpriteManager* sprMngr){
	int sizeOnPixels = height_pixels / TILE_FOR_HEIGHT;


	SDL_Rect rectToDraw = rectIntersect({ xN * CHUNK_SIZE, yN * CHUNK_SIZE, CHUNK_SIZE, CHUNK_SIZE }, { floor(centerX_M - (w_tiles / 2)), floor(centerY_M - (h_tiles / 2)), w_tiles, h_tiles });
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
		for (int j = 0; j <= rectToDraw.h; j++){
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
}*/