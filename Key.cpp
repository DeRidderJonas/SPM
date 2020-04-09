#include "pch.h"
#include "Key.h"

Key::Key(Texture* pTexture, Texture* pName, Texture* pDescription, Point2f bottomLeft)
	: Item{pTexture, pName, pDescription, bottomLeft}
{
}

void Key::Use()
{
	//No use other than opening the door
}
