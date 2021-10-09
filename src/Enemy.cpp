#include "pch.h"
#include "Enemy.h"
#include "utils.h"
#include <iostream>
#include "Managers.h"

const float Enemy::m_MaxFlipCooldown{ 1.f };

Enemy::Enemy(Sprite* pSprite, Sprite* pDeathSprite, const Point2f& bottomLeft, float horSpeed, float attackDuration, float attackCooldown)
	: Sentient{ bottomLeft, true, attackDuration, attackCooldown }
	, m_pSprite{pSprite}
	, m_pDeathSprite{pDeathSprite}
	, m_HorizontalSpeed{horSpeed}
	, m_RemainingFlipCooldown{0.f}
{
	m_Hitbox.width = m_pSprite->GetFrameWidth();
	m_Hitbox.height = m_pSprite->GetFrameHeight();
	m_pSprite->Randomize();

	AddVelocity(Vector2f{ 0.f,-1.f });
}

Enemy::~Enemy()
{
	delete m_pSprite;
	delete m_pDeathSprite;
}

void Enemy::Update(float elapsedSec, const Level* level)
{

	switch (m_GameState)
	{
	case GameState::MovingLeft:
		if (canMove(true, level)) SetHorizontalVelocity(-m_HorizontalSpeed);
		else m_GameState = GameState::MovingRight;
		break;
	case GameState::MovingRight:
		if (canMove(false, level)) SetHorizontalVelocity(m_HorizontalSpeed);
		else m_GameState = GameState::MovingLeft;
		break;
	case GameState::Idle:
		m_GameState = (rand() % 2 == 0) ? GameState::MovingLeft : GameState::MovingRight;
		break;
	}

	if (m_RemainingFrozenSec > 0.f) SetHorizontalVelocity(0.f);
	else if (m_GameState != GameState::Dying) m_pSprite->Update(elapsedSec);
	else m_pDeathSprite->Update(elapsedSec);

	m_RemainingFlipCooldown -= elapsedSec;

	Sentient::Update(elapsedSec, level);
}

void Enemy::Draw() const
{
	if (m_FacingLeft || m_GameState == GameState::MovingLeft)
	{
		glPushMatrix();
		glTranslatef(m_Hitbox.left + m_pSprite->GetFrameWidth(), m_Hitbox.bottom, 0.f);
		glScalef(-1, 1, 1);
		if (m_GameState != GameState::Dying) m_pSprite->Draw(Point2f{});
		else m_pDeathSprite->Draw(Point2f{});
		if (m_RemainingFrozenSec > 0.f) Managers::GetInstance()->GetTextureManager()->GetTexture(TextureManager::TextureType::Frozen)->Draw(Rectf{0.f,0.f,m_Hitbox.width,m_Hitbox.height});
		glPopMatrix();
	}
	else
	{
		if (m_GameState != GameState::Dying) m_pSprite->Draw(m_Hitbox);
		else m_pDeathSprite->Draw(m_Hitbox);
		if (m_RemainingFrozenSec > 0.f) Managers::GetInstance()->GetTextureManager()->GetTexture(TextureManager::TextureType::Frozen)->Draw(m_Hitbox);
	}
}

void Enemy::Die()
{
	m_GameState = GameState::Dying;
}

bool Enemy::IsDead() const
{
	return m_pDeathSprite->HasEnded();
}

bool Enemy::IsDying() const
{
	return m_GameState == GameState::Dying;
}

void Enemy::FlipHorizontalVelocity()
{
	if (!CanBeFlipped()) return;
	SetHorizontalVelocity(-m_Velocity.x);
	m_RemainingFlipCooldown = m_MaxFlipCooldown;
	m_GameState = m_Velocity.x > 0.f ? GameState::MovingRight : GameState::MovingLeft;
}

bool Enemy::CanBeFlipped() const
{
	return m_RemainingFlipCooldown < 0.f && m_GameState != GameState::Dying;
}

bool Enemy::canMove(bool toLeft, const Level* plevel) const
{
	Point2f nextStep{ m_Hitbox.left + m_Hitbox.width + 1.f, m_Hitbox.bottom };
	if (toLeft)
	{
		nextStep = Point2f{ m_Hitbox.left - 1.f, m_Hitbox.bottom };
		if(m_Hitbox.left < plevel->GetBoundaries().left) return false;
	}
	else
	{
		if (m_Hitbox.left + m_Hitbox.width > plevel->GetBoundaries().left + plevel->GetBoundaries().width) return false;
	}

	if (plevel->IsNextToWall(m_Hitbox)) return false;
	
	return plevel->IsOnGround(Rectf{ nextStep.x,nextStep.y, m_Hitbox.width, m_Hitbox.height });
}