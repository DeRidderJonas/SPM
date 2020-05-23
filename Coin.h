#pragma once

#include "Particle.h"
class Coin : public Particle
{
public:
	Coin(const Point2f& bottomLeft);
	virtual ~Coin() = default;
	virtual void Update(float elapsedSec, Level* level) override;
private:
	static const Vector2f m_StartVelocity;
};

