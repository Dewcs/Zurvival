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
		unsigned chances;
		std::vector<fdata> data;
		double init;
		double sum,unit;
	public:
		Trainer(const char * folder, unsigned size, unsigned chances);
		~Trainer();
		std::string random();
		bool is_good(double value);
		void insert(std::string fname, double score);
		std::string mkFName(double score);
		void remove(std::string fname);
	private:
		void sort_from(int pos);
};

