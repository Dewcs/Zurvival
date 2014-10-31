#pragma once
#include <SDL.h>
#include <math.h>
#include <algorithm>
#include "tinyxml2.h"
#define OUTF_SIZE 11
#define INF_SIZE 9
static unsigned int g_seed = 0;

//Used to seed the generator.

inline void fast_srand(int seed) {
	g_seed = seed;
}

//fastrand routine returns one integer, similar output value range as C lib.

inline unsigned fastrand() {
	g_seed = (214013 * g_seed + 2531011);
	return (g_seed >> 16) & 0x7FFF;
}

// input functions

float isum_all(float* values, unsigned* ids, unsigned size);

float irest(float* values, unsigned* ids, unsigned size);

float irest2(float* values, unsigned* ids, unsigned size);

float imul(float* values, unsigned* ids, unsigned size);

float idiv(float* values, unsigned* ids, unsigned size);

float imin(float* values, unsigned* ids, unsigned size);

float imax(float* values, unsigned* ids, unsigned size);

float imed(float* values, unsigned* ids, unsigned size);

float iavg(float* values, unsigned* ids, unsigned size);

// output functions 

float onone(float value);

float osqrt(float value);

float oneg(float value);

float ozero(float value);

float oceil(float value);

float oround(float value);

float ofloor(float value);

float oabs(float value);

float osgn(float value);

float osin(float value);

float ocos(float value);


static float(*in_f[INF_SIZE])(float* values, unsigned* ids, unsigned size) = {
	isum_all,
	irest,
	irest2,
	imul,
	idiv,
	imin,
	imax,
	imed,
	iavg
};

static float(*out_f[OUTF_SIZE])(float value) = {
	onone,
	osqrt,
	oneg,
	ozero,
	oceil,
	oround,
	ofloor,
	oabs,
	osgn,
	ocos,
	osin
};

enum infunc {
	SUM_ALL,
	REST,
	REST2,
	MUL,
	DIV,
	MIN,
	MAX,
	MED,
	AVG
};

enum outfunc {
	NONE,
	SQRT,
	NEG,
	ZERO,
	CEIL,
	ROUND,
	FLOOR,
	ABS,
	SGN,
	SIN,
	COS
};

enum changes_t {
	CHANGE_NONE,
	CHANGE_INF,
	CHANGE_OUTF,
	CHANGE_LINK,
	REMOVE_NODE,
	REMOVE_LINK,
	ADD_NODE,
	ADD_LINK,
	SHUFFLE_LINKS,
	CHANGES_MAX
};

struct Node{
	infunc in;
	outfunc out;
	unsigned size;
	unsigned *ids;
};

class Brain {
private:
	unsigned input;
	unsigned output;
	unsigned hidden;
	unsigned size;
	float *values;
	Node *nodes;
public:
	Brain(unsigned, unsigned);
	~Brain();
	Brain* copy();
	void tweak();
	void randomize();
	void setInput(float*);
	void setValues(unsigned, float*, Node*);
	void evaluate();
	void getResult(float*&, unsigned&);
	void store(const char* fname);
	void load(const char* fname);
};

