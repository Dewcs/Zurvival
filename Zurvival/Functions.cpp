#include "Functions.h"

bool fileExists(const char *fname) {
	if (FILE *file = fopen(fname, "r")) {
		fclose(file);
		return true;
	}
	else {
		return false;
	}
}