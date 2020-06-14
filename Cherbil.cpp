#include "pch.h"
#include "Cherbil.h"
#include "Managers.h"

Cherbil::Cherbil(const Point2f& bottomLeft)
	: Enemy(new Sprite("Resources/Enemies/Cherbil.png", 2, 1, 0.3f), new Sprite("Resources/Enemies/Cherbil_Death.png", 3, 1, 0.3f), bottomLeft, 175.f)
{
	m_pDeathSprite->Loop(1);
}

Cherbil::~Cherbil()
{
	Managers::GetInstance()->GetSoundManager()->PlaySoundEffect(SoundManager::Soundfx::CherbilDeath);
}

void Cherbil::Attack()
{
	bool canAttack{ m_RemainingCooldownSec <= 0.f && m_RemainingFrozenSec <= 0.f && (m_GameState == GameState::MovingLeft || m_GameState == GameState::MovingRight) };
	if (canAttack)
	{
		m_GameState = GameState::Attacking;
		m_RemainingCooldownSec = m_AttackCooldown;
		m_RemainingAttackSec = m_AttackDuration;
		Managers::GetInstance()->GetProjectileManager()->Spawn(this, m_FacingLeft, Projectile::ProjectileType::CherbilProjectile);
		Managers::GetInstance()->GetSoundManager()->PlaySoundEffect(SoundManager::Soundfx::CherbilAttack);
	}
}

bool Cherbil::JumpedOnByPlayer() const
{
	return true;
}
