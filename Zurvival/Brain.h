#pragma once
#include <math.h>
#include <vector>
#include <algorithm>
#include <cmath>
#include <new>
#include "tinyxml2.h"
#include "Functions.h"
static unsigned int g_seed = 0;
#define MAX_VALUE 1000000
#define MIN_VALUE 0.000001
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

float isum_all(const std::vector<float> &values, const std::vector<unsigned> &ids, unsigned size);

float irest(const std::vector<float> &values, const std::vector<unsigned> &ids, unsigned size);

float irest2(const std::vector<float> &values, const std::vector<unsigned> &ids, unsigned size);

float imul(const std::vector<float> &values, const std::vector<unsigned> &ids, unsigned size);

float idiv(const std::vector<float> &values, const std::vector<unsigned> &ids, unsigned size);

float imin(const std::vector<float> &values, const std::vector<unsigned> &ids, unsigned size);

float imax(const std::vector<float> &values, const std::vector<unsigned> &ids, unsigned size);

float imed(const std::vector<float> &values, const std::vector<unsigned> &ids, unsigned size);

float iavg(const std::vector<float> &values, const std::vector<unsigned> &ids, unsigned size);

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

enum infunc {
	SUM_ALL,
	REST,
	REST2,
	MUL,
	DIV,
	MIN,
	MAX,
	MED,
	AVG,
	INF_SIZE
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
	COS,
	OUTF_SIZE
};

static float(*in_f[INF_SIZE])(const std::vector<float> &values, const std::vector<unsigned> &ids, unsigned size) = {
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
	osin,
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
	//unsigned size;
	std::vector<unsigned> ids;
};

class Brain {
private:
	unsigned input;
	unsigned output;
	unsigned hidden;
	unsigned size;
	std::vector<float> values;
	std::vector<Node> nodes;
public:
	Brain(unsigned input, unsigned output);
	~Brain();
	Brain* copy();
	void tweak();
	void randomize();
	void setInput(const std::vector<float> &input);
	void setValues(unsigned hidden, const std::vector<float> &values, const std::vector<Node> &nodes);
	void evaluate();
	void getResult(std::vector<float> &output);
	void store(const char* fname);
	void load(const char* fname);
	void print();
	void optimize();
};

