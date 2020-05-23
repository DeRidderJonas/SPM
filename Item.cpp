#include "pch.h"
#include "Item.h"

Item::Item(Type type, Texture* pTexture, Texture* pName, Texture* pDescription, const Point2f& bottomLeft, Player* pPlayer)
	: m_type{type}
	, m_pTexture{pTexture}
	, m_pNameTexture{pName}
	, m_pDescriptionTexture{pDescription}
	, m_Hitbox{bottomLeft.x, bottomLeft.y, pTexture->GetWidth(), pTexture->GetHeight()}
	, m_pPlayer{pPlayer}
{
}

void Item::Draw() const
{
	m_pTexture->Draw(m_Hitbox);
}

void Item::DrawInInventory(const Rectf& inventoryRect, bool drawDescription, const Rectf& descriptionRect) const
{
	Rectf itemRect{ inventoryRect };
	float itemWidth = itemRect.width / 7;
	itemRect.width = itemWidth;
	m_pTexture->Draw(itemRect);
	Point2f nameBottomLeft{ inventoryRect.left + itemWidth + 5.f, inventoryRect.bottom + m_pNameTexture->GetHeight() / 4};
	m_pNameTexture->Draw(nameBottomLeft);

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
