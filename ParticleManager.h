#pragma once
#include "structs.h"
#include <vector>
#include "Particle.h"
class ParticleManager
{
public:
	ParticleManager();
	~ParticleManager();

	ParticleManager(const ParticleManager& other) = delete;
	ParticleManager& operator=(const ParticleManager& other) = delete;
	ParticleManager(ParticleManager&& other) = delete;
	ParticleManager& operator=(ParticleManager&& other) = delete;

	void Spawn(Point2f bottomLeft, int amountOfParticles);
	void UpdateAll(float elapsedSec, Level* pLevel);
	void DrawAll() const;

	std::vector<Particle*>::iterator DestroyParticle(Particle* pParticle);

private:
	std::vector<Particle*> m_Particles;
};

