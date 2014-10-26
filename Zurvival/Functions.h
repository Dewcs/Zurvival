#pragma once
#include <SDL.h>
#include <istream>
#include <math.h>
#include <algorithm>
#include "Const.h"

bool fileExists(const char *fname);

double angleP2P(double x0, double y0, double x1, double y1);

//float angleP2P(float x0, float y0, float x1, float y1);

double rad2deg(double angle);

double deg2rad(double angle);

bool valueInRange(int value, int min, int max);

bool rectInsideRect(int x0, int y0, int w0, int h0, int x1, int y1, int w1, int h1);

bool rectInsideRect(SDL_Rect a, SDL_Rect b);

float distP2P(float x0, float y0, float x1, float y1);

int mapf2i(float val, float min, float max, int rangea, int rangeb);

SDL_Rect rectIntersect(SDL_Rect a, SDL_Rect b);

double angleDist(double a, double b);

bool inAngleRange(double angle, double a, double b);

unsigned chunkUID(int x, int y);