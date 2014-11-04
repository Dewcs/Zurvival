#include "Trainer.h"


Trainer::Trainer(const char * folder, unsigned size, unsigned chances) {
	this->folder = folder;
	this->size = size;
	this->chances = chances;
	count = 0;
	sum = 0;
	unit = 0;
	data = std::vector<fdata> (size);
	// read dir
	DIR *pdir = NULL;
	struct dirent *pent = NULL;
	pdir = opendir(folder);
	if (pdir != NULL) {
		while (pent = readdir(pdir)) // while there is still something in the directory to list
		{
			if (pent == NULL) // if pent has not been initialised correctly
			{
				log(VERBOSE_ERRORS, "FILE LOOP FAILED FOUND %s", folder);
				break;
			}
			std::string fname = std::string(pent->d_name);
			if (fname != "." && fname != "..") {
				double value = atof(fname.substr(0, fname.length() - 4).c_str());
				insert(std::string(folder) + "/" + std::string(pent->d_name), value);
			}
			
		}
		init = sum / count;
		log(VERBOSE_TRAIN, "TRAIN %s AVG: %f", folder , init);
		closedir(pdir);

	}
	else {
		log(VERBOSE_ERRORS, "FILE NOT FOUND %s", folder);
	}
}


Trainer::~Trainer()
{
	double sum = 0;
	for (int i = 0; i < count; ++i) sum += data[i].score;
	log(VERBOSE_TRAIN, "TRAIN %s INCREASED BY %f", folder, (sum / count) - init);
	data.clear();
}

std::string Trainer::random() {
	
	int r = rand() % 100;
	if (r <= chances || count==0) {
		return "random";
	}
	else {
		int total = round(sum / unit);
		int r2 = rand() % total;
		double tmpsum = 0;
		double final = r2*unit;
		int pos = 0;
		while (pos<size && tmpsum + data[pos].score < final) {
			tmpsum += data[pos].score;
			++pos;
		}
		return data[pos].fname;
		/*// select 1 giving the top one 1 more chances than the seconds and so on
		int max = (count*(count + 1)) >> 1; // sum 1-N
		int r2 = rand() % max; // random pos
		int begin = count; 
		int acc = begin;
		while (r2 > acc&&begin>0) acc += --begin;
		return data[count - begin].fname;*/
	}
}

bool Trainer::is_good(double value) {
	std::string fname = mkFName(value);
	if (fileExists(fname.c_str())) return false;
	else if (count < size) return true;
	else return value>data[size - 1].score;
}

void Trainer::insert(std::string fname, double score) {
	if (count < size) {
		data[count].fname = fname;
		data[count].score = score;
		sort_from(count);
		sum += score;
		if (data.size() != 0) unit = data[count].score;
		++count;
	}
	else {
		if (score>data[size - 1].score) {
			sum = sum - data[size - 1].score + score;
			data[size - 1].score = score;
			deleteFile(data[size - 1].fname.c_str());
			data[size - 1].fname = fname;
			sort_from(size - 1);
			if (data.size() != 0) unit = data[size-1].score;
		}
		else {
			deleteFile(fname.c_str());
		}
	}
}

std::string Trainer::mkFName(double score) {
	return std::string(folder) + "/" + to_string_with_precision(score, 15) + ".xml";
}

void Trainer::sort_from(int pos) {
	while (pos >= 1 && data[pos].score > data[pos - 1].score) {
		// swap
		fdata tmp;
		tmp.fname = data[pos].fname;
		tmp.score = data[pos].score;
		data[pos].fname = data[pos-1].fname;
		data[pos].score = data[pos-1].score;
		data[pos-1].fname = tmp.fname;
		data[pos-1].score = tmp.score;
		--pos;
	}
}

