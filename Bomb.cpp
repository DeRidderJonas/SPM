#include "pch.h"
#include "Bomb.h"
#include "utils.h"
#include "Managers.h"

const float Bomb::m_SecTillExplode{ 2.f };
const float Bomb::m_SecExploding{ 2.f };

Bomb::Bomb(const Sentient* pOwner, Sprite* pSpriteNormal, Sprite* pSpriteExploding)
	: Projectile(pOwner, pSpriteNormal, ProjectileType::Bomb, false)
	, m_pExploding{pSpriteExploding}
	, m_AccSec{0.f}
	, m_IsExploding{false}
{
	m_Hitbox.width = pSpriteExploding->GetFrameWidth();
	m_Hitbox.height = pSpriteExploding->GetFrameHeight();
	Managers::GetInstance()->GetSoundManager()->PlaySoundEffect(SoundManager::Soundfx::BoomerFuse);
}

bool Bomb::Update(const Level* pLevel, float elapsedSec)
{
	m_AccSec += (m_IsExploding ? -1 : 1) * elapsedSec;
	if (m_AccSec >= m_SecTillExplode)
	{
		//explode
		m_IsExploding = true;
		m_AccSec = m_SecExploding;
		m_pExploding->Loop(1);
		Managers::GetInstance()->GetSoundManager()->PlaySoundEffect(SoundManager::Soundfx::BoomerExplode);
	}

	if (m_IsExploding) m_pExploding->Update(elapsedSec);

	if (m_AccSec < 0.f)
	{
		return true;
	}

	Projectile::Update(pLevel, elapsedSec);
	return false;
}

void Bomb::Draw() const
{
	if (!m_IsExploding) m_pSprite->Draw(m_Hitbox);
	else m_pExploding->Draw(m_Hitbox);
}

bool Bomb::IsOverlapping(const GameObject* other) const
{
	if (!m_IsExploding) return false;

	return utils::IsOverlapping(other->GetHitbox(), m_Hitbox);
}


