#include "pch.h"
#include "Spiny.h"
#include "Managers.h"

Spiny::Spiny(const Point2f& bottomLeft)
	: Enemy{ new Sprite("Resources/Enemies/Spiny.png", 4, 1, .5f), new Sprite("Resources/Enemies/Spiny_Death.png", 3, 1, 0.3f), bottomLeft, 75.f }
{
	m_pDeathSprite->Loop(1);
}

Spiny::~Spiny()
{
	Managers::GetInstance()->GetSoundManager()->PlaySoundEffect(SoundManager::Soundfx::SpinyDeath);
}

bool Spiny::JumpedOnByPlayer() const
{
	return false;
}
