#pragma once
#include <map>
#include "Sprite.h"

class SpriteManager
{
public:
	enum class SpriteType
	{
		Coconut,
		Bomb,
		BombExplosion,
		CherbilProjectile,
		Numbers,
		Operators,
		Cudge,
		Barry,
		Boomer,
		MarioIdle,
		MarioRun,
		MarioJump,
		MarioDamaged,
		MarioPickup,
		InvincibleBubble,
		MarioAttackCudge,
		MarioAttackBarry,
		MarioAttackBoomer,
		Chest,
		NotFound
	};

	SpriteManager();
	~SpriteManager();

	SpriteManager(const SpriteManager& other) = delete;
	SpriteManager& operator=(const SpriteManager& other) = delete;
	SpriteManager(SpriteManager&& other) = delete;
	SpriteManager& operator=(SpriteManager&& other) = delete;

	Sprite* GetSprite(SpriteType type);
private:
	std::map<SpriteType, Sprite*> m_SpriteMap;
};

