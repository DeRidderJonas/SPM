#include "pch.h"
#include "IceStorm.h"
#include "Managers.h"

IceStorm::IceStorm(Texture* pTexture, Texture* pName, Texture* pDescription, const Point2f& bottomLeft)
	: Item(Item::Type::PowerBlock, 100, pTexture, pName, pDescription, bottomLeft)
{
}

void IceStorm::Use() const
{
	EnemyManager* em{ Managers::GetInstance()->GetEnemyManager() };
	auto randomFreeze = [&em](Enemy* enemy) {
		int chanceToFreeze{ 66 };
		int randChance{ rand() % 100 };
		if (randChance <= chanceToFreeze) enemy->Freeze();
	};

	em->ApplyOnEnemies(randomFreeze);
}
