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

bool deleteFile(const char *fname) {
	if (fileExists(fname) && remove(fname)==0) return true;
	return false;
}

int log(int level, const char *format, ...) {
	if ((level & VERBOSE_LEVEL)!=0) {
		va_list arg;
		int done;

		va_start(arg, format);
		time_t t = time(0);   // get time now
		struct tm * now = localtime(&t);
		printf("[%04d/%02d/%02d %02d:%02d:%02d] ", now->tm_year+1900, now->tm_mon, now->tm_mday, now->tm_hour, now->tm_min, now->tm_sec);
		done = vfprintf(stdout, format, arg);
		printf("\n");
		va_end(arg);
		fflush(stdout);
		return done;
	}
	return 0;
}

double angleP2P(double x0, double y0, double x1, double y1) {
	double dx = x1 - x0;
	double dy = y1 - y0;
	return atan2(dy, dx);
}

/*float angleP2P(float x0, float y0, float x1, float y1) {
	float dx = x1 - x0;
	float dy = y1 - y0;
	return atan2(dy, dx);
}*/

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
	bool xOverlap = valueInRange(x0, x1, x1 + w1) || valueInRange(x1, x0, x0 + w0);

	bool yOverlap = valueInRange(y0, y1, y1 + h1) || valueInRange(y1, y0, y0 + h0);

	return xOverlap && yOverlap;
}

bool rectInsideRect(SDL_Rect a, SDL_Rect b) {
	bool xOverlap = valueInRange(a.x, b.x, b.x + b.w) || valueInRange(b.x, a.x, a.x + a.w);

	bool yOverlap = valueInRange(a.y, b.y, b.y + b.h) || valueInRange(b.y, a.y, a.y + a.h);

	return xOverlap && yOverlap;
}

bool pointInsideRect(int x0, int y0, int x1, int y1, int w1, int h1) {
	return x0 >= x1 && x0 <= x1 + w1 && y0 >= y1 && y0 <= y1 + h1;
}


float distP2P(float x0, float y0, float x1, float y1) {
	float dx = x1 - x0;
	float dy = y1 - y0;
	return sqrt(dx*dx + dy*dy);
}

int mapf2i(float val, float min, float max, int rangea, int rangeb) {
	if (val <= min) return rangea;
	else if (val >= max) return rangeb;
	else {
		return int((val - min) / (max - min) * (rangeb - rangea)) + rangea;
	}
}

double angleDist(double a, double b) {
	return abs(atan2(sin(a - b), cos(b - a)));
}

bool inAngleRange(double angle, double a, double b) {
	return abs(angleDist(a, b) - angleDist(angle, a) - angleDist(angle, b))<0.001;
}

SDL_Rect rectIntersect(SDL_Rect a, SDL_Rect b){
	int x = max(a.x, b.x);
	int num1 = min(a.x + a.w, b.x + b.w);
	int y = max(a.y, b.y);
	int num2 = min(a.y + a.h, b.y + b.h);
	if (num1 >= x && num2 >= y) {
		SDL_Rect ret = { x, y, num1 - x, num2 - y };
		return ret;
	}
	else {
		SDL_Rect ret = { 0, 0, 0, 0 };
		return ret;
	}
}

std::string to_string_with_precision(double a_value, const int n) {
	std::ostringstream out;
	out << std::fixed << std::setprecision(n) << a_value;
	return out.str();
}

double randomReal(double min, double max) {
	return min + (max - min) * rand() / (double)RAND_MAX;
}

unsigned chunkUID(int x, int y) {
	return (x + (MAX_CHUNK >> 1)) * MAX_CHUNK + (y + (MAX_CHUNK >> 1));
}