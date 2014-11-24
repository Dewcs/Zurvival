#include "Chunk.h"

/*chunk constructor
parameters: x position / y position / list set if exist this chunk
create a chunk, asign x and y position, create a matrix pointer with chunk_size and put boolean isCalled false. put this chunk in set exists*/

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

/*chunk constructor 2
parameters: x position / y position / list set if exist this chunk / four pointers, right, bot, left, and top
create a chunk, asign x and y position, create a matrix pointer with chunk_size, asign pointer neighbords to foru pointers and put boolean isCalled false. put this chunk in set exists*/

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

/*chunk destructor
delete chunk and all neighbord links
*/
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
}

/*randomChunk()
randomize all cells in the matrix chunk*/

void Chunk::randomChunk(){
	matrix = new int[CHUNK_SIZE * CHUNK_SIZE];
	for (int i = 0; i < CHUNK_SIZE * CHUNK_SIZE; i++){
		matrix[i] = rand() % 10;
	}
}

/*setChunk
parameters: int id chunk / pointer chunk
igualate neighbord pointers of this chunk*/

void Chunk::setChunk(int id, Chunk * chunk){
	if (id == 0) top=chunk;
	else if (id == 1) right = chunk;
	else if (id == 2) bot = chunk;
	else if (id == 3) left = chunk;
}

/*getChunk
parameters: int id chunk
return one neighbord of this chunk*/

Chunk* Chunk::getChunk(int id){
	if (id == 0) return top;
	else if (id == 1) return right;
	else if (id == 2) return bot;
	else if(id == 3) return left;
	return NULL;
}

/*areDiferentChunk
parameters; int x position, int y position
return true if this x and y cordinates are diferents of this chunk*/

bool Chunk::areDiferentChunk(int x, int y){
	if (this->x != x || this->y != y){
		return true;
	}
	else return false;
}

/*drawChunk
parameters: double center map x / double center map y / int width in pixels / int height in pixels / sdl renderer / pointer sprite manager
draw chunk and draw neighbord if i need*/

void Chunk::drawChunk(double centerX_M, double  centerY_M, int  width_pixels, int height_pixels, unsigned *drawn, SDL_Renderer* renderer, SpriteManager* sprMngr){
	if (!isCalled) { //comprovem que el chunk ha sigut dibuixat
		isCalled = true; //com no ha sigut dibuixat i hem entrat posem el seu boolea a true
		//SDL_Log("DRAWN CHUNK %d %d", x, y);
		//creem una variable que sigui la width i el height d'un tile en pixels
		int sizeOnPixels = height_pixels / TILE_FOR_HEIGHT;
		//creem una variable que sigui la height de tiles
		int h_tiles = TILE_FOR_HEIGHT;

		//el mateix per la width de tiles
		int w_tiles = ceil(width_pixels / (double)(height_pixels / TILE_FOR_HEIGHT));

		//comprovem si hem de pintar el chunk
		if (rectInsideRect(floor(centerX_M - (w_tiles / 2)), floor(centerY_M - (h_tiles / 2)), w_tiles, h_tiles, x * CHUNK_SIZE, y * CHUNK_SIZE, CHUNK_SIZE, CHUNK_SIZE)) {

			//creem un rectancle del tros de chunk que hem de pintar en pantalla
			SDL_Rect rectToDraw = rectIntersect({ x * CHUNK_SIZE, y * CHUNK_SIZE, CHUNK_SIZE, CHUNK_SIZE }, { floor(centerX_M - (w_tiles / 2)), floor(centerY_M - (h_tiles / 2)), w_tiles, h_tiles });

			//crrem dues variables que ens guardin la distancia en pixels des del centre fins al vertex del rectangle que hem de pintar
			int distInPixelsX = (rectToDraw.x - centerX_M)*sizeOnPixels;
			int distInPixelsY = (rectToDraw.y - centerY_M)*sizeOnPixels;

			//creem dues variables que siguin la posicio en pixels del nostre vertex
			int vertexDrawX = (width_pixels / 2) + distInPixelsX;
			int vertexDrawY = (height_pixels / 2) + distInPixelsY;
			int relativeX = ((rectToDraw.x % CHUNK_SIZE) + CHUNK_SIZE) % CHUNK_SIZE;
			int relativeY = ((rectToDraw.y % CHUNK_SIZE) + CHUNK_SIZE) % CHUNK_SIZE;

			//fer dos bucles per recorrer el rectangle que hem de pintar 
			for (int i = 0; i <= rectToDraw.w; i++){
				for (int j = 0; j <= rectToDraw.h; j++){
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
			// fem un for pels 4 veins i per cada vei que no sigui NULL, tornem a cridar la funcio drawChunk
			for (int i = 0; i < 4; i++){
				Chunk * nei = getChunk(i);
				if (nei != NULL){
					nei->drawChunk(centerX_M, centerY_M, width_pixels, height_pixels, drawn, renderer, sprMngr);
				}
			}
		}
	}
	
}

/*spawnNeighbords
parameters: window
create a neighbords if i need*/

void Chunk::spawnNeighbors(SDL_Rect window, ItemMap * itemap) {
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
						for (int j = 0; j < 2; j++){
							itemap->addRandomItem((double)nei->x, (double)nei->y, (double)CHUNK_SIZE, (double)CHUNK_SIZE);
						}
					}
					// add links
					setChunk(i, nei);
					nei->setChunk((i + 2) % 4, this);
				}
				//call spawnNeighbords whith neibord
				nei->spawnNeighbors(window, itemap);
			}
		}
	}
}

/*resetCalls
transform all booleans of chunk of false*/

void Chunk::resetCalls() {
	if (isCalled) {
		isCalled = false;
		for (int i = 0; i < 4; i++){
			Chunk * nei = getChunk(i);
			if (nei!=NULL) nei->resetCalls();
		}
	}
}

/*serch
parameters: int x position / int y position
return chunk alocated in this position*/

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

/*debug
print in console all information of this chunk*/

void Chunk::debug() {
	std::cout << x << " " << y << " " << this << " " << top << " " << right << " " << bot << " " << left << std::endl;
}

