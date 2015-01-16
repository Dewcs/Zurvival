#pragma once
#include <iostream>
#include <stdlib.h>
#include <string>
#include "dirent.h"
#include "Const.h"
#include "Functions.h"
#include <string>
#include <vector>

// basic file data
struct fdata {
	std::string fname;
	double score;
};

/*
	Class to hndle the train of brains for the same purpose keeping the best ones alive and deleting the worst ones 
	Gives also random filenames to use in production with random probabilities equals to the good the solution is
*/
class Trainer {
	private:
		const char * folder; // folder where brain data will be stored
		int size,count; // total size ans current size of the array
		unsigned chances; // chances between 0-100 to get a random data
		std::vector<fdata> data; // array of files in the trainer
		double init; // initial value of the trainer for growth calculation
		double sum,unit; // sum of all cases and average
	public:
		Trainer(const char * folder, unsigned size, unsigned chances);
		~Trainer();
		std::string random(); // get a random filename // random means create a new random brain
		bool is_good(double value); // checks if a score on value fits inside the train or should be deprecated
		void insert(std::string fname, double score); // inserts a brain in the trainer
		std::string mkFName(double score); // creates the filename of a file with score score
		void remove(std::string fname); // deletes a file with fname
	private:
		void sort_from(int pos); // sort all items after pos 
};

