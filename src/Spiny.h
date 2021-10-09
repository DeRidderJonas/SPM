#pragma once
#include "Enemy.h"
class Spiny : public Enemy
{
public:
	Spiny(const Point2f& bottomLeft);
	virtual ~Spiny();

	virtual bool JumpedOnByPlayer() const override;
};

