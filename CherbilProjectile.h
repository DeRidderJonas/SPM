#pragma once
#include "Projectile.h"

class CherbilProjectile : public Projectile
{
public:
	CherbilProjectile(const Sentient* pOwner, Sprite* pSprite);
	virtual ~CherbilProjectile() = default;
	virtual bool Update(const Level* pLevel, float elapsedSec) override;
};

