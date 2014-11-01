#include "Trainer.h"


Trainer::Trainer(const char * folder) {
	this->folder = folder;
}


Trainer::~Trainer()
{
}

std::string Trainer::random() {
	DIR *pdir = NULL;
	struct dirent *pent = NULL;

	pdir = opendir(folder);

	if (pdir != NULL) {
		while (pent = readdir(pdir)) // while there is still something in the directory to list
		{
			if (pent == NULL) // if pent has not been initialised correctly
			{ 
				break;
			}
			std::cout <<pent->d_name<<std::endl;
		}
		closedir(pdir);

	}
	else {
		// should create the directory anyway
	}
	return "random";
}