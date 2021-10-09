#pragma once
#include "Sentient.h"
class ProjectileFactory
{
public:
	static Projectile* MakeProjectile(Sentient* pOwner, Projectile::ProjectileType type);
};

