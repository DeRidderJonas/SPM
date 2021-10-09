#include "pch.h"
#include "CherbilProjectile.h"
#include "Managers.h"

CherbilProjectile::CherbilProjectile(const Sentient* pOwner, Sprite* pSprite)
	: Projectile(pOwner, pSprite, ProjectileType::CherbilProjectile)
	, m_pCountered{Managers::GetInstance()->GetSpriteManager()->GetSprite(SpriteManager::SpriteType::CounteredCherbilProjectile)}
	, m_WasCountered{false}
{
}

bool CherbilProjectile::Update(const Level* pLevel, float elapsedSec)
{
	float border{ 5.f };
	bool hitLevelBoundaries{ m_Hitbox.left < pLevel->GetBoundaries().left + border || m_Hitbox.left + m_Hitbox.width > pLevel->GetBoundaries().left + pLevel->GetBoundaries().width - border };
	if (hitLevelBoundaries)	return true;
	if (pLevel->IsNextToWall(m_Hitbox)) return true;

	if (m_WasCountered) m_pCountered->Update(elapsedSec);

	return Projectile::Update(pLevel, elapsedSec);
}

void CherbilProjectile::Draw() const
{
	if (m_WasCountered) m_pCountered->Draw(m_Hitbox);
	else m_pSprite->Draw(m_Hitbox);
}

void CherbilProjectile::TransferOwnershipTo(Sentient* pSentient)
{
	m_WasCountered = true;
	Projectile::TransferOwnershipTo(pSentient);
}
