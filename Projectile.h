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
		Coconut, Bomb
	};
	Projectile(const Sentient* pOwner, Sprite* pSprite, bool verticalSpeedConst = true);
	virtual ~Projectile() = default;
	virtual bool Update(const Level* pLevel, float elapsedSec);
	virtual void Draw() const;
	virtual bool IsOverlapping(const GameObject* other) const override;
	bool BelongsTo(Sentient* pSentient) const;
	void TransferOwnershipTo(Sentient* pSentient);
protected:
	static const float m_HorizontalSpeed;

	Sprite* m_pSprite;
	const Sentient* m_pOwner;
};

