#include "pch.h"
#include "Managers.h"
#include "Player.h"
#include "ProjectileFactory.h"

ProjectileManager::ProjectileManager()
{
}

ProjectileManager::~ProjectileManager()
{
	for (Projectile* pProjectile : m_Projectiles)
	{
		delete pProjectile;
	}
}

Projectile* ProjectileManager::Spawn(Sentient* pOwner, bool goingLeft, Projectile::ProjectileType type)
{
	Projectile* newProjectile{ ProjectileFactory::MakeProjectile(pOwner, type) };
	if (goingLeft) newProjectile->SetHorizontalVelocity(-newProjectile->GetVelocity().x);

	if (type == Projectile::ProjectileType::Bomb)
	{
		newProjectile->SetHorizontalVelocity(0.f);
		newProjectile->SetPosition(Point2f{ pOwner->GetHitbox().left + (goingLeft ? -1 : 1) * newProjectile->GetHitbox().width, pOwner->GetHitbox().bottom });
	}

	m_Projectiles.push_back(newProjectile);

	return newProjectile;
}

void ProjectileManager::UpdateAll(float elapsedSec, const Level* level)
{
	for (Projectile* proj : m_Projectiles)
	{
		if (proj->Update(level, elapsedSec))
		{
			Destroy(proj);
		}
	}
}

void ProjectileManager::DrawAll() const
{
	for (Projectile* proj : m_Projectiles)
	{
		proj->Draw();
	}
}

void ProjectileManager::HitSentients(Player* pPlayer)
{
	for (Projectile* proj : m_Projectiles)
	{
		if (proj->BelongsTo(pPlayer))
		{
			for (Enemy* enemy : Managers::GetInstance()->GetEnemyManager()->GetEnemies())
			{
				if (proj->IsOverlapping(enemy))
				{
					Managers::GetInstance()->GetEnemyManager()->Kill(enemy);
					Destroy(proj);
					break;
				}
			}
		}
		else
		{
			if (proj->IsOverlapping(pPlayer))
			{
				float playerMidX{ pPlayer->GetHitbox().left + pPlayer->GetHitbox().width / 2 };
				float projMidX{ proj->GetHitbox().left + proj->GetHitbox().width / 2 };
				bool projLeftFromPlayer{ playerMidX < projMidX };

				pPlayer->WasHit(projLeftFromPlayer);
				Destroy(proj);
			}
		}
		
	}
}

void ProjectileManager::Destroy(Projectile* pProjectile)
{
	std::vector<Projectile*>::iterator it{ std::find(m_Projectiles.begin(), m_Projectiles.end(), pProjectile) };
	if (it == m_Projectiles.end()) return;
	
	//Projectile was found
	int index{ int(std::distance(m_Projectiles.begin(), it)) };

	m_Projectiles.erase(m_Projectiles.begin() + index);
	delete pProjectile;
}

void ProjectileManager::DestroyAll()
{
	for (Projectile* projectile : m_Projectiles)
	{
		delete projectile;
	}
	m_Projectiles.clear();
}
