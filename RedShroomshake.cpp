#include "pch.h"
#include "RedShroomshake.h"

const int RedShroomshake::m_HealthRestore{ 10 }; //if edited, also edit the description in TextManager

RedShroomshake::RedShroomshake(Texture* pTexture, Texture* pName, Texture* pDescription, Point2f bottomLeft, Player* pPlayer)
	: Item{Item::Type::RedShroomshake, pTexture, pName, pDescription, bottomLeft, pPlayer}
{
}

void RedShroomshake::Use()
{
	m_pPlayer->RestoreHealth(m_HealthRestore);
}
