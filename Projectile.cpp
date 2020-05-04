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
	m_Hitbox.width = m_pSprite->GetFrameWidth();
	m_Hitbox.height = m_pSprite->GetFrameHeight();
}

bool Projectile::Update(const Level* pLevel, float elapsedSec)
{
	float border{ 5.f };
	bool hitLevelBoundaries{ m_Hitbox.left < pLevel->GetBoundaries().left + border || m_Hitbox.left + m_Hitbox.width > pLevel->GetBoundaries().left + pLevel->GetBoundaries().width - border };
	if (hitLevelBoundaries)	return true;
	if (pLevel->IsNextToWall(m_Hitbox)) return true;
	GameObject::Update(elapsedSec, pLevel);
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
