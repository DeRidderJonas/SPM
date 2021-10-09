#include "pch.h"
#include "Heart.h"
#include "Managers.h"
#include "utils.h"

Heart::Heart(const Point2f& bottomLeft)
	: m_Hitbox{bottomLeft.x, bottomLeft.y, 0.f, 0.f}
	, m_pBurning{Managers::GetInstance()->GetSpriteManager()->GetSprite(SpriteManager::SpriteType::BurningHeart)}
	, m_pStone{Managers::GetInstance()->GetSpriteManager()->GetSprite(SpriteManager::SpriteType::StoneHeart)}
	, m_pVulnerable{Managers::GetInstance()->GetSpriteManager()->GetSprite(SpriteManager::SpriteType::VulnerableHeart)}
	, m_pBroken{Managers::GetInstance()->GetSpriteManager()->GetSprite(SpriteManager::SpriteType::BrokenHeart)}
	, m_State{State::Burning}
{
	m_Hitbox.width = m_pBurning->GetFrameWidth();
	m_Hitbox.height = m_pBurning->GetFrameHeight();
}

void Heart::Update(float elapsedSec)
{
	switch (m_State)
	{
	case Heart::State::Burning:
		m_pBurning->Update(elapsedSec);
		break;
	case Heart::State::Stone:
		m_pStone->Update(elapsedSec);
		break;
	case Heart::State::Vulnerable:
		m_pVulnerable->Update(elapsedSec);
		break;
	case Heart::State::Broken:
		m_pBroken->Update(elapsedSec);
		break;
	}
}

void Heart::Draw() const
{
	switch (m_State)
	{
	case Heart::State::Burning:
		m_pBurning->Draw(m_Hitbox);
		break;
	case Heart::State::Stone:
		m_pStone->Draw(m_Hitbox);
		break;
	case Heart::State::Vulnerable:
		m_pVulnerable->Draw(m_Hitbox);
		break;
	case Heart::State::Broken:
		m_pBroken->Draw(m_Hitbox);
		break;
	}
}

bool Heart::WasHit(const Rectf& hitbox, AttackType attackType)
{
	bool wasHit{ utils::IsOverlapping(m_Hitbox, hitbox) };

	if (wasHit)
	{
		switch (attackType)
		{
		case Heart::AttackType::Frozen:
			if (m_State == State::Burning) m_State = State::Stone;
			else wasHit = false;
			break;
		case Heart::AttackType::Bomb:
			if (m_State == State::Stone) m_State = State::Vulnerable;
			else wasHit = false;
			break;
		case Heart::AttackType::Hammer:
			if (m_State == State::Vulnerable) m_State = State::Broken;
			else wasHit = false;
			break;
		case Heart::AttackType::Unaffected:
			wasHit = false;
			break;
		}
	}

	return wasHit;
}

bool Heart::IsBroken() const
{
	return m_State == State::Broken;
}
