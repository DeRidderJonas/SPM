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
	, m_AmountOfCoins{0}
	, m_LastPressed{SDL_SCANCODE_0}
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
		if (m_GameState == GameState::Attacking && m_ActivePixl.GetType() == Pixl::Type::Boomer) glTranslatef(-0.45f*GetHitbox().width, 0, 0);
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

	if (m_RemainingFrozenSec > 0.f)
	{
		Managers::GetInstance()->GetTextureManager()->GetTexture(TextureManager::TextureType::Frozen)->Draw(GetHitbox());
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

	if (m_CurrentHealth <= 0) Die();

	else Managers::GetInstance()->GetSoundManager()->PlaySoundEffect(SoundManager::Soundfx::Damage);
}

bool Player::IsInvincible() const
{
	return m_RemainingInvincibleSec > 0.f;
}

void Player::Die()
{
	SetActiveSprite(SpriteManager::SpriteType::MarioDeath);
	m_GameState = GameState::Dying;
	Managers::GetInstance()->GetSoundManager()->PlaySoundEffect(SoundManager::Soundfx::GameOver);
}

bool Player::IsDead() const
{
	return Managers::GetInstance()->GetSpriteManager()->GetSprite(SpriteManager::SpriteType::MarioDeath)->HasEnded();
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

void Player::HealthFromLoad(int current, int max)
{
	m_CurrentHealth = current;
	m_MaxHealth = max;
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

	if (m_GameState == GameState::Attacking && m_ActivePixl.GetType() == Pixl::Type::Boomer)
	{
		Rectf hitbox{ m_Hitbox };
		hitbox.width = 2 * hitbox.width / 3;
		return hitbox;
	}

	return GameObject::GetHitbox();
}

Rectf Player::GetAttackHitbox() const
{
	return m_AttackingHitbox;
}

bool Player::CanCounterProjectiles() const
{
	return m_ActivePixl.GetType() == Pixl::Type::Barry && m_GameState == GameState::Attacking;
}

void Player::AdjustAmountOfCoins(int adjustment)
{
	m_AmountOfCoins += adjustment;
}

int Player::GetAmountOfCoins() const
{
	return m_AmountOfCoins;
}

void Player::OnInput(float elapsedSec)
{
	bool inputReceived{ false };
	bool MarioMidAir{ m_GameState == GameState::Falling };
	bool canMove{ m_GameState != GameState::Attacked && m_GameState != GameState::Attacking && !m_IsPickingUp && m_RemainingFrozenSec < 0.f && m_GameState != GameState::Dying};

	const Uint8* pKeysStates = SDL_GetKeyboardState(nullptr);
	//Go Left
	if (pKeysStates[SDL_SCANCODE_A] && canMove)
	{
		SetHorizontalVelocity(-m_HorizontalSpeed);
		inputReceived = true;
		m_FacingLeft = true;
		if (m_GameState != GameState::Falling) SetActiveSprite(SpriteManager::SpriteType::MarioRun);;
		m_LastPressed = SDL_SCANCODE_A;
	}
	//Go Right
	if (pKeysStates[SDL_SCANCODE_D] && canMove)
	{
		SetHorizontalVelocity(m_HorizontalSpeed);
		inputReceived = true;
		m_FacingLeft = false;
		if (m_GameState != GameState::Falling) SetActiveSprite(SpriteManager::SpriteType::MarioRun);
		m_LastPressed = SDL_SCANCODE_D;
	}
	//Jump
	if (pKeysStates[SDL_SCANCODE_SPACE] && !MarioMidAir && canMove)
	{
		AddVelocity(Vector2f{ 0.f, m_JumpSpeed });
		inputReceived = true;
		SetActiveSprite(SpriteManager::SpriteType::MarioJump);
		m_LastPressed = SDL_SCANCODE_SPACE;
	}
	//Attack
	if (pKeysStates[SDL_SCANCODE_F] && canMove)
	{
		m_LastPressed = SDL_SCANCODE_F;
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

	//Thaw out
	if (m_RemainingFrozenSec > 0.f)
	{
		if (pKeysStates[SDL_SCANCODE_A] && m_LastPressed != SDL_SCANCODE_A)
		{
			m_RemainingFrozenSec -= 0.5f;
			m_LastPressed = SDL_SCANCODE_A;
		}
		if (pKeysStates[SDL_SCANCODE_D] && m_LastPressed != SDL_SCANCODE_D)
		{
			m_RemainingAttackSec -= 0.5f;
			m_LastPressed = SDL_SCANCODE_D;
		}
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
	case Pixl::Type::Boomer:
		SetActiveSprite(SpriteManager::SpriteType::MarioAttackBoomer);
		m_RemainingAttackSec = 0.5f;
		m_RemainingCooldownSec = 6.f;
		break;
	default:
		//No active pixl, revert to falling
		m_GameState = GameState::Falling;
		break;
	}

	m_pActiveSprite->SetFrame(0);
}

void Player::AttackEnded()
{
	if (m_ActivePixl.GetType() == Pixl::Type::Boomer) Managers::GetInstance()->GetProjectileManager()->Spawn(this, m_FacingLeft, Projectile::ProjectileType::Bomb);

	m_GameState = GameState::Idle;
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

