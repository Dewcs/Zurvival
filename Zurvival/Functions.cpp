#include "Functions.h"
#include <math.h>

bool fileExists(const char *fname) {
	if (FILE *file = fopen(fname, "r")) {
		fclose(file);
		return true;
	}
	else {
		return false;
	}
}

double angleP2P(int x0, int y0, int x1, int y1) {
	int dx = x1 - x0;
	int dy = y1 - y0;
	return atan2(dy, dx);
}

double rad2deg(double angle) {
	return angle * 180 / M_PI;
}

double deg2rad(double angle) {
	return angle / 180 * M_PI;
}

bool valueInRange(int value, int min, int max)
{
	return (value >= min) && (value <= max);
}

bool rectInsideRect(int x0, int y0, int w0, int h0, int x1, int y1, int w1, int h1) {
	bool xOverlap = valueInRange(x0, x1, x1 + w1) ||
		valueInRange(x1, x0, x0 + w0);

	bool yOverlap = valueInRange(y0, y1, y1 + h1) ||
		valueInRange(y1, y0, y0 + h0);

	return xOverlap && yOverlap;
}