#include "pch.h"
#include "Sentient.h"
#include <iostream>

Sentient::Sentient(const Point2f& bottomLeft, bool IsHorizontalSpeedConstant, float attackDuration, float attackCooldown)
	: GameObject(bottomLeft, IsHorizontalSpeedConstant)
	, m_AttackDuration{attackDuration}
	, m_RemainingAttackSec{0.f}
	, m_AttackCooldown{attackCooldown}
	, m_RemainingCooldownSec{0.f}
	, m_FacingLeft{false}
{
}

void Sentient::Attack()
{
}

void Sentient::AttackEnded()
{
	m_GameState = m_FacingLeft ? GameState::MovingLeft : GameState::MovingRight;
}

void Sentient::Update(float elapsedSec, const Level* level)
{
	if (m_GameState == GameState::MovingLeft) m_FacingLeft = true;
	if (m_GameState == GameState::MovingRight) m_FacingLeft = false;

	if (m_GameState == GameState::Attacking) {
		m_RemainingAttackSec -= elapsedSec;
		if (m_RemainingAttackSec <= 0.f) AttackEnded();
		return;
	}
	else
	{
		m_RemainingCooldownSec -= elapsedSec;
		GameObject::Update(elapsedSec, level);
	}
}

Rectf Sentient::GetHitbox() const
{
	return GameObject::GetHitbox();
}
