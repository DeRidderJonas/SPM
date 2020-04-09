#include "pch.h"
#include "Spiny.h"

Spiny::Spiny(const Point2f& bottomLeft)
	: Enemy{ new Sprite("Resources/Enemies/Spiny.png", 4, 1, .5f), bottomLeft, 75.f }
{
}

bool Spiny::JumpedOnByPlayer() const
{
	return false;
}
