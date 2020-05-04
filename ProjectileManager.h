#pragma once
#include <vector>
#include "Projectile.h"
#include "SpriteManager.h"
#include "Level.h"

class Player;
class EnemyManager;

class ProjectileManager
{
public:
	ProjectileManager();
	~ProjectileManager();
	
	ProjectileManager(const ProjectileManager& other) = delete;
	ProjectileManager& operator=(const ProjectileManager& other) = delete;
	ProjectileManager(ProjectileManager&& other) = delete;
	ProjectileManager operator=(ProjectileManager&& other) = delete;

	Projectile* Spawn(Sentient* pOwner, bool goingLeft, Projectile::ProjectileType type = Projectile::ProjectileType::Coconut);
	void UpdateAll(float elapsedSec, const Level* level);
	void DrawAll() const;
	void HitSentients(Player* pPlayer);

	void Destroy(Projectile* pProjectile);
	void DestroyAll();
private:
	std::vector<Projectile*> m_Projectiles;
};

