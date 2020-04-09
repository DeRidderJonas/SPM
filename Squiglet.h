#pragma once
#include "Enemy.h"
class Squiglet : public Enemy
{
public:
	Squiglet(const Point2f& bottomLeft);
	virtual ~Squiglet() = default;

	virtual void Attack() override;

	virtual bool JumpedOnByPlayer() const override;
};

