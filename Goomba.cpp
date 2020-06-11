#include "pch.h"
#include "Goomba.h"
#include "Managers.h"

Goomba::Goomba(const Point2f& bottomLeft)
	: Enemy{ new Sprite("Resources/Enemies/Goomba.png", 4, 2, 0.15f), new Sprite("Resources/Enemies/Goomba_Death.png", 3, 1, 0.3f), bottomLeft }
{
}

Goomba::~Goomba()
{
	Managers::GetInstance()->GetSoundManager()->PlaySoundEffect(SoundManager::Soundfx::EnemyDeath);
}

bool Goomba::JumpedOnByPlayer() const
{
	return true;
}
