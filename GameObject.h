#pragma once
#include "Vector2f.h"
#include "Level.h"
enum class GameState
{
	Idle, MovingLeft, MovingRight, Falling, Attacked, Attacking
};

class GameObject
{
public:
	GameObject(const Point2f& bottomLeft, bool IsHorizontalSpeedConstant = true, bool IsVerticalSpeedConstant = false);
	virtual ~GameObject() = default;
	virtual void Update(float elapsedSec, const Level* level);
	virtual void Draw() const = 0;

	virtual bool IsOverlapping(const GameObject* other) const;
	virtual bool IsOverlapping(const Rectf& other) const;
	Vector2f GetVelocity() const;
	void SetVelocity(const Vector2f& velocity);
	void SetHorizontalVelocity(float Vx);
	void SetVerticalVelocity(float Vy);
	void AddVelocity(const Vector2f& force);
	void SetPosition(const Point2f& pos);

	virtual Rectf GetHitbox() const;
protected:
	static const float m_FallAcceleration;
	static const float m_MaxFallSpeed;

	Vector2f m_Velocity;
	bool m_IsHorizontalSpeedConstant;
	bool m_IsVerticalSpeedConstant;

	Rectf m_Hitbox;
	GameState m_GameState;
};

