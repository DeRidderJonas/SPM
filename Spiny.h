#pragma once
#include "Enemy.h"
class Spiny : public Enemy
{
public:
	Spiny(const Point2f& bottomLeft);
	virtual ~Spiny() = default;

	virtual bool JumpedOnByPlayer() const override;
};

