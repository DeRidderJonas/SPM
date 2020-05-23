#pragma once
#include "Item.h"

class PowerBlock : public Item
{
public:
	PowerBlock(Texture* pTexture, Texture* pName, Texture* pDescription, const Point2f& bottomLeft);
	virtual void Use() const override;
};

