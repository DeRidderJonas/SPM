#include "pch.h"
#include "Spiny.h"

Spiny::Spiny(const Point2f& bottomLeft)
	: Enemy{ new Sprite("Resources/Enemies/Spiny.png", 4, 1, .5f), new Sprite("Resources/Enemies/Spiny_Death.png", 3, 1, 0.3f), bottomLeft, 75.f }
{
}

bool Spiny::JumpedOnByPlayer() const
{
	return false;
}
