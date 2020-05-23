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
		Coconut, Bomb, CherbilProjectile
	};
	Projectile(const Sentient* pOwner, Sprite* pSprite, ProjectileType type, bool verticalSpeedConst = true);
	virtual ~Projectile() = default;
	virtual bool Update(const Level* pLevel, float elapsedSec);
	virtual void Draw() const;
	virtual bool IsOverlapping(const GameObject* other) const override;
	
	bool BelongsTo(Sentient* pSentient) const;
	void TransferOwnershipTo(Sentient* pSentient);

	ProjectileType GetType() const;
protected:
	static const float m_HorizontalSpeed;

	ProjectileType m_Type;
	Sprite* m_pSprite;
	const Sentient* m_pOwner;
};

