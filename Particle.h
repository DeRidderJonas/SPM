#pragma once
#include "structs.h"
#include "Level.h"

class Particle
{
public:
	enum class ParticleType
	{
		Coin, Smoke
	};

	Particle(Point2f bottomLeft, float lifeDuration, Sprite* pSprite, ParticleType type);
	virtual ~Particle();

	virtual void Update(float elapsedSec, Level* pLevel) = 0;
	void Draw() const;
	ParticleType GetType() const;
	bool CanBeKilled() const;
protected:
	Rectf m_Shape;
	float m_TimeRemaining;
	Sprite* m_pSprite;

	Vector2f m_Velocity;

	ParticleType m_Type;
};

