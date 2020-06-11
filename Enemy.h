#pragma once
#include "Sentient.h"
#include "Sprite.h"

class Enemy : public Sentient
{
public:
	enum class Type
	{
		Goomba, Spiny, Squiglet, Cherbil
	};

	Enemy(Sprite* pSprite, Sprite* pDeathSprite, const Point2f& bottomLeft, float horSpeed = 200.f, float attackDuration = 1.f, float attackCooldown = 5.f);
	
	virtual ~Enemy();
	Enemy(const Enemy& other) = delete;
	Enemy& operator=(const Enemy& other) = delete;
	Enemy(Enemy&& other) = delete;
	Enemy& operator=(Enemy&& other) = delete;

	virtual void Update(float elapsedSec, const Level* level) override;
	virtual void Draw() const override;

	virtual void Die() override;
	virtual bool IsDead() const override;
	bool IsDying() const;

	//Returns wether the enemy should be killed when jumped on
	virtual bool JumpedOnByPlayer() const = 0;
	void FlipHorizontalVelocity();
	bool CanBeFlipped() const;
	
private:
	const float m_HorizontalSpeed;
	static const float m_MaxFlipCooldown;
	float m_RemainingFlipCooldown;

	bool canMove(bool toLeft, const Level* plevel) const;
protected:
	Sprite* m_pSprite;
	Sprite* m_pDeathSprite;
};

