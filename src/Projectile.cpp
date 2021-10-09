#include "pch.h"
#include "Projectile.h"
#include "Sentient.h"

const float Projectile::m_HorizontalSpeed{ 300.f };

Projectile::Projectile(const Sentient* pOwner, Sprite* pSprite, Projectile::ProjectileType type, bool verticalSpeedConst)
	: GameObject(Point2f{pOwner->GetHitbox().left, pOwner->GetHitbox().bottom}, true, verticalSpeedConst)
	, m_pOwner{pOwner}
	, m_pSprite{pSprite}
	, m_Type{type}
{
	SetHorizontalVelocity(m_HorizontalSpeed);
	m_Hitbox.width = m_pSprite->GetFrameWidth();
	m_Hitbox.height = m_pSprite->GetFrameHeight();
	pSprite->SetFrame(0);
}

bool Projectile::Update(const Level* pLevel, float elapsedSec)
{
	GameObject::Update(elapsedSec, pLevel);
	m_pSprite->Update(elapsedSec);
	return false;
}

void Projectile::Draw() const
{
	m_pSprite->Draw(m_Hitbox);
}

bool Projectile::IsOverlapping(const GameObject* other) const
{
	if(other == m_pOwner) return false;
	return GameObject::IsOverlapping(other);
}

void Projectile::SetIsVerticalSpeedConst(bool value)
{
	m_IsVerticalSpeedConstant = value;
}

bool Projectile::BelongsTo(Sentient* pSentient) const
{
	return pSentient == m_pOwner;
}

void Projectile::TransferOwnershipTo(Sentient* pSentient)
{
	m_pOwner = pSentient;
}

bool Projectile::IsActive() const
{
	return true;
}

Projectile::ProjectileType Projectile::GetType() const
{
	return m_Type;
}

const Sentient* Projectile::GetOwner() const
{
	return m_pOwner;
}
