#pragma once
#include <vector>
#include <SDL.h>
#include "SpriteManager.h"
#include "Functions.h"
using namespace std;

struct Particle {
	double x, y;
	double dx, dy;
	double speed;
	unsigned time;
	unsigned duration;
	string sprite;
};

class ParticleMap
{
	double cx, cy;
	vector<Particle> particles;
public:
	ParticleMap();
	~ParticleMap();
	void update(double x, double y, unsigned delta);
	void draw(SDL_Renderer* renderer, SpriteManager* sprMngr, int width, int height);
	void add(double x, double y, string sprite, unsigned duration, unsigned amount, double speed);
};

