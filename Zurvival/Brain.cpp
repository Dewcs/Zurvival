 #include "Brain.h"

Brain::Brain(unsigned input, unsigned output) {
	this->input = input;
	this->output = output;
	/*values = NULL;
	nodes = NULL;*/
}


Brain::~Brain() {
	for (unsigned i = 0; i < nodes.size(); ++i) {
		nodes[i].ids.clear();
	}
	nodes.clear();
	values.clear();
	log(VERBOSE_BRAIN, "BRAIN DELETED");
}

Brain* Brain::copy() {
	Brain *c = new Brain(input, output);
	c->setValues(hidden, values, nodes);
	return c;
}

void Brain::tweak() {
	
	changes_t change = (changes_t)(fastrand() % CHANGES_MAX);
	log(VERBOSE_BRAIN, "BRAIN TWEAKED %d", change);
	unsigned nodeid = fastrand() % (output + hidden);
	unsigned linkid = fastrand() % (nodes[nodeid].ids.size());
	unsigned next = 0;
	unsigned a, b, tmp;
	Node node;
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
		nodes[nodeid].ids[linkid] = fastrand() % (input + 5 + nodeid);
		break;
	case REMOVE_NODE:
		if (hidden > 0) {
			--hidden;
			size = input + 5 + output + hidden;
			// remove value
			values.erase(values.begin() + nodeid);
			nodes.erase(nodes.begin() + nodeid);
			for (unsigned i = nodeid; i < output + hidden; ++i) {
				for (int j = nodes[i].ids.size()-1; j >= 0; --j) {
					if (nodes[i].ids[j]>nodeid + input + 5) --nodes[i].ids[j]; // no problem
					else if (nodes[i].ids[j] == nodeid + input + 5) {
						// remove link
						nodes[i].ids.erase(nodes[i].ids.begin() + j);
					}
				}
				if (nodes[i].ids.size() == 0) {
					nodes[i].ids = std::vector<unsigned>(fastrand() % (input + 4 + i) + 1);
					for (unsigned j = 0; j < nodes[i].ids.size(); ++j) {
						nodes[i].ids[j] = fastrand() % (input + 5 + i);
					}
				}
			}
		}
		break;
	case REMOVE_LINK:
		if (nodes[nodeid].ids.size() > 1) {
			nodes[nodeid].ids.erase(nodes[nodeid].ids.begin() + linkid);
		}
		break;
	case ADD_NODE:
		++hidden;
		size = input + 5 + output + hidden;
		values.insert(values.begin() + nodeid, 0);
		node.in = infunc(fastrand() % INF_SIZE);
		node.out = outfunc(fastrand() % OUTF_SIZE);
		node.ids = std::vector<unsigned>(fastrand() % (input + 4 + nodeid) + 1);
		for (unsigned j = 0; j < node.ids.size(); ++j) {
			node.ids[j] = fastrand() % (input + 5 + nodeid);
		}
		nodes.insert(nodes.begin() + nodeid, node);
		// fix post nodes
		for (unsigned i = nodeid+1; i < nodes.size(); ++i) {
			for (unsigned j = 0; j < nodes[i].ids.size(); ++j) {
				if (nodes[i].ids[j] >= nodeid + input + 5) ++nodes[i].ids[j]; // no problem
			}
			// copy after nodeid values
			// lets link
			if (fastrand() % (input + 5 + nodeid) == 0) {
				nodes[i].ids.insert(nodes[i].ids.begin() + (rand() % nodes[i].ids.size()), nodeid);
			}
		}
		break;
	case ADD_LINK:
		nodes[nodeid].ids.insert(nodes[nodeid].ids.begin() + linkid, fastrand() % (input + 5 + nodeid));
		break;
	case SHUFFLE_LINKS:
		for (unsigned j = 0; j < nodes[nodeid].ids.size(); ++j) {
			a = fastrand() % nodes[nodeid].ids.size();
			b = fastrand() % nodes[nodeid].ids.size();
			// swap
			tmp = nodes[nodeid].ids[a];
			nodes[nodeid].ids[a] = nodes[nodeid].ids[b];
			nodes[nodeid].ids[b] = tmp;
		}
		break;
	default:
		break;
	}
	// check for errors
	for (unsigned i = 0; i < output + hidden; ++i) {
		for (unsigned j = 0; j < nodes[i].ids.size(); ++j) {
			if (nodes[i].ids[j] >= i + input + 5) {
				log(VERBOSE_ERRORS, "BAD TWEAK: %d NODE: %d LINK: %d VALUE: %d", change, i, j, nodes[i].ids[j]);
			}
		}
	}
}

void Brain::randomize() {
	hidden = fastrand() % ((input + output) / 2) + 1;
	size = input + 5 + output + hidden;
	values = std::vector<float> (size);
	nodes = std::vector<Node> (output + hidden);
	for (unsigned i = 0; i < nodes.size(); ++i) {
		nodes[i].in = infunc(fastrand() % INF_SIZE);
		nodes[i].out = outfunc(fastrand() % OUTF_SIZE);
		unsigned size2 = fastrand() % (input + 4 + i) + 1;
		nodes[i].ids = std::vector<unsigned> (size2);
		for (unsigned j = 0; j < size2; ++j) {
			nodes[i].ids[j] = fastrand() % (input + 5 + i);
		}
	}
}
void Brain::setInput(const std::vector<float> &input) {
	for (unsigned i = 0; i < this->input; ++i) {
		values[i] = input[i];
	}
	values[this->input] = -1;
	values[this->input + 1] = 0;
	values[this->input + 2] = 1;
	values[this->input + 3] = (float)this->input;
	values[this->input + 4] = (float)this->output;
}
void Brain::setValues(unsigned hidden, const std::vector<float> &values, const std::vector<Node> &nodes) {
	this->hidden = hidden;
	size = input + 5 + output + hidden;
	this->values = std::vector<float>(size);
	this->nodes = std::vector<Node>(output + hidden);
	for (unsigned i = 0; i < output + hidden; ++i) {
		this->nodes[i].in = nodes[i].in;
		this->nodes[i].out = nodes[i].out;
		this->nodes[i].ids = std::vector<unsigned> (nodes[i].ids.size());
		for (unsigned j = 0; j < nodes[i].ids.size(); ++j) {
			this->nodes[i].ids[j] = nodes[i].ids[j];
		}
	}
}
void Brain::evaluate() {
	log(VERBOSE_BRAIN, "EVAL %d NODES", output + hidden);
	for (unsigned i = 0; i < output + hidden; ++i) {
		for (int j = 0; j < nodes[i].ids.size(); ++j) {
			if (nodes[i].ids[j] >= i + 5 + input) log(VERBOSE_ERRORS, "BAD LINK IN: %d OUT: %d SIZE: %d VALUE: %d", nodes[i].in, nodes[i].in, nodes[i].ids.size(), nodes[i].ids[j]);
		}
		log(VERBOSE_BRAIN, "EVAL NODE %d IN: %d OUT: %d SIZE: %d", i, nodes[i].in, nodes[i].in, nodes[i].ids.size());
		values[i + 5 + input] = out_f[nodes[i].out](in_f[nodes[i].in](values, nodes[i].ids, nodes[i].ids.size()));
		// fix min and max
		if (values[i + 5 + input] < -MAX_VALUE) values[i + 5 + input] = -MAX_VALUE;
		else if (values[i + 5 + input] > MAX_VALUE) values[i + 5 + input] = MAX_VALUE;
		else if (values[i + 5 + input]>0 && values[i + 5 + input]<MIN_VALUE) values[i + 5 + input] = 0;
		else if (values[i + 5 + input]<0 && values[i + 5 + input]>-MIN_VALUE) values[i + 5 + input] = 0;
		// check bad values
		if (!std::isfinite(values[i + 5 + input])) {
			log(VERBOSE_ERRORS, "NON FINITE VALUE IN: %d OUT: %d SIZE: %d", nodes[i].in, nodes[i].in, nodes[i].ids.size());
			for (int j = 0; j < nodes[i].ids.size(); ++j) {
				log(VERBOSE_ERRORS, "INVAL %d -> %d = %f", j, nodes[i].ids[j], values[nodes[i].ids[j]]);
			}
			values[i + 5 + input] = 0;
		}
		log(VERBOSE_BRAIN, "EVAL NODE %d RESULT: %f", i, values[i + 5 + input]);
	}
}
void Brain::getResult(std::vector<float> &output) {
	output = std::vector<float>(output);
	for (unsigned i = 0; i < this->output; ++i) output[i] = values[input + 5 + hidden + i];
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
		printer.PushAttribute("size", nodes[i].ids.size());
		for (unsigned j = 0; j < nodes[i].ids.size(); ++j) {
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
	log(VERBOSE_BRAIN, "BRAIN READ FROM %s",fname);
	tinyxml2::XMLDocument doc;
	doc.LoadFile(fname);
	// first element
	tinyxml2::XMLNode *rootnode = doc.FirstChild();
	rootnode->FirstChildElement("input")->QueryIntText((int*)&input);
	rootnode->FirstChildElement("hidden")->QueryIntText((int*)&hidden);
	rootnode->FirstChildElement("output")->QueryIntText((int*)&output);
	size = input + 5 + hidden + output;
	log(VERBOSE_BRAIN, "INPUT: %d HIDDEN: %d OUTPUT: %d",input,hidden,output);
	values = std::vector<float> (size);
	nodes = std::vector<Node>(output + hidden);
	tinyxml2::XMLNode *nodesNode = rootnode->FirstChildElement("node");
	for (unsigned i = 0; i < output + hidden; ++i) {
		const tinyxml2::XMLElement *element = nodesNode->ToElement();
		element->QueryIntAttribute("inf", (int*)&nodes[i].in);
		element->QueryIntAttribute("outf", (int*)&nodes[i].out);
		int size;
		element->QueryIntAttribute("size", &size);

		log(VERBOSE_BRAIN, "NODE: %d INF: %d OUTF: %d SIZE: %d", i, nodes[i].in, nodes[i].out,size);
		nodes[i].ids = std::vector<unsigned>(size);
		tinyxml2::XMLNode *linksNode = nodesNode->FirstChildElement("link");
		for (unsigned j = 0; j < nodes[i].ids.size(); ++j) {
			linksNode->ToElement()->QueryIntText((int*)&nodes[i].ids[j]);
			log(VERBOSE_BRAIN, "LINK: %d ID: %d", j, nodes[i].ids[j]);
			if (nodes[i].ids[j] >= i + input + 5) {
				log(VERBOSE_ERRORS, "BAD READ FILE: %s NODE: %d LINK: %d VALUE: %d", fname,i,j,nodes[i].ids[j]);
			}
			linksNode = linksNode->NextSibling();
		}
		nodesNode = nodesNode->NextSibling();
	}
}

void Brain::print() {
	log(1, "BRAIN %d %d %d", input, hidden, output);
	for (unsigned i = 0; i < output + hidden; ++i) {
		log(1, "NODE %d IN: %d OUT: %d", i + input + 5, nodes[i].in, nodes[i].out);
		for (unsigned j = 0; j < nodes[i].ids.size(); ++j) {
			log(1, "LINK %d TO: %d", j, nodes[i].ids[j]);
		}
	}
}

void Brain::optimize() {

}

float isum_all(const std::vector<float> &values, const std::vector<unsigned> &ids, unsigned size) {
	float sum = 0;
	for (unsigned i = 0; i < size; ++i) sum += values[ids[i]];
	return sum;
}

float irest(const std::vector<float> &values, const std::vector<unsigned> &ids, unsigned size) {
	float ret = values[ids[0]];
	for (unsigned i = 1; i < size; ++i) ret -= values[ids[i]];
	return ret;
}

float irest2(const std::vector<float> &values, const std::vector<unsigned> &ids, unsigned size) {
	float ret = -values[ids[0]];
	for (unsigned i = 1; i < size; ++i) ret -= values[ids[i]];
	return ret;
}

float imul(const std::vector<float> &values, const std::vector<unsigned> &ids, unsigned size) {
	float ret = 1;
	for (unsigned i = 0; i < size; ++i) ret *= values[ids[i]];
	return ret;
}

float idiv(const std::vector<float> &values, const std::vector<unsigned> &ids, unsigned size) {
	float ret = values[ids[0]];
	for (unsigned i = 1; i < size; ++i) {
		if (values[ids[i]] != 0) ret /= values[ids[i]];
	}
	return ret;
}

float imin(const std::vector<float> &values, const std::vector<unsigned> &ids, unsigned size) {
	float min = values[ids[0]];
	for (unsigned i = 1; i < size; ++i) {
		if (min>values[ids[i]]) {
			min = values[ids[i]];
		}
	}
	return min;
}

float imax(const std::vector<float> &values, const std::vector<unsigned> &ids, unsigned size) {
	float max = values[ids[0]];
	for (unsigned i = 1; i < size; ++i) {
		if (max<values[ids[i]]) {
			max = values[ids[i]];
		}
	}
	return max;
}

float imed(const std::vector<float> &values, const std::vector<unsigned> &ids, unsigned size) {
	std::vector<float> tmp(size);
	for (unsigned i = 0; i < size; ++i) {
		tmp[i] = values[ids[i]];
	}
	std::sort(tmp.begin(), tmp.end());
	float ret = tmp[size / 2];
	if (size % 2 == 0) {
		ret = (ret + tmp[(size / 2) - 1]) / 2;
	}
	tmp.clear();
	return ret;
}

float iavg(const std::vector<float> &values, const std::vector<unsigned> &ids, unsigned size) {
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

