#pragma once
#include <iostream>
#include <stdlib.h>
#include <string>
#include "dirent.h"
#include "Const.h"
#include "Functions.h"
#include <string>
#include <vector>

struct fdata {
	std::string fname;
	double score;
};

class Trainer {
	private:
		const char * folder;
		int size,count;
		std::vector<fdata> data;
		double init;
	public:
		Trainer(const char * folder, unsigned size);
		~Trainer();
		std::string random();
		bool is_good(double value);
		void insert(std::string fname, double score);
		std::string mkFName(double score);
	private:
		void sort_from(int pos);
};

