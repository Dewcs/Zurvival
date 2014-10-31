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
	changes_t change = (changes_t)(fastrand() % CHANGES_MAX);
	unsigned nodeid = fastrand() % (output + hidden);
	unsigned linkid = fastrand() % (nodes[nodeid].size);
	Node *tmpnodes = NULL;
	unsigned *tmpids = NULL;
	unsigned next = 0;
	unsigned a, b, tmp;
	switch (change)
	{
	case CHANGE_NONE:
		break;
	case CHANGE_INF:
		nodes[nodeid].in = infunc(fastrand() % INF_SIZE);
		break;
	case CHANGE_OUTF:
		nodes[nodeid].out = outfunc(fastrand() % OUTF_SIZE);
		break;
	case CHANGE_LINK:
		nodes[nodeid].ids[linkid] = fastrand() % (input + 5 + linkid);
		break;
	case REMOVE_NODE:
		SDL_Log("BEGIN REMOVE NODE");
		--hidden;
		size = input + 5 + output + hidden;
		values = new float[size];
		tmpnodes = new Node[output + hidden];
		for (unsigned i = 0; i < nodeid; ++i) {
			// copy before nodeid values
			tmpnodes[i].in = nodes[i].in;
			tmpnodes[i].out = nodes[i].out;
			tmpnodes[i].size = nodes[i].size;
			tmpnodes[i].ids = new unsigned[nodes[i].size];
			for (unsigned j = 0; j < tmpnodes[i].size; ++j) {
				tmpnodes[i].ids[j] = nodes[i].ids[j];
			}
			delete nodes[i].ids;
		}
		for (unsigned i = nodeid; i < output + hidden; ++i) {
			// copy after nodeid values
			tmpnodes[i].in = nodes[i + 1].in;
			tmpnodes[i].out = nodes[i + 1].out;
			tmpnodes[i].size = nodes[i + 1].size;
			tmpnodes[i].ids = new unsigned[nodes[i+1].size];
			next = 0; // counter for next element to insert
			for (unsigned j = 0; j < nodes[i + 1].size; ++j) {
				if (nodes[i + 1].ids[j]<nodeid) tmpnodes[i].ids[next++] = nodes[i + 1].ids[j]; // no problem
				else if (nodes[i + 1].ids[j]>nodeid) tmpnodes[i].ids[next++] = nodes[i + 1].ids[j] - 1; // subratct 1 position
				else {
					// remove link
					--tmpnodes[i].size;
					tmpids = new unsigned[tmpnodes[i].size];
					for (unsigned k = 0; k < next; ++k) {
						tmpids[k] = tmpnodes[i].ids[k];
					}
					delete tmpnodes[i].ids;
					tmpnodes[i].ids = tmpids;
				}
			}
			delete nodes[i+1].ids;
		}
		delete nodes;
		nodes = tmpnodes;
		SDL_Log("END REMOVE NODE");
		break;
	case REMOVE_LINK:
		SDL_Log("BEGIN REMOVE LINK");
		nodes[nodeid].size -= 1;
		tmpids = new unsigned[nodes[nodeid].size];
		for (unsigned j = 0; j < linkid; ++j) {
			tmpids[j] = nodes[nodeid].ids[j];
		}
		for (unsigned j = linkid; j < nodes[nodeid].size; ++j) {
			tmpids[j] = nodes[nodeid].ids[j + 1];
		}
		delete nodes[nodeid].ids;
		nodes[nodeid].ids = tmpids;
		SDL_Log("END REMOVE LINK");
		break;
	case ADD_NODE:
		SDL_Log("BEGIN ADD NODE");
		++hidden;
		size = input + 5 + output + hidden;
		values = new float[size];
		tmpnodes = new Node[output + hidden];
		// pre nodes
		for (unsigned i = 0; i < nodeid; ++i) {
			// copy before nodeid values
			tmpnodes[i].in = nodes[i].in;
			tmpnodes[i].out = nodes[i].out;
			tmpnodes[i].size = nodes[i].size;
			tmpnodes[i].ids = new unsigned[nodes[i].size];
			for (unsigned j = 0; j < tmpnodes[i].size; ++j) {
				tmpnodes[i].ids[j] = nodes[i].ids[j];
			}
			delete nodes[i].ids;
		}
		// node
		tmpnodes[nodeid].in = infunc(fastrand() % INF_SIZE);
		tmpnodes[nodeid].out = outfunc(fastrand() % OUTF_SIZE);
		tmpnodes[nodeid].size = fastrand() % (input + 4 + nodeid) + 1;
		tmpnodes[nodeid].ids = new unsigned[tmpnodes[nodeid].size];
		for (unsigned j = 0; j < tmpnodes[nodeid].size; ++j) {
			tmpnodes[nodeid].ids[j] = fastrand() % (input + 5 + nodeid);
		}
		// post nodes
		for (unsigned i = nodeid; i < output + hidden - 1; ++i) {
			// copy after nodeid values
			tmpnodes[i+1].in = nodes[i].in;
			tmpnodes[i+1].out = nodes[i].out;
			tmpnodes[i+1].size = nodes[i].size;
			tmpnodes[i+1].ids = new unsigned[nodes[i].size];
			next = 0; // counter for next element to insert
			for (unsigned j = 0; j < nodes[i].size; ++j) {
				if (nodes[i].ids[j]<nodeid) tmpnodes[i+1].ids[j] = nodes[i].ids[j]; // no problem
				else tmpnodes[i+1].ids[j] = nodes[i].ids[j] + 1; // add 1 position
			}
			// lets link
			if (fastrand() % (input + 5 + nodeid) == 0) {
				// copy current
				tmpids = new unsigned[tmpnodes[i + 1].size + 1];
				for (unsigned k = 0; k < tmpnodes[i + 1].size; ++k) {
					tmpids[k] = tmpnodes[i + 1].ids[k];
				}
				// add new
				tmpids[tmpnodes[i + 1].size] = nodeid;
				delete tmpnodes[i + 1].ids;
				tmpnodes[i + 1].size += 1;
				tmpnodes[i + 1].ids = tmpids;
			}
			delete nodes[i].ids;
		}
		
		delete nodes;
		nodes = tmpnodes;
		SDL_Log("END ADD NODE");
		break;
	case ADD_LINK:
		SDL_Log("BEGIN ADD LINK");
		nodes[nodeid].size += 1;
		tmpids = new unsigned[nodes[nodeid].size];
		for (unsigned j = 0; j < linkid; ++j) {
			tmpids[j] = nodes[nodeid].ids[j];
		}
		tmpids[linkid] = fastrand() % (input + 5 + nodeid);
		for (unsigned j = linkid; j < nodes[nodeid].size; ++j) {
			tmpids[j + 1] = nodes[nodeid].ids[j];
		}
		delete nodes[nodeid].ids;
		nodes[nodeid].ids = tmpids;
		SDL_Log("END ADD LINK");
		break;
	case SHUFFLE_LINKS:
		SDL_Log("BEGIN SHUFFLE LINK");
		for (unsigned j = 0; j < nodes[nodeid].size; ++j) {
			a = fastrand() % nodes[nodeid].size;
			b = fastrand() % nodes[nodeid].size;
			// swap
			tmp = nodes[nodeid].ids[a];
			nodes[nodeid].ids[a] = nodes[nodeid].ids[b];
			nodes[nodeid].ids[b] = tmp;
		}
		SDL_Log("END SHUFFLE LINKS");
		break;
	default:
		break;
	}
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

void Brain::store(const char* fname) {
	tinyxml2::XMLDocument doc;
	tinyxml2::XMLPrinter printer;
	printer.OpenElement("brain");
	// store number of input
	printer.OpenElement("input");
	printer.PushText(input);
	printer.CloseElement();
	// store number of hidden
	printer.OpenElement("hidden");
	printer.PushText(hidden);
	printer.CloseElement();
	// store number of output
	printer.OpenElement("output");
	printer.PushText(output);
	printer.CloseElement();
	for (unsigned i = 0; i < output + hidden; ++i) {
		printer.OpenElement("node");
		printer.PushAttribute("inf", nodes[i].in);
		printer.PushAttribute("outf", nodes[i].out);
		printer.PushAttribute("size", nodes[i].size);
		for (unsigned j = 0; j < nodes[i].size; ++j) {
			printer.OpenElement("link");
			printer.PushText(nodes[i].ids[j]);
			printer.CloseElement();
		}
		printer.CloseElement();
	}
	printer.CloseElement();
	doc.Parse(printer.CStr());
	doc.SaveFile(fname);
}

void Brain::load(const char* fname) {
	tinyxml2::XMLDocument doc;
	doc.LoadFile(fname);
	// first element
	tinyxml2::XMLNode *rootnode = doc.FirstChild();
	rootnode->FirstChildElement("input")->QueryIntText((int*)&input);
	rootnode->FirstChildElement("hidden")->QueryIntText((int*)&hidden);
	rootnode->FirstChildElement("output")->QueryIntText((int*)&output);
	size = input + 5 + hidden + output;
	values = new float[size];
	nodes = new Node[output + hidden];
	tinyxml2::XMLNode *nodesNode = rootnode->FirstChildElement("node");
	for (unsigned i = 0; i < output + hidden; ++i) {
		const tinyxml2::XMLElement *element = nodesNode->ToElement();
		element->QueryIntAttribute("inf", (int*)nodes[i].in);
		element->QueryIntAttribute("outf", (int*)nodes[i].out);
		element->QueryIntAttribute("size", (int*)nodes[i].size);
		nodes[i].ids = new unsigned[nodes[i].size];
		tinyxml2::XMLNode *linksNode = nodesNode->FirstChildElement("link");
		for (unsigned j = 0; j < nodes[i].size; ++j) {
			linksNode->ToElement()->QueryIntText((int*)&nodes[i].ids[j]);
			linksNode = linksNode->NextSibling();
		}
		nodesNode = nodesNode->NextSibling();
	}
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

