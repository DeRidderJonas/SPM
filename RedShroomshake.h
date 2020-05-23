#pragma once
#include "Item.h"
class RedShroomshake : public Item
{
public:
	RedShroomshake(Texture* pTexture, Texture* pName, Texture* pDescription, const Point2f& bottomLeft, Player* pPlayer);
	virtual void Use() override;
private:
	static const int m_HealthRestore;
};

