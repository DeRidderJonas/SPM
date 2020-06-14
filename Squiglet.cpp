#include "pch.h"
#include "Squiglet.h"
#include "Managers.h"
#include <iostream>

Squiglet::Squiglet(const Point2f& bottomLeft)
	: Enemy(new Sprite("Resources/Enemies/Squiglet.png", 4, 1, .075f), new Sprite("Resources/Enemies/Squiglet_Death.png", 3, 1, 0.3f), bottomLeft, 200.f, 0.5f)
{
	m_pDeathSprite->Loop(1);
}

Squiglet::~Squiglet()
{
	Managers::GetInstance()->GetSoundManager()->PlaySoundEffect(SoundManager::Soundfx::EnemyDeath);
}

void Squiglet::Attack()
{
	bool canAttack{ m_RemainingCooldownSec <= 0.f && m_RemainingFrozenSec <= 0.f && (m_GameState == GameState::MovingLeft || m_GameState == GameState::MovingRight) };
	if (canAttack)
	{
		m_GameState = GameState::Attacking;
		m_RemainingCooldownSec = m_AttackCooldown;
		m_RemainingAttackSec = m_AttackDuration;
		Managers::GetInstance()->GetProjectileManager()->Spawn(this, m_FacingLeft);
		Managers::GetInstance()->GetSoundManager()->PlaySoundEffect(SoundManager::Soundfx::SquigletAttack);
	}
}

bool Squiglet::JumpedOnByPlayer() const
{
	return true;
}
