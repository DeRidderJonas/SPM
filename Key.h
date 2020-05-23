#pragma once
#include "Item.h"
class Key : public Item
{
public:
	Key(Texture* pTexture, Texture* pName, Texture* pDescription, const Point2f& bottomLeft);
	virtual ~Key() = default;

	virtual void Use() const override;
};

