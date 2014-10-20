#pragma once
#include <SDL.h>
#include <istream>
#include <math.h>

bool fileExists(const char *fname);

double angleP2P(int x0, int y0, int x1, int y1);

double rad2deg(double angle);

double deg2rad(double angle);

bool valueInRange(int value, int min, int max);

bool rectInsideRect(int x0, int y0, int w0, int h0, int x1, int y1, int w1, int h1);

SDL_Rect rectIntersect(SDL_Rect a, SDL_Rect b);