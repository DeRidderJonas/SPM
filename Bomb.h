#pragma once
#include "Projectile.h"

class Bomb : public Projectile
{
public:
	Bomb(Sentient* pOwner, Sprite* pSpriteNormal, Sprite* pSpriteExploding);
	virtual ~Bomb() = default;
	virtual bool Update(const Level* pLevel, float elapsedSec) override;
	virtual void Draw() const override;
	virtual bool IsOverlapping(const GameObject* other) const override;
private:
	static const float m_SecTillExplode;
	static const float m_SecExploding;
	float m_AccSec;

	bool m_IsExploding;
	Sprite* m_pExploding;
};

