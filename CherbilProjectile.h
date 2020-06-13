#pragma once
#include "Projectile.h"

class CherbilProjectile : public Projectile
{
public:
	CherbilProjectile(const Sentient* pOwner, Sprite* pSprite);
	virtual ~CherbilProjectile() = default;
	virtual bool Update(const Level* pLevel, float elapsedSec) override;
	virtual void Draw() const override;
	virtual void TransferOwnershipTo(Sentient* pSentient) override;
private:
	Sprite* m_pCountered;
	bool m_WasCountered;
};

