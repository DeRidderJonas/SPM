#include "pch.h"
#include "Projectile.h"
#include "Sentient.h"

const float Projectile::m_HorizontalSpeed{ 300.f };

Projectile::Projectile(const Sentient* pOwner, Sprite* pSprite)
	: GameObject(Point2f{pOwner->GetHitbox().left, pOwner->GetHitbox().bottom}, true, true)
	, m_pOwner{pOwner}
	, m_pSprite{pSprite}
{
	SetHorizontalVelocity(m_HorizontalSpeed);
}

bool Projectile::Update(const Level* level, float elapsedSec)
{
	float border{ 5.f };
	bool hitLevelBoundaries{ m_Hitbox.left < level->GetBoundaries().left + border || m_Hitbox.left + m_Hitbox.width > level->GetBoundaries().left + level->GetBoundaries().width - border };
	if (hitLevelBoundaries)	return true;
	GameObject::Update(elapsedSec, level);
	m_pSprite->Update(elapsedSec);
	return false;
}

void Projectile::Draw() const
{
	m_pSprite->Draw(m_Hitbox);
}

bool Projectile::IsOverlapping(const GameObject* other) const
{
	if(other == m_pOwner) return false;
	return GameObject::IsOverlapping(other);
}

bool Projectile::BelongsTo(Sentient* pSentient) const
{
	return pSentient == m_pOwner;
}
