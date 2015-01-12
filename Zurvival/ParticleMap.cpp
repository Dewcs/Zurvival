#include "ParticleMap.h"


ParticleMap::ParticleMap()
{
}


ParticleMap::~ParticleMap()
{
	particles.clear();
}

void ParticleMap::update(double x, double y, unsigned delta) {
	cx = x;
	cy = y;
	for (int i = particles.size() - 1; i >= 0; --i) {
		particles[i].time += delta;
		if (particles[i].time<400) {
			particles[i].x += particles[i].dx * particles[i].speed * delta;
			particles[i].y += particles[i].dy * particles[i].speed * delta;
		}
		if (particles[i].time > particles[i].duration) {
			particles.erase(particles.begin() + i);
		}
	}
}
void ParticleMap::draw(SDL_Renderer* renderer, SpriteManager* sprMngr, int width, int height) {
	double scale = (double)height / TILE_FOR_HEIGHT;
	for (int i = 0; i < particles.size(); ++i) {
		SDL_Rect rect = { round(width / 2 + (particles[i].x - cx) * scale), round(height / 2 + (particles[i].y - cy) * scale), 2, 2 };
		SDL_RenderCopy(renderer, sprMngr->getTexture(particles[i].sprite), NULL, &rect);
	}
}
void ParticleMap::add(double x, double y, string sprite, unsigned duration, unsigned amount, double speed) {
	for (int i = 0; i < amount; ++i) {
		Particle p;
		p.duration = duration;
		p.time = 0;
		p.x = x;
		p.y = y;
		p.speed = speed;
		p.sprite = sprite;
		p.dx = randomReal(-1, 1);
		p.dy = randomReal(-1, 1);
		particles.push_back(p);
	}
}