#pragma once
#include "Particle.h"
class Smoke : public Particle
{
public:
	Smoke(Point2f bottomLeft);
	~Smoke() = default;

	virtual void Update(float elapsedSec, const Level* pLevel) override;
private:
	static const Vector2f m_StartVelocity;
};

