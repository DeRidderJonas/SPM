#pragma once
#include "Item.h"
#include "TextureManager.h"
#include "TextManager.h"
class ItemFactory
{
public:
	static Item* CreateItem(Item::Type itemType, const Point2f& bottomLeft, Player* pPlayer = nullptr);
	static Item* CreateRandom(const Point2f& bottomLeft, Player* pPlayer, bool keyAllowed);
private:
	static Texture* GetTexture(Item::Type itemType);
};

