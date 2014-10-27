#include "Brain.h"

Brain::Brain(unsigned input, unsigned output) {
	this->input = input;
	this->output = output;
}


Brain::~Brain() {
	for (unsigned i = 0; i < output + hidden; ++i) {
		delete nodes[i].ids;
	}
	delete nodes;
	delete values;
}

Brain* Brain::copy() {
	Brain *c = new Brain(input, output);
	c->setValues(hidden, values, nodes);
	return c;
}

void Brain::tweak() {

}

void Brain::randomize() {
	hidden = fastrand() % ((input + output) / 2) + 1;
	size = input + 5 + output + hidden;
	values = new float[size];
	nodes = new Node[output + hidden];
	for (unsigned i = 0; i < output + hidden; ++i) {
		nodes[i].in = infunc(fastrand() % INF_SIZE);
		nodes[i].out = outfunc(fastrand() % OUTF_SIZE);
		unsigned size2 = fastrand() % (input + 4 + i) + 1;
		nodes[i].size = size2;
		nodes[i].ids = new unsigned[size2];
		for (unsigned j = 0; j < size2; ++j) {
			nodes[i].ids[j] = fastrand() % (input + 5 + i);
		}
	}
}
void Brain::setInput(float* input) {
	for (unsigned i = 0; i < this->input; ++i) {
		values[i] = input[i];
	}
	values[this->input] = -1;
	values[this->input + 1] = 0;
	values[this->input + 2] = 1;
	values[this->input + 3] = (float)this->input;
	values[this->input + 4] = (float)this->output;
}
void Brain::setValues(unsigned hidden, float *values, Node* nodes) {
	this->hidden = hidden;
	size = input + 5 + output + hidden;
	this->values = new float[size];
	this->nodes = new Node[output + hidden];
	for (unsigned i = 0; i < output + hidden; ++i) {
		this->nodes[i].in = nodes[i].in;
		this->nodes[i].out = nodes[i].out;
		this->nodes[i].size = nodes[i].size;
		this->nodes[i].ids = new unsigned[nodes[i].size];
		for (unsigned j = 0; j < nodes[i].size; ++j) {
			this->nodes[i].ids[j] = nodes[i].ids[j];
		}
	}
}
void Brain::evaluate() {
	for (unsigned i = 0; i < output + hidden; ++i) {
		values[i + 5 + input] = out_f[nodes[i].out](in_f[nodes[i].in](values, nodes[i].ids, nodes[i].size));
	}
}
void Brain::getResult(float* &arr, unsigned &size) {
	arr = new float[output];
	size = output;
	for (unsigned i = 0; i < size; ++i) arr[i] = values[input + 5 + hidden + i];
}

float isum_all(float* values, unsigned* ids, unsigned size) {
	float sum = 0;
	for (unsigned i = 0; i < size; ++i) sum += values[ids[i]];
	return sum;
}

float irest(float* values, unsigned* ids, unsigned size) {
	float ret = values[ids[0]];
	for (unsigned i = 1; i < size; ++i) ret -= values[ids[i]];
	return ret;
}

float irest2(float* values, unsigned* ids, unsigned size) {
	float ret = -values[ids[0]];
	for (unsigned i = 1; i < size; ++i) ret -= values[ids[i]];
	return ret;
}

float imul(float* values, unsigned* ids, unsigned size) {
	float ret = 1;
	for (unsigned i = 0; i < size; ++i) ret *= values[ids[i]];
	return ret;
}

float idiv(float* values, unsigned* ids, unsigned size) {
	float ret = values[ids[0]];
	for (unsigned i = 1; i < size; ++i) {
		if (values[ids[i]] != 0) ret /= values[ids[i]];
	}
	return ret;
}

float imin(float* values, unsigned* ids, unsigned size) {
	float min = values[ids[0]];
	for (unsigned i = 1; i < size; ++i) {
		if (min>values[ids[i]]) {
			min = values[ids[i]];
		}
	}
	return min;
}

float imax(float* values, unsigned* ids, unsigned size) {
	float max = values[ids[0]];
	for (unsigned i = 1; i < size; ++i) {
		if (max<values[ids[i]]) {
			max = values[ids[i]];
		}
	}
	return max;
}

float imed(float* values, unsigned* ids, unsigned size) {
	float *tmp = new float[size];
	for (unsigned i = 0; i < size; ++i) {
		tmp[i] = values[ids[i]];
	}
	std::sort(tmp, tmp + size);
	float ret = tmp[size / 2];
	if (size % 2 == 0) {
		ret = (ret + tmp[(size / 2) + 1]) / 2;
	}
	delete tmp;
	return ret;
}

float iavg(float* values, unsigned* ids, unsigned size) {
	float sum = 0;
	for (unsigned i = 0; i < size; ++i) sum += values[ids[i]];
	return sum / size;
}

// output functions 

float onone(float value) {
	return value;
}

float osqrt(float value) {
	if (value>0) return sqrt(value);
	else return 0;
}

float oneg(float value) {
	return value*-1;
}

float ozero(float value) {
	if (value == 0) return 1;
	else return 0;
}

float oceil(float value) {
	return ceil(value);
}

float oround(float value) {
	return round(value);

}

float ofloor(float value) {
	return floor(value);
}

float oabs(float value) {
	return fabs(value);
}

float osgn(float value) {
	if (value > 0) return 1;
	else if (value < 0) return -1;
	else return 0;
}

float osin(float value) {
	return sin(value);
}

float ocos(float value) {
	return cos(value);
}

