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

SDL_Rect rectIntersect(SDL_Rect a, SDL_Rect b){
	int x = fmax(a.x, b.x);
	int num1 = fmin(a.x + a.w, b.x + b.w);
	int y = fmax(a.y, b.y);
	int num2 = fmin(a.y + a.h, b.y + b.h);
	if (num1 >= x && num2 >= y)
		return { x, y, num1 - x, num2 - y };
	else
		return {0,0,0,0};
}