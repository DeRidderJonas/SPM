#pragma once
#include "Texture.h"
#include "structs.h"
#include "Player.h"
class Item
{
public:
	enum class Type
	{
		Key,
		RedShroomshake
	};
	Item(Texture* pTexture, Texture* pName, Texture* pDescription, Point2f bottomLeft, Player* pPlayer = nullptr);
	virtual ~Item() = default;

	void Draw() const;
	void DrawInInventory(const Rectf& inventoryRect, bool drawDescription, const Rectf& descriptionRect) const;
	void DrawAtPos(const Point2f& center) const;
	virtual void Use() = 0;

	Rectf GetHitbox() const;
protected:
	Texture* m_pTexture;
	Texture* m_pNameTexture;
	Texture* m_pDescriptionTexture;

	Rectf m_Hitbox;
	Player* m_pPlayer;
};

