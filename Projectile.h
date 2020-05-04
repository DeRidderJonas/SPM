#pragma once
#include "Sprite.h"
#include "Level.h"
#include "GameObject.h"

class Sentient;

class Projectile : public GameObject
{
public:
	enum class ProjectileType
	{
		Coconut
	};
	Projectile(const Sentient* pOwner, Sprite* pSprite);
	bool Update(const Level* pLevel, float elapsedSec);
	void Draw() const;
	virtual bool IsOverlapping(const GameObject* other) const override;
	bool BelongsTo(Sentient* pSentient) const;
private:
	static const float m_HorizontalSpeed;

	Sprite* m_pSprite;
	const Sentient* m_pOwner;
};

