#include "pch.h"
#include "Player.h"
#include "Managers.h"
#include "utils.h"
#include <iostream>

const float Player::m_HorizontalSpeed{ 200.f };
const float Player::m_JumpSpeed{ 700.f };
const float Player::m_MaxInvincibleSec{ 2.f };

Player::Player(const Point2f& bottomLeft)
	: Sentient{bottomLeft, false, .25f, .5f}
	, m_pActiveSprite{Managers::GetInstance()->GetSpriteManager()->GetSprite(SpriteManager::SpriteType::MarioRun)}
	, m_pInvincibleBubble{ Managers::GetInstance()->GetSpriteManager()->GetSprite(SpriteManager::SpriteType::InvincibleBubble)}
	, m_MaxHealth{10}
	, m_CurrentHealth{10}
	, m_RemainingInvincibleSec{0.f}
	, m_ActivePixl{Pixl::Type::Cudge}
	, m_AttackingHitbox{}
	, m_IsPickingUp{false}
{
	m_Hitbox.width = m_pActiveSprite->GetFrameWidth();
	m_Hitbox.height = m_pActiveSprite->GetFrameHeight();
}

Player::~Player()
{
}

void Player::Update(float elapsedSec, const Level* level)
{
	Sentient::Update(elapsedSec, level);
	OnInput(elapsedSec);

	m_pActiveSprite->Update(elapsedSec);
	m_ActivePixl.Update(elapsedSec);
	

	if (IsInvincible())
	{
		m_RemainingInvincibleSec -= elapsedSec;
		m_pInvincibleBubble->Update(elapsedSec);
	}

	if (m_RemainingAttackSec < 0.f)
	{
		m_AttackingHitbox = Rectf{};
	}
	//PrintGameState();
}

void Player::Draw() const
{
	if (m_FacingLeft || m_GameState == GameState::MovingLeft)
	{
		glPushMatrix();
		glTranslatef(m_Hitbox.left + m_pActiveSprite->GetFrameWidth(), m_Hitbox.bottom, 0.f);
		if (m_GameState == GameState::Attacking && m_ActivePixl.GetType() == Pixl::Type::Cudge) glTranslatef(-GetHitbox().width, 0, 0);
		if (m_GameState == GameState::Attacking && m_ActivePixl.GetType() == Pixl::Type::Barry) glTranslatef(-GetHitbox().width/2, 0, 0);
		glScalef(-1, 1, 1);
		m_pActiveSprite->Draw(Point2f{});
		glPopMatrix();
	}
	else
	{
		Rectf hitbox{ m_Hitbox };
		if (m_GameState == GameState::Attacking && m_ActivePixl.GetType() == Pixl::Type::Cudge) hitbox.left -= GetHitbox().width;
		if (m_GameState == GameState::Attacking && m_ActivePixl.GetType() == Pixl::Type::Barry) hitbox.left -= GetHitbox().width/2;
		m_pActiveSprite->Draw(hitbox);
	}

	if (IsInvincible())
	{
		m_pInvincibleBubble->Draw(GetHitbox());
	}

	const float margin{ 5.f };
	m_ActivePixl.Draw(Point2f{ m_Hitbox.left - m_ActivePixl.GetWidth() - margin, m_Hitbox.bottom + m_Hitbox.height + margin - m_ActivePixl.GetHeight()/2 });
}

void Player::WasHit(bool fromLeft)
{
	if (IsInvincible()) return;

	SetVerticalVelocity(0.f);
	const float horForce{ 500.f };
	const float verForce{ 300.f };
	Vector2f force{(fromLeft ? -1 : 1) * horForce, verForce};
	AddVelocity(force);

	m_CurrentHealth--;
	m_GameState = GameState::Attacked;
	m_RemainingInvincibleSec += m_MaxInvincibleSec;
	SetActiveSprite(SpriteManager::SpriteType::MarioDamaged);

	m_AttackingHitbox = Rectf{};
}

bool Player::IsInvincible() const
{
	return m_RemainingInvincibleSec > 0.f;
}

int Player::GetMaxHealth() const
{
	return m_MaxHealth;
}

int Player::GetCurrentHealth() const
{
	return m_CurrentHealth;
}

void Player::RestoreHealth(int amount)
{
	m_CurrentHealth += amount;
	if (m_CurrentHealth > m_MaxHealth) m_CurrentHealth = m_MaxHealth;
}

void Player::SetIsPickingUp(bool value)
{
	m_IsPickingUp = value;
	if (m_IsPickingUp)
	{
		SetActiveSprite(SpriteManager::SpriteType::MarioPickup);
		m_Velocity.x = 0.f;
	}
}

void Player::SetActivePixlType(Pixl::Type type)
{
	m_ActivePixl.SetType(type);
}

Rectf Player::GetHitbox() const
{
	if (m_GameState == GameState::Attacking && m_ActivePixl.GetType() == Pixl::Type::Cudge)
	{
		Rectf hitbox{ m_Hitbox };
		hitbox.width = hitbox.width / 3;
		return hitbox;
	}

	if (m_GameState == GameState::Attacking && m_ActivePixl.GetType() == Pixl::Type::Barry)
	{
		Rectf hitbox{ m_Hitbox };
		hitbox.width = hitbox.width / 2;
		return hitbox;
	}

	return GameObject::GetHitbox();
}

Rectf Player::GetAttackHitbox() const
{
	return m_AttackingHitbox;
}

void Player::OnInput(float elapsedSec)
{
	bool inputReceived{ false };
	bool MarioMidAir{ m_GameState == GameState::Falling };
	bool canMove{ m_GameState != GameState::Attacked && m_GameState != GameState::Attacking && !m_IsPickingUp };

	const Uint8* pKeysStates = SDL_GetKeyboardState(nullptr);
	//Go Left
	if (pKeysStates[SDL_SCANCODE_A] && canMove)
	{
		SetHorizontalVelocity(-m_HorizontalSpeed);
		inputReceived = true;
		m_FacingLeft = true;
		if (m_GameState != GameState::Falling) SetActiveSprite(SpriteManager::SpriteType::MarioRun);;
	}
	//Go Right
	if (pKeysStates[SDL_SCANCODE_D] && canMove)
	{
		SetHorizontalVelocity(m_HorizontalSpeed);
		inputReceived = true;
		m_FacingLeft = false;
		if (m_GameState != GameState::Falling) SetActiveSprite(SpriteManager::SpriteType::MarioRun);
	}
	//Jump
	if (pKeysStates[SDL_SCANCODE_SPACE] && !MarioMidAir && canMove)
	{
		AddVelocity(Vector2f{ 0.f, m_JumpSpeed });
		inputReceived = true;
		SetActiveSprite(SpriteManager::SpriteType::MarioJump);
	}
	//Attack
	if (pKeysStates[SDL_SCANCODE_F] && canMove)
	{
		Attack();
		if (m_GameState != GameState::Attacking) SetActiveSprite(SpriteManager::SpriteType::MarioIdle);
		return;
	}

	//Reset to Idle
	if (!inputReceived && !MarioMidAir && canMove)
	{
		SetHorizontalVelocity(0.f);
		SetActiveSprite(SpriteManager::SpriteType::MarioIdle);
	}
}

void Player::Attack()
{
	bool canAttack{ m_GameState != GameState::Attacking && m_GameState != GameState::Attacked && m_GameState != GameState::Falling && m_RemainingCooldownSec < 0.f };
	if (!canAttack) return;
	m_GameState = GameState::Attacking;
	m_RemainingAttackSec = m_AttackDuration;
	m_RemainingCooldownSec = m_AttackCooldown;
	
	switch (m_ActivePixl.GetType())
	{
	case Pixl::Type::Cudge:
		SetActiveSprite(SpriteManager::SpriteType::MarioAttackCudge);
		m_AttackingHitbox = Rectf{m_Hitbox.left + m_pActiveSprite->GetFrameWidth()/2, m_Hitbox.bottom, m_pActiveSprite->GetFrameWidth() / 2, m_pActiveSprite->GetFrameHeight()};
		if (m_FacingLeft) m_AttackingHitbox.left = m_Hitbox.left - m_pActiveSprite->GetFrameWidth() / 2;
		break;
	case Pixl::Type::Barry:
		SetActiveSprite(SpriteManager::SpriteType::MarioAttackBarry);
		m_AttackingHitbox = Rectf{ m_Hitbox.left - m_pActiveSprite->GetFrameWidth()/4, m_Hitbox.bottom
			, m_pActiveSprite->GetFrameWidth(), m_pActiveSprite->GetFrameHeight() };
		m_RemainingAttackSec = 1.f;
		break;
	default:
		//No active pixl, revert to falling
		m_GameState = GameState::Falling;
		break;
	}

	m_pActiveSprite->SetFrame(0);
}

void Player::SetActiveSprite(SpriteManager::SpriteType spriteType)
{
	m_pActiveSprite = Managers::GetInstance()->GetSpriteManager()->GetSprite(spriteType);
	m_Hitbox.width = m_pActiveSprite->GetFrameWidth();
	m_Hitbox.height = m_pActiveSprite->GetFrameHeight();
}

void Player::PrintGameState()
{
	switch (m_GameState)
	{
	case GameState::Idle:
		std::cout << "Idle" << '\n';
		break;
	case GameState::MovingLeft:
		std::cout << "MovingLeft" << '\n';
		break;
	case GameState::MovingRight:
		std::cout << "MovingRight" << '\n';
		break;
	case GameState::Falling:
		std::cout << "Falling" << '\n';
		break;
	case GameState::Attacked:
		std::cout << "Attacked" << '\n';
		break;
	case GameState::Attacking:
		std::cout << "Attacking" << '\n';
		break;
	default:
		break;
	}
}

