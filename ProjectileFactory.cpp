#include "pch.h"
#include "ProjectileFactory.h"
#include "Coconut.h"
#include "Bomb.h"
#include "CherbilProjectile.h"
#include "Managers.h"

Projectile* ProjectileFactory::MakeProjectile(Sentient* pOwner, Projectile::ProjectileType type)
{
	SpriteManager* pSM{ Managers::GetInstance()->GetSpriteManager() };
	switch (type)
	{
	case Projectile::ProjectileType::Coconut:
		return new Coconut(pOwner, pSM->GetSprite(SpriteManager::SpriteType::Coconut));
		break;
	case Projectile::ProjectileType::Bomb:
		return new Bomb(pOwner, pSM->GetSprite(SpriteManager::SpriteType::Bomb), pSM->GetSprite(SpriteManager::SpriteType::BombExplosion));
		break;
	case Projectile::ProjectileType::CherbilProjectile:
		return new CherbilProjectile(pOwner, pSM->GetSprite(SpriteManager::SpriteType::CherbilProjectile));
		break;
	}
	return nullptr;
}
