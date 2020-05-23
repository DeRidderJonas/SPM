#include "pch.h"
#include "PowerBlock.h"
#include "Managers.h"
#include <iostream>

PowerBlock::PowerBlock(Texture* pTexture, Texture* pName, Texture* pDescription, const Point2f& bottomLeft)
	: Item(Item::Type::PowerBlock, pTexture, pName, pDescription, bottomLeft)
{
}

void PowerBlock::Use()
{
	EnemyManager* em{ Managers::GetInstance()->GetEnemyManager() };
	auto killRandom = [&em](Enemy* pEnemy) {
		int chanceToKill{ 50 };
		int randomChance{ rand() % 100 };
		if (chanceToKill >= randomChance) em->Kill(pEnemy);
	};
	em->ApplyOnEnemies(killRandom);
}
