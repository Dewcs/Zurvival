#pragma once
#include <vector>
#include <SDL.h>
#include "SpriteManager.h"
#include "Functions.h"
using namespace std;

struct Particle {
	double x, y; // current x,y coordinate
	double dx, dy; // direction of the particle
	double speed; // particle travel speed
	unsigned time; // particle alive time (ms)
	unsigned duration; // how many ms does the particle last?
	string sprite; // particle sprite key
};

class ParticleMap
{
	double cx, cy; // map center
	vector<Particle> particles; // list of particles
public:
	ParticleMap();
	~ParticleMap();
	void update(double x, double y, unsigned delta); // update all particles delta times and cet center x,y
	void draw(SDL_Renderer* renderer, SpriteManager* sprMngr, int width, int height); // drar all particles inside screen
	void add(double x, double y, string sprite, unsigned duration, unsigned amount, double speed); // add a new particle (direction is random)
};

