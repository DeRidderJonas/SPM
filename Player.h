#pragma once
#include "structs.h"
#include "Sprite.h"
#include "Sentient.h"
#include "PixlManager.h"

class Player: public Sentient
{
public:
	Player(const Point2f& bottomLeft);
	virtual ~Player();
	Player(const Player& other) = delete;
	Player& operator=(const Player& other) = delete;
	Player(Player&& other) = delete;
	Player& operator=(Player&& other) = delete;

	virtual void Update(float elapsedSec, const Level* level) override;
	virtual void Draw() const override;

	void WasHit(bool fromLeft);
	bool IsInvincible() const;

	int GetMaxHealth() const;
	int GetCurrentHealth() const;
	void RestoreHealth(int amount);

	void SetIsPickingUp(bool value);
	void SetActivePixlType(Pixl::Type type);

	virtual Rectf GetHitbox() const override;
	Rectf GetAttackHitbox() const;
private:
	static const float m_HorizontalSpeed;
	static const float m_JumpSpeed;
	static const float m_MaxInvincibleSec;

	bool m_IsPickingUp;

	Sprite* m_pActiveSprite;
	Sprite* m_pInvincibleBubble;

	int m_MaxHealth, m_CurrentHealth;
	float m_RemainingInvincibleSec;

	Pixl m_ActivePixl;
	Rectf m_AttackingHitbox;

	void OnInput(float elapsedSec);
	virtual void Attack() override;

	void SetActiveSprite(SpriteManager::SpriteType spriteType);

	//DEBUG ONLY
	void PrintGameState();
};

