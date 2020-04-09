#include "pch.h"
#include "Goomba.h"

Goomba::Goomba(const Point2f& bottomLeft)
	: Enemy{ new Sprite("Resources/Enemies/Goomba.png", 4, 2, 0.15f), bottomLeft }
{
}

bool Goomba::JumpedOnByPlayer() const
{
	return true;
}
