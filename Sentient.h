#pragma once
#include "GameObject.h"
#include "ProjectileManager.h"

class Sentient : public GameObject
{
public:
	Sentient(const Point2f& bottomLeft, bool IsHorizontalSpeedConstant = true, float attackDuration = 1.f, float attackCooldown = 5.f);
	virtual ~Sentient() = default;
	virtual void Attack();
	virtual void AttackEnded();
	virtual void Update(float elapsedSec, const Level* level) override;

	void Freeze();
	bool IsFrozen() const;

	virtual void Die() = 0;
	virtual bool IsDead() const = 0;

	virtual Rectf GetHitbox() const override;
protected:
	float m_AttackDuration;
	float m_RemainingAttackSec;
	float m_AttackCooldown;
	float m_RemainingCooldownSec;

	bool m_FacingLeft;

	float m_RemainingFrozenSec;
	float m_FreezeDuration;
};

