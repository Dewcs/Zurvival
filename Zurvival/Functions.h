#pragma once
#include <SDL.h>
#include <istream>
#include <math.h>
#include <algorithm>
#include "Const.h"


/*******************************************************************************
FILE FUNCTIONS
********************************************************************************/
// checks if a file exists
bool fileExists(const char *fname);

/*******************************************************************************
GEOMETRY FUNCTIONS
********************************************************************************/
// angle formed by 2 points
double angleP2P(double x0, double y0, double x1, double y1); 
// angle from radians to degrees
double rad2deg(double angle);
// angle from degrees to radians
double deg2rad(double angle);
// is value in range [min-max]
bool valueInRange(int value, int min, int max);
// does 2 rect had a common part
bool rectInsideRect(int x0, int y0, int w0, int h0, int x1, int y1, int w1, int h1);
// does 2 rect had a common part
bool rectInsideRect(SDL_Rect a, SDL_Rect b);
// distance between 2 points
float distP2P(float x0, float y0, float x1, float y1);
// map floats to int
int mapf2i(float val, float min, float max, int rangea, int rangeb);
// intersection of 2 rectangles
SDL_Rect rectIntersect(SDL_Rect a, SDL_Rect b);
// distance between 2 angles
double angleDist(double a, double b);
// is angle between angle a and angle b?
bool inAngleRange(double angle, double a, double b);

/*************************************************************************
OTHER FUNCTIONS
**************************************************************************/

unsigned chunkUID(int x, int y);