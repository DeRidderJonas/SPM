#include "pch.h"
#include "Item.h"
#include "Managers.h"

Item::Item(Type type, int cost, Texture* pTexture, Texture* pName, Texture* pDescription, const Point2f& bottomLeft, Player* pPlayer)
	: m_type{type}
	, m_pTexture{pTexture}
	, m_pNameTexture{pName}
	, m_pDescriptionTexture{pDescription}
	, m_Hitbox{bottomLeft.x, bottomLeft.y, pTexture->GetWidth(), pTexture->GetHeight()}
	, m_pPlayer{pPlayer}
	, m_Cost{cost}
{
}

void Item::Draw() const
{
	m_pTexture->Draw(m_Hitbox);
}

void Item::DrawInInventory(const Rectf& inventoryRect, bool drawDescription, const Rectf& descriptionRect, bool showCost) const
{
	Rectf itemRect{ inventoryRect };
	float itemWidth = itemRect.width / 7;
	itemRect.width = itemWidth;
	m_pTexture->Draw(itemRect);
	Point2f nameBottomLeft{ inventoryRect.left + itemWidth + 5.f, inventoryRect.bottom + m_pNameTexture->GetHeight() / 4};
	m_pNameTexture->Draw(nameBottomLeft);


	if (showCost)
	{
		Sprite* pNumbers{ Managers::GetInstance()->GetSpriteManager()->GetSprite(SpriteManager::SpriteType::Numbers) };
		int hundreds{ m_Cost / 100 }, tens{ (m_Cost / 10) % 10 }, digits{ m_Cost % 10 };
		Point2f numberBottomLeft{ inventoryRect.left + 2 * inventoryRect.width / 3, nameBottomLeft.y };
		pNumbers->SetFrame(hundreds);
		pNumbers->Draw(numberBottomLeft);
		numberBottomLeft.x += pNumbers->GetFrameWidth();

		pNumbers->SetFrame(tens);
		pNumbers->Draw(numberBottomLeft);
		numberBottomLeft.x += pNumbers->GetFrameWidth();

		pNumbers->SetFrame(digits);
		pNumbers->Draw(numberBottomLeft);
	}

	if (!drawDescription) return;

	m_pDescriptionTexture->Draw(descriptionRect);
}

void Item::DrawAtPos(const Point2f& center) const
{
	m_pTexture->Draw(Point2f{ center.x - m_pTexture->GetWidth() / 2, center.y - m_pTexture->GetHeight() / 2 });
}

Rectf Item::GetHitbox() const
{
	return m_Hitbox;
}

Item::Type Item::GetType() const
{
	return m_type;
}

int Item::GetCost() const
{
	return m_Cost;
}
