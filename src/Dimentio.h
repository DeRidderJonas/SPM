#pragma once
#include "Sentient.h"
#include "Player.h"

class Dimentio : public Sentient
{
public:
	Dimentio(const Point2f& bottomLeft, Player* pPlayer, const Rectf& containmentField );
	virtual ~Dimentio() = default;

	virtual void Attack() override;
	virtual void Update(float elapsedSec, const Level* pLevel) override;
	virtual void Draw() const override;

	void Attacked();

	virtual void Die() override;
	virtual bool IsDead() const override;
private:
	Sprite* m_pSprite;
	Sprite* m_pAttacking;
	Sprite* m_pAttacked;
	Sprite* m_pDeath;

	Player* m_pPlayer;

	int m_Health;

	const float m_ProjectileSpeed;
	const Vector2f m_Movementspeed;

	Rectf m_ContainmentField;
	Point2f m_NextLocation;
	const float m_NewLocationErrorMargin{ 5.f };

	void RandomiseNewLocation();
};

