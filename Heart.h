#pragma once
#include "Sprite.h"

class Heart
{
public:
	enum class AttackType
	{
		Frozen, Bomb, Hammer, Unaffected
	};
	Heart(const Point2f& bottomLeft);
	~Heart() = default;

	void Update(float elapsedSec);
	void Draw() const;

	bool WasHit(const Rectf& hitbox, AttackType attackType);
	bool IsBroken() const;
private:
	enum class State
	{
		Burning, Stone, Vulnerable, Broken
	};

	Rectf m_Hitbox;

	Sprite* m_pBurning;
	Sprite* m_pStone;
	Sprite* m_pVulnerable;
	Sprite* m_pBroken;

	State m_State;
};

