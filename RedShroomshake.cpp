#include "pch.h"
#include "RedShroomshake.h"
#include "Managers.h"

const int RedShroomshake::m_HealthRestore{ 10 }; //if edited, also edit the description in TextManager

RedShroomshake::RedShroomshake(Texture* pTexture, Texture* pName, Texture* pDescription, const Point2f& bottomLeft, Player* pPlayer)
	: Item{Item::Type::RedShroomshake, 75, pTexture, pName, pDescription, bottomLeft, pPlayer}
{
}

void RedShroomshake::Use() const
{
	m_pPlayer->RestoreHealth(m_HealthRestore);
	Managers::GetInstance()->GetSoundManager()->PlaySoundEffect(SoundManager::Soundfx::Shroomshake);
}
