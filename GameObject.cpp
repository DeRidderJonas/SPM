#include "pch.h"
#include "GameObject.h"
#include "utils.h"
#include <iostream>

const float GameObject::m_FallAcceleration{ 900.f };
const float GameObject::m_MaxFallSpeed{ 1900.f };

GameObject::GameObject(const Point2f& bottomLeft, bool IsHorizontalSpeedConstant, bool IsVerticalSpeedConstant)
	: m_Hitbox{ bottomLeft.x, bottomLeft.y, 0.f, 0.f }
	, m_GameState{GameState::Idle}
	, m_Velocity{0.f,0.f}
	, m_IsHorizontalSpeedConstant{IsHorizontalSpeedConstant}
	, m_IsVerticalSpeedConstant{IsVerticalSpeedConstant}
{
}

void GameObject::Update(float elapsedSec, const Level* level)
{
	//Stick GameObject to level boundaries
	if (m_Hitbox.left <= level->GetBoundaries().left) m_Hitbox.left = level->GetBoundaries().left;
	if (m_Hitbox.left + m_Hitbox.width >= level->GetBoundaries().left + level->GetBoundaries().width) 
		m_Hitbox.left = level->GetBoundaries().left + level->GetBoundaries().width - m_Hitbox.width;


	//GameObject cant move when attacking
	if (m_GameState == GameState::Attacking) {
		return;
	}

	//GameObject is moving vertically
	if (!m_IsVerticalSpeedConstant && abs(m_Velocity.y) > 0.0001f)
	{
		m_Hitbox.bottom += m_Velocity.y * elapsedSec;
		
		m_Velocity.y -= elapsedSec * m_FallAcceleration;
		if (m_GameState != GameState::Attacked && m_Velocity.y < 0.f) m_GameState = GameState::Falling;
		if (m_Velocity.y < -m_MaxFallSpeed) m_Velocity.y = -m_MaxFallSpeed;

		level->HandleCollision(m_Hitbox, m_Velocity);
		if (level->IsOnGround(m_Hitbox))
		{
			m_GameState = GameState::Idle;
		}
	}

	if (!m_IsVerticalSpeedConstant && !level->IsOnGround(m_Hitbox) && abs(m_Velocity.y) < 0.0001f)
	{
		m_Velocity.y -= elapsedSec * m_FallAcceleration;
	}

	//GameObject is moving vertically
	m_Hitbox.left += elapsedSec * m_Velocity.x;

	if (m_IsHorizontalSpeedConstant) return;
	m_Velocity.x += (m_GameState == GameState::MovingLeft ? 1 : -1) * elapsedSec * m_Velocity.x;

	if (abs(m_Velocity.x) < 0.0001f) m_Velocity.x = 0.f;
}

bool GameObject::IsOverlapping(const GameObject* other) const
{
	return IsOverlapping(other->GetHitbox());
}

bool GameObject::IsOverlapping(const Rectf& other) const
{
	return utils::IsOverlapping(m_Hitbox, other);
}

Vector2f GameObject::GetVelocity() const
{
	return m_Velocity;
}

void GameObject::SetVelocity(const Vector2f& velocity)
{
	if (m_GameState == GameState::Attacked) return;
	
	m_Velocity = velocity;

	if (abs(m_Velocity.y) > 0.f) m_GameState = GameState::Falling;
	else
	{
		if (abs(m_Velocity.x) < 0.0001f) m_GameState = GameState::Idle;
		else m_GameState = m_Velocity.x < 0.f ? GameState::MovingLeft : GameState::MovingRight;
	}
}

void GameObject::SetHorizontalVelocity(float Vx)
{
	SetVelocity(Vector2f{ Vx, m_Velocity.y });
}

void GameObject::SetVerticalVelocity(float Vy)
{
	SetVelocity(Vector2f{ m_Velocity.x, Vy });
}

void GameObject::AddVelocity(const Vector2f& force)
{
	m_Velocity += force;

	if (abs(m_Velocity.y) > 0.f) m_GameState = GameState::Falling;
}

void GameObject::SetPosition(const Point2f& pos)
{
	m_Hitbox.left = pos.x;
	m_Hitbox.bottom = pos.y;
}

Rectf GameObject::GetHitbox() const
{
	return m_Hitbox;
}
