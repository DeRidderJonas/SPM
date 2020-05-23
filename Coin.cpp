#include "pch.h"
#include "Coin.h"

const Vector2f Coin::m_StartVelocity{ 50.f, 500.f };

Coin::Coin(const Point2f& bottomLeft)
	: Particle(bottomLeft, 5.f, new Sprite("Resources/Particle/coin.png", 4, 1, 0.1f))
{
	float randFloat{ float((rand() % 11 + 1) / 10.f) };
	m_Velocity.y = randFloat * m_StartVelocity.y;
	m_Velocity.x = (rand() % 2 == 0 ? 1 : -1) * randFloat * m_StartVelocity.x;

	m_pSprite->SetFrame(rand() % 4);
}

void Coin::Update(float elapsedSec, Level* level)
{
	float fallAcceleration{ 25.1f };
	float horizontalFriction{ 0.6f };
	float elasticity{ 0.7f };

	m_Shape.left += m_Velocity.x * elapsedSec;
	m_Velocity.x += -1 * horizontalFriction * m_Velocity.x * elapsedSec;

	m_Shape.bottom += m_Velocity.y * elapsedSec;

	if (abs(m_Velocity.y) > 0.0001f)
	{
		m_Velocity.y -= fallAcceleration;

		if (m_Velocity.y < 0.0001f && level->IsOnGround(m_Shape))
		{
			if (m_Velocity.y > -1.f) m_Velocity.y = 0.f;
			else m_Velocity.y *= -elasticity; //Bounce back up with less speed
		}
	}


	m_TimeRemaining -= elapsedSec;

	m_pSprite->Update(elapsedSec);
}
