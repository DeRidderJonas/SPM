#pragma once
#include "Projectile.h"

class Coconut : public Projectile
{
public:
	Coconut(const Sentient* pOwner, Sprite* pSprite);
	virtual ~Coconut() = default;
	virtual bool Update(const Level* pLevel, float elapsedSec) override;
};

