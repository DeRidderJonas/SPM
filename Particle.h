#pragma once
#include "structs.h"
#include "Level.h"

class Particle
{
public:
	Particle(Point2f bottomLeft, float lifeDuration, Sprite* pSprite);
	virtual ~Particle();

	virtual void Update(float elapsedSec, Level* pLevel) = 0;
	void Draw() const;

	bool CanBeKilled() const;
protected:
	Rectf m_Shape;
	float m_TimeRemaining;
	Sprite* m_pSprite;

	Vector2f m_Velocity;
};

