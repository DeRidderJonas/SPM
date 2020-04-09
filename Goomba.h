#pragma once
#include "Enemy.h"
class Goomba : public Enemy
{
public:
	Goomba(const Point2f& bottomLeft);
	virtual ~Goomba() = default;

	virtual bool JumpedOnByPlayer() const override;
};

