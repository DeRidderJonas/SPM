#include "pch.h"
#include "CherbilProjectile.h"

CherbilProjectile::CherbilProjectile(const Sentient* pOwner, Sprite* pSprite)
	: Projectile(pOwner, pSprite, ProjectileType::CherbilProjectile)
{
}

bool CherbilProjectile::Update(const Level* pLevel, float elapsedSec)
{
	float border{ 5.f };
	bool hitLevelBoundaries{ m_Hitbox.left < pLevel->GetBoundaries().left + border || m_Hitbox.left + m_Hitbox.width > pLevel->GetBoundaries().left + pLevel->GetBoundaries().width - border };
	if (hitLevelBoundaries)	return true;
	if (pLevel->IsNextToWall(m_Hitbox)) return true;

	return Projectile::Update(pLevel, elapsedSec);
}
