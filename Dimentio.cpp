#include "pch.h"
#include "Dimentio.h"
#include "Managers.h"

Dimentio::Dimentio(const Point2f& bottomLeft, Player* pPlayer, const Rectf& containmentField)
	: Sentient(bottomLeft)
	, m_pPlayer{pPlayer}
	, m_pSprite{Managers::GetInstance()->GetSpriteManager()->GetSprite(SpriteManager::SpriteType::Dimentio)}
	, m_pAttacking{Managers::GetInstance()->GetSpriteManager()->GetSprite(SpriteManager::SpriteType::DimentioAttack)}
	, m_pAttacked{Managers::GetInstance()->GetSpriteManager()->GetSprite(SpriteManager::SpriteType::DimentioAttacked)}
	, m_pDeath{Managers::GetInstance()->GetSpriteManager()->GetSprite(SpriteManager::SpriteType::DimentioDeath)}
	, m_ProjectileSpeed{200.f}
	, m_Movementspeed{300.f,250.f}
	, m_NextLocation{bottomLeft}
	, m_Health{9}
	, m_ContainmentField{containmentField}
{
	m_Hitbox.width = m_pSprite->GetFrameWidth();
	m_Hitbox.height = m_pSprite->GetFrameHeight();
}

void Dimentio::Attack()
{
	bool canAttack{m_RemainingCooldownSec <= 0.f || m_RemainingFrozenSec <= 0.f};
	if (!canAttack) return;

	m_RemainingAttackSec = m_AttackDuration;
	m_RemainingCooldownSec = m_AttackCooldown;
	m_GameState = GameState::Attacking;

	Rectf playerHitbox{ m_pPlayer->GetHitbox() };
	Point2f playerCenter{ playerHitbox.left + playerHitbox.width / 2, playerHitbox.bottom + playerHitbox.height / 2 };

	Point2f bossCenter{ m_Hitbox.left + m_Hitbox.width / 2, m_Hitbox.bottom + m_Hitbox.height / 2 };
	Vector2f projVelocity{ bossCenter, playerCenter };
	projVelocity = projVelocity.Normalized();
	projVelocity *= m_ProjectileSpeed;

	Projectile* pProj{ Managers::GetInstance()->GetProjectileManager()->Spawn(this, projVelocity.x < 0.f, Projectile::ProjectileType::CherbilProjectile) };
	pProj->SetIsVerticalSpeedConst(false);
	pProj->SetVelocity(projVelocity);

	Managers::GetInstance()->GetSoundManager()->PlaySoundEffect(SoundManager::Soundfx::CherbilAttack);

	Enemy::Type randType{ Enemy::Type(rand() % int(Enemy::Type::Cherbil)) };
	Managers::GetInstance()->GetEnemyManager()->Spawn(randType, 1, m_Hitbox);
}

void Dimentio::Update(float elapsedSec, const Level* pLevel)
{
	if (m_RemainingCooldownSec <= 0.f && m_GameState != GameState::Attacked) Attack();

	Vector2f toNewLocation{ Point2f{m_Hitbox.left + m_Hitbox.width / 2, m_Hitbox.bottom + m_Hitbox.height / 2}, m_NextLocation };
	
	float distance{ toNewLocation.Length() };
	if (distance <= m_NewLocationErrorMargin || abs(toNewLocation.x) <= 0.1f || abs(toNewLocation.y) <= 0.1f) RandomiseNewLocation();

	if (m_Hitbox.left <= m_ContainmentField.left || m_Hitbox.left + m_Hitbox.width >= m_ContainmentField.left + m_ContainmentField.width
		|| m_Hitbox.bottom <= m_ContainmentField.bottom || m_Hitbox.bottom + m_Hitbox.height >= m_ContainmentField.bottom + m_ContainmentField.height) RandomiseNewLocation();

	toNewLocation = toNewLocation.Normalized();
	toNewLocation.x *= m_Movementspeed.x;
	toNewLocation.y *= m_Movementspeed.y;
	m_Velocity = toNewLocation;

	if (m_GameState == GameState::Attacking) m_pAttacking->Update(elapsedSec);
	else if (m_GameState == GameState::Attacked)
	{
		m_pAttacked->Update(elapsedSec);
		if (m_pAttacked->HasEnded())
		{
			m_GameState = GameState::Idle;
			m_pAttacked->SetFrame(0);
		}
	}
	else if (m_GameState == GameState::Dying) m_pDeath->Update(elapsedSec);
	else m_pSprite->Update(elapsedSec);
	Sentient::Update(elapsedSec, pLevel);
}

void Dimentio::Draw() const
{
	if (m_GameState == GameState::Attacking) m_pAttacking->Draw(m_Hitbox);
	else if (m_GameState == GameState::Attacked) m_pAttacked->Draw(m_Hitbox);
	else if (m_GameState == GameState::Dying) m_pDeath->Draw(m_Hitbox);
	else m_pSprite->Draw(m_Hitbox);
}

void Dimentio::Attacked()
{
	m_Health--;
	m_GameState = GameState::Attacked;
	Managers::GetInstance()->GetSoundManager()->PlaySoundEffect(SoundManager::Soundfx::DimentioAttacked);
	if (m_Health <= 0) Die();
	
}

void Dimentio::Die()
{
	m_GameState = GameState::Dying;
	Managers::GetInstance()->GetEnemyManager()->SetKeySpawned(false);
	Managers::GetInstance()->GetItemManager()->Spawn(Item::Type::Key, Point2f{ m_Hitbox.left, m_Hitbox.bottom });
}

bool Dimentio::IsDead() const
{
	return m_pDeath->HasEnded();
}

void Dimentio::RandomiseNewLocation()
{
	const float verticalMargin{ 50.f };
	const float horizontalMargin{ 100.f };

	m_NextLocation.x = float(rand() % int(m_ContainmentField.width - horizontalMargin) + int(m_ContainmentField.left + horizontalMargin / 2));
	m_NextLocation.y = float(rand() % int(m_ContainmentField.height - verticalMargin) + int(m_ContainmentField.bottom + verticalMargin / 2));
}
