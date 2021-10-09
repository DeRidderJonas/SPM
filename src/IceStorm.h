#pragma once
#include "Item.h"

class IceStorm : public Item
{
public:
	IceStorm(Texture* pTexture, Texture* pName, Texture* pDescription, const Point2f& bottomLeft);
	virtual void Use() const override;
};

