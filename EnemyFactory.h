#pragma once
#include "Enemy.h"
#include "SpriteManager.h"

class EnemyFactory
{
public:
	static Enemy* CreateEnemy(Enemy::Type type, const Rectf& spawnBox);
};

