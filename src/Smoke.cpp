#include "pch.h"
#include "Smoke.h"

const Vector2f Smoke::m_StartVelocity{50.f, 150.f};

Smoke::Smoke(Point2f bottomLeft)
	: Particle(bottomLeft, 1.3f, new Sprite("Resources/Particle/smoke.png", 3, 3, 2.f/9), ParticleType::Smoke)
{
	float randFloat{ ((rand() % 10) + 1) / 10.f };
	m_Velocity.x = (rand() % 2 == 0 ? -1 : 1) * randFloat * m_StartVelocity.x;
	m_Velocity.y = randFloat * m_StartVelocity.y;
}

void Smoke::Update(float elapsedSec, const Level* pLevel)
{
	m_pSprite->Update(elapsedSec);
	m_TimeRemaining -= elapsedSec;

	m_Shape.left += m_Velocity.x * elapsedSec;
	m_Shape.bottom += m_Velocity.y * elapsedSec;

	if (m_Shape.bottom + m_Shape.height >= pLevel->GetBoundaries().bottom + pLevel->GetBoundaries().height) m_Velocity.y = 0.f;
}
