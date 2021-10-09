#include "pch.h"
#include "Key.h"

Key::Key(Texture* pTexture, Texture* pName, Texture* pDescription, const Point2f& bottomLeft)
	: Item{Item::Type::Key, 999, pTexture, pName, pDescription, bottomLeft}
{
}

void Key::Use() const
{
	//No use other than opening the door
}
