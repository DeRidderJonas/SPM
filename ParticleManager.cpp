#include "pch.h"
#include "ParticleManager.h"
#include "Coin.h"
#include "Smoke.h"

ParticleManager::ParticleManager()
	: m_pPlayer{nullptr}
{
}

ParticleManager::~ParticleManager()
{
	for (Particle* p: m_Particles)
	{
		delete p;
	}
}

void ParticleManager::Spawn(Point2f bottomLeft, Particle::ParticleType type, int amountOfParticles)
{
	switch (type)
	{
	case Particle::ParticleType::Coin:
		for (size_t i = 0; i < amountOfParticles; i++)
		{
			m_Particles.push_back(new Coin(bottomLeft));
		}
		break;
	case Particle::ParticleType::Smoke:
		for (size_t i = 0; i < amountOfParticles; i++)
		{
			m_Particles.push_back(new Smoke(bottomLeft));
		}
		break;
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
		if (pParticle->GetType() == Particle::ParticleType::Coin) m_pPlayer->AdjustAmountOfCoins(1);

		int index{ int(std::distance(m_Particles.begin(), it)) };
		std::vector<Particle*>::iterator it = m_Particles.erase(m_Particles.begin() + index);
		delete pParticle;

		return it;
	}

	return m_Particles.end();
}

void ParticleManager::SetPlayer(Player* pPlayer)
{
	m_pPlayer = pPlayer;
}
