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
		CounteredCherbilProjectile,
		BurningHeart,
		StoneHeart,
		VulnerableHeart,
		BrokenHeart,
		Numbers,
		Operators,
		Cudge,
		Barry,
		Boomer,
		Flamm,
		MarioIdle,
		MarioRun,
		MarioJump,
		MarioDamaged,
		MarioPickup,
		MarioDeath,
		InvincibleBubble,
		MarioAttackCudge,
		MarioAttackBarry,
		MarioAttackBoomer,
		Dimentio,
		DimentioAttack,
		DimentioAttacked,
		DimentioDeath,
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

