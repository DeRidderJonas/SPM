#include "pch.h"
#include "ParticleManager.h"
#include "Coin.h"

ParticleManager::ParticleManager()
{
}

ParticleManager::~ParticleManager()
{
	for (Particle* p: m_Particles)
	{
		delete p;
	}
}

void ParticleManager::Spawn(Point2f bottomLeft, int amountOfParticles)
{
	for (size_t i = 0; i < amountOfParticles; i++)
	{
		m_Particles.push_back(new Coin(bottomLeft));
	}
}

void ParticleManager::UpdateAll(float elapsedSec, Level* pLevel)
{
	std::vector<Particle*>::iterator it{ m_Particles.begin() };
	while (it != m_Particles.end())
	{
		(*it)->Update(elapsedSec, pLevel);

		if ((*it)->CanBeKilled())
		{
			it = DestroyParticle(*it);
		}
		else ++it;
	}
}

void ParticleManager::DrawAll() const
{
	for (Particle* p : m_Particles)
	{
		p->Draw();
	}
}

std::vector<Particle*>::iterator ParticleManager::DestroyParticle(Particle* pParticle)
{
	std::vector<Particle*>::iterator it{ std::find(m_Particles.begin(), m_Particles.end(), pParticle) };
	if (it != m_Particles.end())
	{
		int index{ int(std::distance(m_Particles.begin(), it)) };
		std::vector<Particle*>::iterator it = m_Particles.erase(m_Particles.begin() + index);
		delete pParticle;
		return it;
	}

	return m_Particles.end();
}