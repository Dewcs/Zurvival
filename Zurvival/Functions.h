#pragma once
#include <SDL.h>
#include <istream>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <math.h>
#include <stdio.h>
#include <algorithm>
#include <time.h>
#include "Const.h"
#include "dirent.h"


/*******************************************************************************
FILE FUNCTIONS
********************************************************************************/
// checks if a file exists
bool fileExists(const char *fname);
// deletes a file
bool deleteFile(const char *fname);
// copy a file
void copyFile(const char *src, const char *dst);

/*LOG*/
int log(int level, const char *format, ...);

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
// is point inside rectangle
bool pointInsideRect(int x0, int y0, int x1, int y1, int w1, int h1);
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
STRING FUNCTIONS
**************************************************************************/
// convers a double to n decimals string
std::string to_string_with_precision(double a_value, const int n);

/*************************************************************************
OTHER FUNCTIONS
**************************************************************************/
// random real value
double randomReal(double min, double max);
// returns a chunk unique identifier
unsigned chunkUID(int x, int y);