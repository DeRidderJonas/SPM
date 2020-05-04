#include "pch.h"
#include "Coconut.h"

Coconut::Coconut(const Sentient* pOwner, Sprite* pSprite)
	: Projectile(pOwner, pSprite)
{
}

bool Coconut::Update(const Level* pLevel, float elapsedSec)
{
	float border{ 5.f };
	bool hitLevelBoundaries{ m_Hitbox.left < pLevel->GetBoundaries().left + border || m_Hitbox.left + m_Hitbox.width > pLevel->GetBoundaries().left + pLevel->GetBoundaries().width - border };
	if (hitLevelBoundaries)	return true;
	if (pLevel->IsNextToWall(m_Hitbox)) return true;

	return Projectile::Update(pLevel, elapsedSec);
}
