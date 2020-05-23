#pragma once
#include <vector>
#include "Enemy.h"
#include "SpriteManager.h"
#include "Player.h"
#include "ItemManager.h"
#include <algorithm>

class EnemyManager
{
public:
	EnemyManager();
	~EnemyManager();
	EnemyManager(const EnemyManager& other) = delete;
	EnemyManager& operator=(const EnemyManager& other) = delete;
	EnemyManager(EnemyManager&& other) = delete;
	EnemyManager& operator=(EnemyManager&& other) = delete;

	void Spawn(Enemy::Type type, size_t amount, Rectf spawnBox);
	void UpdateAll(float elapsedSec, const Level* level);
	void DrawAll() const;
	void AllAttack(); //All enemies attack
	void AttackAll(const Rectf& attackHitbox); //Player attacks enemies

	void SetPlayer(Player* pPlayer);
	bool HitPlayer();

	void Kill(Enemy* enemy);
	void KillAllEnemies();
	std::vector<Enemy*> GetEnemies() const;

	template<typename Function>
	void ApplyOnEnemies(Function f)
	{
		std::for_each(m_Enemies.begin(), m_Enemies.end(), f);
	}

	void SetKeySpawned(bool value);
private:
	Player* m_pPlayer;
	std::vector<Enemy*> m_Enemies;

	void SpawnLoot(Point2f pos);
	bool m_KeySpawned;

	static const int m_LootSpawnChance;
};
