#include "Chunk.h"


Chunk::Chunk(int x, int y, std::set<unsigned>* exists){
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
	SDL_Log("CREATED CHUNK %d %d", x, y);
	this->exists->insert(chunkUID(x, y));
	
}

Chunk::Chunk(int x, int y, std::set<unsigned>* exists, Chunk *r, Chunk* b, Chunk *l, Chunk *t){
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
	SDL_Log("CREATED CHUNK %d %d", x, y);
	this->exists->insert(chunkUID(x, y));
}


Chunk::~Chunk(){
	int links = countLinks();
	SDL_Log("DELETED CHUNK %d %d", x, y);
	exists->erase(chunkUID(x, y));
	for (int i = 0; i < 4; ++i) {
		if (getChunk(i) != NULL) {
			if (getChunk(i) != NULL && exists->count(chunkUID(getChunk(i)->x, getChunk(i)->y)) == 1) {
				Chunk *tmp = getChunk(i);
				tmp->setChunk((i + 2) % 4, NULL);
				setChunk(i, NULL);
				delete tmp;
			}
		}
	}
	delete matrix;
	/* keep it maybe you will need it someday...
	if (links == 1) {
		// if i only have 1 neighbor just remove the link delete him and remove my link
		for (int i = 0; i < 4; ++i) {
			if (getChunk(i) != NULL) {
				getChunk(i)->setChunk((i + 2) % 4, NULL);
				delete getChunk(i);
				setChunk(i, NULL);
			}
		}
	}
	else if (links == 2) {
		// if i have 2 neighboors just cross my links to them delete them and delete me
		int a, b;
		a = b = -1;
		for (int i = 0; i < 4; ++i) {
			if (getChunk(i) != NULL) {
				if (a == -1) a = i;
				else b = i;
			}
		}
		getChunk(a)->setChunk((a + 2) % 4, getChunk(b));
		getChunk(b)->setChunk((b + 2) % 4, getChunk(a));
		delete getChunk(a);
		for (int i = 0; i < 4; ++i) {
			setChunk(i, NULL);
		}
	}
	else if (links == 3) {
		int a, b, c;
		a = b = c = -1;
		for (int i = 0; i < 4; ++i) {
			if (getChunk(i) != NULL) {
				if (a == -1) a = i;
				else if (b==-1) b = i;
				else c = i;
			}
		}
		getChunk(a)->setChunk((a + 2) % 4, getChunk(b));
		getChunk(b)->setChunk((b + 2) % 4, getChunk(a));
		delete getChunk(a);
		getChunk(c)->setChunk((c + 2) % 4, NULL);
		delete getChunk(c);
		for (int i = 0; i < 4; ++i) {
			setChunk(i, NULL);
		}
	}
	else if (links == 4) {
		// if i have 4 neighboors just make 2 crosses and delete them
		for (int i = 0; i < 2; ++i) {
			getChunk(i)->setChunk((i + 2) % 4, getChunk((i + 2) % 4));
			getChunk((i + 2) % 4)->setChunk(i, getChunk(i));	
		}
		delete getChunk(0);
		delete getChunk(1);
		for (int i = 0; i < 4; ++i) {
			setChunk(i, NULL);
		}
	}
	for (int i = 0; i < 4; ++i) {
		if (getChunk(i) != NULL) {
			getChunk(i)->setChunk((i + 2) % 4,NULL);
			delete getChunk(i);
		}
	}*/
	
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
			// is inside the screen window?
			if (rectInsideRect(window.x, window.y, window.w, window.h, (x + xval[i]) * CHUNK_SIZE, (y + yval[i]) * CHUNK_SIZE, CHUNK_SIZE, CHUNK_SIZE)) {
				Chunk * nei = getChunk(i);
				if (nei == NULL) {
					// does the chunk exists?
					if (exists->count(chunkUID(x + xval[i], y + yval[i])) != 0) {
						// search for it
						nei = search(x + xval[i], y + yval[i]);
					}
					else {
						// create id
						nei = new Chunk(x + xval[i], y + yval[i], exists);
					}
					// add links
					setChunk(i, nei);
					nei->setChunk((i + 2) % 4, this);
				}
				nei->spawnNeighbors(window);
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

Chunk* Chunk::search(int x, int y) {
	std::set<unsigned> visited;
	return _search(x, y, chunkUID(x, y), &visited);
}

// SHOULD BE OPTIMIZED!!!!
Chunk* Chunk::_search(int x, int y, unsigned uid, std::set<unsigned> *visited) {
	if (uid == chunkUID(this->x, this->y)) return this;
	else {
		Chunk * ret = NULL;
		for (int i = 0; i < 4 && ret == NULL; ++i) {
			Chunk * nei = getChunk(i);
			unsigned neiUID = chunkUID(this->x + xval[i], this->y + yval[i]);
			if (nei != NULL && visited->count(neiUID) == 0) {
				visited->insert(neiUID);
				ret = nei->_search(x, y, uid, visited);
			}
		}
		return ret;
	}
}

int Chunk::countLinks() {
	int count = 0;
	for (int i = 0; i < 4; ++i) {
		if (getChunk(i) != NULL) ++count;
	}
	return count;
}

void Chunk::debug() {
	std::cout << x << " " << y << " " << this << " " << top << " " << right << " " << bot << " " << left << std::endl;
}