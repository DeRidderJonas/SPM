#include "pch.h"
#include "EnemyFactory.h"
#include "Managers.h"
#include "Goomba.h"
#include "Spiny.h"
#include "Squiglet.h"

Enemy* EnemyFactory::CreateEnemy(Enemy::Type type, const Rectf& spawnBox)
{
	float randX{ (rand() % int(spawnBox.width)) + spawnBox.left };
	float randY{ (rand() % int(spawnBox.height)) + spawnBox.bottom };
	Point2f randPos{ randX, randY };

	switch (type)
	{
	case Enemy::Type::Goomba:
		return new Goomba(randPos);
		break;
	case Enemy::Type::Spiny:
		return new Spiny(randPos);
		break;
	case Enemy::Type::Squiglet:
		return new Squiglet(randPos);
		break;
	}
	return nullptr;
}