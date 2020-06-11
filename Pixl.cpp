#include "pch.h"
#include "Pixl.h"
#include "Managers.h"

Pixl::Pixl(Type type)
	: m_Type{type}
{
	SetType(type);
}

void Pixl::Update(float elapsedSec)
{
	m_pSprite->Update(elapsedSec);
}

void Pixl::Draw(const Point2f& bottomLeft) const
{
	m_pSprite->Draw(bottomLeft);
}

void Pixl::DrawInMenu(const Rectf& menuRect, bool drawDesc, const Rectf& descRect) const
{
	Rectf pixlRect{ menuRect };
	float pixlWidth{ pixlRect.width / 7 };
	pixlRect.width = pixlWidth;
	m_pSprite->SetFrame(0);
	m_pSprite->Draw(pixlRect);
	Point2f nameBottomLeft{ menuRect.left + pixlWidth + 5.f, menuRect.bottom + m_pNameTexture->GetHeight() / 4 };
	m_pNameTexture->Draw(nameBottomLeft);

	if (!drawDesc) return;

	m_pDescriptionTexture->Draw(descRect);
}

void Pixl::DrawInTitleScreen(const Rectf& drawRect)
{
	m_pSprite->Draw(drawRect);
}

void Pixl::SetType(Type newType)
{
	m_Type = newType;
	switch (newType)
	{
	case Pixl::Type::Cudge:
		m_pSprite = Managers::GetInstance()->GetSpriteManager()->GetSprite(SpriteManager::SpriteType::Cudge);
		m_pNameTexture = Managers::GetInstance()->GetTextManager()->GetTexture(TextManager::Text::Cudge);
		m_pDescriptionTexture = Managers::GetInstance()->GetTextManager()->GetTexture(TextManager::Text::CudgeDesc);
		break;
	case Pixl::Type::Barry:
		m_pSprite = Managers::GetInstance()->GetSpriteManager()->GetSprite(SpriteManager::SpriteType::Barry);
		m_pNameTexture = Managers::GetInstance()->GetTextManager()->GetTexture(TextManager::Text::Barry);
		m_pDescriptionTexture = Managers::GetInstance()->GetTextManager()->GetTexture(TextManager::Text::BarryDesc);
		break;
	case Pixl::Type::Boomer:
		m_pSprite = Managers::GetInstance()->GetSpriteManager()->GetSprite(SpriteManager::SpriteType::Boomer);
	default:
		break;
	}
}

Pixl::Type Pixl::GetType() const
{
	return m_Type;
}

float Pixl::GetWidth() const
{
	return m_pSprite->GetFrameWidth();
}

float Pixl::GetHeight() const
{
	return m_pSprite->GetFrameHeight();
}
