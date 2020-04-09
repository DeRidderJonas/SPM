#include "pch.h"
#include "EnemyManager.h"
#include "EnemyFactory.h"
#include "Managers.h"
#include <iostream>

const int EnemyManager::m_LootSpawnChance{ 10 };

EnemyManager::EnemyManager()
	: m_KeySpawned{false}
	, m_pPlayer{nullptr}
{
}

EnemyManager::~EnemyManager()
{
	for (Enemy* enemy : m_Enemies)
	{
		delete enemy;
	}
}

void EnemyManager::Spawn(Enemy::Type type, size_t amount, Rectf spawnBox)
{
	for (size_t i = 0; i < amount; i++)
	{
		m_Enemies.push_back(EnemyFactory::CreateEnemy(type, spawnBox));
	}
}

void EnemyManager::UpdateAll(float elapsedSec, const Level* level)
{
	for (Enemy* enemy: m_Enemies)
	{
		enemy->Update(elapsedSec, level);
		for (Enemy* other : m_Enemies)
		{
			if (enemy != other && enemy->CanBeFlipped() && enemy->IsOverlapping(other))
			{
				enemy->FlipHorizontalVelocity();
				bool enemyGoingLeft{ enemy->GetVelocity().x < 0 };
				other->SetHorizontalVelocity((enemyGoingLeft ? abs(other->GetVelocity().x) : -abs(other->GetVelocity().x)));
			}
		}
	}
}

void EnemyManager::DrawAll() const
{
	for (Enemy* enemy : m_Enemies)
	{
		enemy->Draw();
	}
}

void EnemyManager::AllAttack()
{
	for (Enemy* enemy : m_Enemies)
	{
		enemy->Attack();
	}
}

void EnemyManager::AttackAll(const Rectf& attackHitbox)
{
	for (Enemy* enemy : m_Enemies)
	{
		if (enemy->IsOverlapping(attackHitbox))
		{
			Kill(enemy);
		}
	}
}

void EnemyManager::SetPlayer(Player* pPlayer)
{
	m_pPlayer = pPlayer;
}

bool EnemyManager::HitPlayer()
{
	if (m_pPlayer->IsInvincible()) return false;

	for (Enemy* enemy : m_Enemies)
	{
		if (enemy->IsOverlapping(m_pPlayer))
		{
			float playerMidX{ m_pPlayer->GetHitbox().left + m_pPlayer->GetHitbox().width / 2 };
			float enemyMidX{ enemy->GetHitbox().left + enemy->GetHitbox().width / 2 };
			bool playerLeftOfEnemy{ playerMidX < enemyMidX };

			bool playerLandsOnEnemy{m_pPlayer->GetHitbox().bottom > enemy->GetHitbox().bottom + 3 * enemy->GetHitbox().height / 5};
			if (playerLandsOnEnemy)
			{
				bool enemyShouldDie{ enemy->JumpedOnByPlayer() };
				if (enemyShouldDie)
				{
					Kill(enemy);
					m_pPlayer->SetVerticalVelocity(300.f);
				}
				else m_pPlayer->WasHit(playerLeftOfEnemy);
				return true;
			}

			m_pPlayer->WasHit(playerLeftOfEnemy);
			return true;
		}
	}
	return false;
}

void EnemyManager::Kill(Enemy* enemy)
{
	std::vector<Enemy*>::iterator it{ std::find(m_Enemies.begin(), m_Enemies.end(), enemy) };
	if (it != m_Enemies.end())
	{
		//Enemy was found
		int index{ int(std::distance(m_Enemies.begin(), it)) };
		
		m_Enemies.erase(m_Enemies.begin() + index);
		SpawnLoot(Point2f{enemy->GetHitbox().left + enemy->GetHitbox().width/2, enemy->GetHitbox().bottom});

		delete enemy;
	}
}

std::vector<Enemy*> EnemyManager::GetEnemies() const
{
	return m_Enemies;
}

void EnemyManager::SpawnLoot(Point2f pos)
{
	//Loot is spawned after m_Enemies size is decreased
	if (m_Enemies.size() > 0)
	{
		int chance{ rand() % 100 };
		bool itemWillSpawn{ chance <= m_LootSpawnChance };
		if (itemWillSpawn) Managers::GetInstance()->GetItemManager()->SpawnRandom(pos, m_pPlayer, m_KeySpawned);
	}
	else if(!m_KeySpawned)
	{
		Managers::GetInstance()->GetItemManager()->Spawn(Item::Type::Key, pos, m_pPlayer);
	}
}

