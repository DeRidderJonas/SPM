#include "pch.h"
#include "Particle.h"

Particle::Particle(Point2f bottomLeft, float lifeDuration, Sprite* pSprite, ParticleType type)
	: m_Shape{bottomLeft.x, bottomLeft.y, pSprite->GetFrameWidth(), pSprite->GetFrameHeight()}
	, m_TimeRemaining{lifeDuration}
	, m_pSprite{pSprite}
	, m_Velocity{0.f,0.f}
	, m_Type{type}
{
}

Particle::~Particle()
{
	delete m_pSprite;
}

void Particle::Draw() const
{
	m_pSprite->Draw(m_Shape);
}

Particle::ParticleType Particle::GetType() const
{
	return m_Type;
}

bool Particle::CanBeKilled() const
{
	return m_TimeRemaining <= 0.f;
}
