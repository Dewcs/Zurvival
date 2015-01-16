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
// sum all inputs
float isum_all(const std::vector<float> &values, const std::vector<unsigned> &ids, unsigned size);

// rest all inputs -in[0]-in[2]...-in[n]
float irest(const std::vector<float> &values, const std::vector<unsigned> &ids, unsigned size);
// rest in[0]-in[2]...-in[n]
float irest2(const std::vector<float> &values, const std::vector<unsigned> &ids, unsigned size);
// multiply all inputs
float imul(const std::vector<float> &values, const std::vector<unsigned> &ids, unsigned size);
// divide all inputs avoiding zeros
float idiv(const std::vector<float> &values, const std::vector<unsigned> &ids, unsigned size);
// return the minimum value among the inputs
float imin(const std::vector<float> &values, const std::vector<unsigned> &ids, unsigned size);
// returns the maximum value among the inputs
float imax(const std::vector<float> &values, const std::vector<unsigned> &ids, unsigned size);
// returns the median among the inputs
float imed(const std::vector<float> &values, const std::vector<unsigned> &ids, unsigned size);
// returns the average of the inputs
float iavg(const std::vector<float> &values, const std::vector<unsigned> &ids, unsigned size);

// output functions 
// do nothing to the input
float onone(float value);
// returns the square root
float osqrt(float value);
// multiplies the number by -1
float oneg(float value);
// returs 1 if the value is 0, 0 otherwise
float ozero(float value);
// returns the ceil of the number
float oceil(float value);
// returns the round of the number
float oround(float value);
// returns the floor of the number
float ofloor(float value);
// returns the abs of the number
float oabs(float value);
// returns 1 if the number of positive -1 if negative
float osgn(float value);
// returns the sin of the number
float osin(float value);
// returns the cos of the number 
float ocos(float value);

// list of input function
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

// list of output functions
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
// links to input functions
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
// links to output functions
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
// define different type of mutations
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

// definition of a node
struct Node{
	infunc in; // id of the input function
	outfunc out; // id of the output functions
	//unsigned size;
	std::vector<unsigned> ids; // array of links
};

class Brain {
private:
	unsigned input; // input nodes length
	unsigned output; // output nodes length
	unsigned hidden; // hidden nodes length
	unsigned size; // total length of nodes
	std::vector<float> values; // array of values
	std::vector<Node> nodes; // array of nodes
public:
	Brain(unsigned input, unsigned output);
	~Brain();
	Brain* copy(); // make a copy of 1 brain
	void tweak(); // tweak the brain to perform a mutation
	void randomize(); // randomize a brand new brain
	void setInput(const std::vector<float> &input); // set input to brain
	void setValues(unsigned hidden, const std::vector<float> &values, const std::vector<Node> &nodes); // used to copy brains
	void evaluate(); // evaluate to get the output
	void getResult(std::vector<float> &output); // copy the results of an evaluation to output
	void store(const char* fname); // store the brain inside a file
	void load(const char* fname); // load a brain from a file
	void print(); // print the brain in console
	void optimize(); // optimize the brain to remove unused nodes
};

