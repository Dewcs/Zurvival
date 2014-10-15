#pragma once
#define CHUNK_SIZE 32

class Chunk
{
	int *matrix;
	int x;
	int y;
	Chunk *left;
	Chunk *right;
	Chunk *top;
	Chunk *bot;
public:
	Chunk(int,int);
	Chunk(int, int, Chunk* r, Chunk* b, Chunk* l , Chunk* t);
	~Chunk();
	void RandomChunk();
	void SetChunk(int id,Chunk *chunk);
	Chunk* GetChunk(int id);
};

