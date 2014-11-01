#pragma once
#include <iostream>
#include <string>
#include "dirent.h"

struct fdata {
	std::string fname;
	double score;
};

class Trainer {
	private:
		const char * folder;
	public:
		Trainer(const char * folder);
		~Trainer();
		std::string random();
};

