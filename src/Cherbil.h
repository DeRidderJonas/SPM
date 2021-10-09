#pragma once
#include "Enemy.h"

class Cherbil : public Enemy
{
public:
	Cherbil(const Point2f& bottomLeft);
	virtual ~Cherbil();

	virtual void Attack() override;

	virtual bool JumpedOnByPlayer() const override;
};

