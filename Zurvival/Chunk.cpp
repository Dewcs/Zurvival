#include "Chunk.h"

/*chunk constructor
parameters: x position / y position / list set if exist this chunk
create a chunk, asign x and y position, create a matrix pointer with chunk_size and put boolean isCalled false. put this chunk in set exists*/

#define coord(x,y) x*CHUNK_SIZE+y

Chunk::Chunk(int x, int y, std::set<unsigned>* exists){
	//save x and y cordinates
	this->x = x;
	this->y = y;
	//create a matrix of int's of the chunk
	matrix = new int[CHUNK_SIZE * CHUNK_SIZE];
	for (int i = 0; i < CHUNK_SIZE * CHUNK_SIZE; i++){
		matrix[i] = TILE_MAX;
	}
	randomChunk();

	//initializate all pointers of NULL
	right = NULL;
	bot= NULL;
	left = NULL;
	top = NULL;

	isCalled = false;

	this->exists = exists;
	SDL_Log("CREATED CHUNK %d %d", x, y);
	for (int i = 0; i < CHUNK_SIZE; ++i) {
		log(1, "%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d",
			matrix[coord(0,i)],
			matrix[coord(1, i)],
			matrix[coord(2, i)],
			matrix[coord(3, i)],
			matrix[coord(4, i)],
			matrix[coord(5, i)],
			matrix[coord(6, i)],
			matrix[coord(7, i)],
			matrix[coord(8, i)],
			matrix[coord(9, i)],
			matrix[coord(10, i)],
			matrix[coord(11, i)],
			matrix[coord(12, i)],
			matrix[coord(13, i)],
			matrix[coord(14, i)],
			matrix[coord(15, i)],
			matrix[coord(16, i)],
			matrix[coord(17, i)],
			matrix[coord(18, i)],
			matrix[coord(19, i)],
			matrix[coord(20, i)],
			matrix[coord(21, i)],
			matrix[coord(22, i)],
			matrix[coord(23, i)],
			matrix[coord(24, i)],
			matrix[coord(25, i)],
			matrix[coord(26, i)],
			matrix[coord(27, i)],
			matrix[coord(28, i)],
			matrix[coord(29, i)],
			matrix[coord(30, i)],
			matrix[coord(31, i)]
		);
	}
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
		matrix[i] = TILE_MAX;
	}
	randomChunk();
	//initializate all pointers
	right = r;
	bot = b;
	left = l;
	top = t;

	isCalled = false;

	this->exists = exists;
	SDL_Log("CREATED CHUNK %d %d", x, y);
	for (int i = 0; i < CHUNK_SIZE; ++i) {
		log(1, "%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d",
			matrix[coord(0, i)],
			matrix[coord(1, i)],
			matrix[coord(2, i)],
			matrix[coord(3, i)],
			matrix[coord(4, i)],
			matrix[coord(5, i)],
			matrix[coord(6, i)],
			matrix[coord(7, i)],
			matrix[coord(8, i)],
			matrix[coord(9, i)],
			matrix[coord(10, i)],
			matrix[coord(11, i)],
			matrix[coord(12, i)],
			matrix[coord(13, i)],
			matrix[coord(14, i)],
			matrix[coord(15, i)],
			matrix[coord(16, i)],
			matrix[coord(17, i)],
			matrix[coord(18, i)],
			matrix[coord(19, i)],
			matrix[coord(20, i)],
			matrix[coord(21, i)],
			matrix[coord(22, i)],
			matrix[coord(23, i)],
			matrix[coord(24, i)],
			matrix[coord(25, i)],
			matrix[coord(26, i)],
			matrix[coord(27, i)],
			matrix[coord(28, i)],
			matrix[coord(29, i)],
			matrix[coord(30, i)],
			matrix[coord(31, i)]
			);
	}
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
	//matrix = new int[CHUNK_SIZE * CHUNK_SIZE];
	for (int i = 0; i < CHUNK_SIZE; i+=2) {
		for (int j = 0; j < CHUNK_SIZE; j += 2) {
			int p1, p2;
			if (j > 0) p1 = matrix[coord(i, (j - 2))];
			else p1 = rand()%TILE_MAX;
			if (i > 0) p2 = matrix[coord((i - 2), j)];
			else p2 = rand() % TILE_MAX;
			if (rand() % 10 == 0) matrix[coord(i, j)] = rand() % TILE_MAX;
			else if (rand() % 2 == 0) matrix[coord(i, j)] = p1;
			else matrix[coord(i, j)] = p2;
			//log(1, "%d %d %d %d %d %d %d", i, j, p1, p2, matrix[coord(i, j)], coord((i - 2), j), coord(i, (j - 2)));
		}
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

void Chunk::drawChunk(double centerX_M, double  centerY_M, int  width_pixels, int height_pixels, unsigned *drawn, SDL_Renderer* renderer, SpriteManager* sprMngr, TileManager *tm){
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
						SDL_Rect rect = { vertexDrawX + (i*sizeOnPixels), vertexDrawY + (j*sizeOnPixels), sizeOnPixels, sizeOnPixels };
						if (getMapValue(relativeX + i, relativeY + j) == TILE_MAX) {
							if (((relativeX + i) + (relativeY + j)) % 2 == 0) {
								// all
								SDL_RenderCopy(
									renderer,
									tm->get(
										getMapValue(relativeX + i - 1, relativeY + j - 1),
										getMapValue(relativeX + i + 1, relativeY + j - 1),
										getMapValue(relativeX + i + 1, relativeY + j + 1),
										getMapValue(relativeX + i - 1, relativeY + j + 1),
										renderer
									), 
									NULL,
									&rect
								);
								//sprMngr->drawNumber(getMapValue(relativeX + i - 1, relativeY + j - 1) * 1000 + getMapValue(relativeX + i + 1, relativeY + j - 1) * 100 + getMapValue(relativeX + i + 1, relativeY + j + 1) * 10 + getMapValue(relativeX + i - 1, relativeY + j + 1), "red_numbers", rect.x, rect.y, rect.h/4, ALIGN_LEFT);
							}
							else {
								// else
								SDL_RenderCopy(
									renderer,
									tm->get(
										getMapValue(relativeX + i, relativeY + j - 1),
										getMapValue(relativeX + i + 1, relativeY + j),
										getMapValue(relativeX + i, relativeY + j + 1),
										getMapValue(relativeX + i - 1, relativeY + j),
										renderer
									),
									NULL,
									&rect
									);
								//sprMngr->drawNumber(getMapValue(relativeX + i, relativeY + j - 1) * 1000 + getMapValue(relativeX + i + 1, relativeY + j) * 100 + getMapValue(relativeX + i, relativeY + j + 1) * 10 + getMapValue(relativeX + i - 1, relativeY + j), "red_numbers", rect.x, rect.y, rect.h/4, ALIGN_LEFT);
							}
						}
						else {
							SDL_RenderCopy(renderer, tm->get(getMapValue(relativeX + i, relativeY + j)), NULL, &rect);
							//sprMngr->drawNumber(getMapValue(relativeX + i, relativeY + j), "red_numbers", rect.x, rect.y, rect.h, ALIGN_LEFT);
						}
						/*switch (matrix[(relativeX + i)*CHUNK_SIZE + (relativeY + j)]){
						case TILE_GRASS:
							SDL_RenderCopy(renderer, sprMngr->getTexture("tile_grass"), NULL, &rect);
							break;
						case TILE_GRASS2:
							SDL_RenderCopy(renderer, sprMngr->getTexture("tile_grass2"), NULL, &rect);
							break;
						case TILE_ROCK:
							SDL_RenderCopy(renderer, sprMngr->getTexture("tile_rock"), NULL, &rect);
							break;
						case TILE_WOOD:
							SDL_RenderCopy(renderer, sprMngr->getTexture("tile_wood"), NULL, &rect);
							break;
						case TILE_SNOW:
							SDL_RenderCopy(renderer, sprMngr->getTexture("tile_snow"), NULL, &rect);
							break;
						case TILE_METAL:
							SDL_RenderCopy(renderer, sprMngr->getTexture("tile_metal"), NULL, &rect);
							break;
						case TILE_WATER:
							SDL_RenderCopy(renderer, sprMngr->getTexture("tile_water"), NULL, &rect);
							break;
						default:
							break;
						}*/
					}
				}
			}
			// fem un for pels 4 veins i per cada vei que no sigui NULL, tornem a cridar la funcio drawChunk
			for (int i = 0; i < 4; i++){
				Chunk * nei = getChunk(i);
				if (nei != NULL){
					nei->drawChunk(centerX_M, centerY_M, width_pixels, height_pixels, drawn, renderer, sprMngr, tm);
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
			if (rectInsideRect(window.x, window.y, window.w, window.h, (x + xval[i]) * CHUNK_SIZE - 2, (y + yval[i]) * CHUNK_SIZE - 2, CHUNK_SIZE + 4, CHUNK_SIZE + 4)) {
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
						for (int j = 0; j < 10; j++){
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
	// if i'm the searched chunk return my pointer
	if (uid == chunkUID(this->x, this->y)) return this;
	else {
		// otherwise search if my neigboors are
		Chunk * ret = NULL;
		for (int i = 0; i < 4 && ret == NULL; ++i) {
			Chunk * nei = getChunk(i);
			unsigned neiUID = chunkUID(this->x + xval[i], this->y + yval[i]);
			// check if neigboors where called before
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

int Chunk::getMapValue(int x, int y) {
	int ret = 7;
	if (x >= 0 && x < CHUNK_SIZE && y >= 0 && y < CHUNK_SIZE) {
		// the position is inside the chunk
		ret = matrix[coord(x,y)];
	}
	else if (x < 0) {
		if (left == NULL) left = search(this->x - 1, this->y);
		if (left != NULL) ret = left->getMapValue(CHUNK_SIZE + x, y);
	}
	else if (x >= CHUNK_SIZE) {
		if (right == NULL) right = search(this->x + 1, this->y);
		if (right != NULL) ret = right->getMapValue(x - CHUNK_SIZE, y);
	}
	else if (y < 0) {
		if (top == NULL) top = search(this->x, this->y-1);
		if (top != NULL) ret = top->getMapValue(x, CHUNK_SIZE + y);
	}
	else if (y >= CHUNK_SIZE) {
		if (bot == NULL) bot = search(this->x, this->y + 1);
		if (bot != NULL)ret = bot->getMapValue(x, y - CHUNK_SIZE);
	}
	return ret;
}

