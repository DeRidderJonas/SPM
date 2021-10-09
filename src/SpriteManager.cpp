#include "pch.h"
#include "SpriteManager.h"
#include <iostream>

SpriteManager::SpriteManager()
{
	m_SpriteMap[SpriteType::Coconut] = new Sprite("Resources/Projectiles/Coconut.png", 1, 1, 1.f);
	m_SpriteMap[SpriteType::Bomb] = new Sprite("Resources/Projectiles/Bomb.png", 3, 1, 0.5f);
	m_SpriteMap[SpriteType::BombExplosion] = new Sprite("Resources/Projectiles/BombExplosion.png", 3, 1, 0.25f);
	m_SpriteMap[SpriteType::CherbilProjectile] = new Sprite("Resources/Projectiles/CherbilProjectile.png", 3, 2, 0.25f);
	m_SpriteMap[SpriteType::CounteredCherbilProjectile] = new Sprite("Resources/Projectiles/ReflectedCherbilProjectile.png", 3, 2, 0.25f);
	m_SpriteMap[SpriteType::BurningHeart] = new Sprite("Resources/Enemies/BurningHeart.png", 3, 1, 0.25f);
	m_SpriteMap[SpriteType::StoneHeart] = new Sprite("Resources/Enemies/StoneHeart.png", 3, 1, 0.25f);
	m_SpriteMap[SpriteType::BrokenHeart] = new Sprite("Resources/Enemies/BrokenHeart.png", 3, 1, 0.25f);
	m_SpriteMap[SpriteType::VulnerableHeart] = new Sprite("Resources/Enemies/VulnerableHeart.png", 3, 1, 0.25f);
	m_SpriteMap[SpriteType::Numbers] = new Sprite("Resources/HUD/Numbers.png", 10, 1, 1.f);
	m_SpriteMap[SpriteType::Operators] = new Sprite("Resources/HUD/Operators.png", 2, 1, 1.f);
	m_SpriteMap[SpriteType::Cudge] = new Sprite("Resources/Pixls/Cudge.png", 1, 1, 1.f);
	m_SpriteMap[SpriteType::Barry] = new Sprite("Resources/Pixls/Barry.png", 1, 1, 1.f);
	m_SpriteMap[SpriteType::Boomer] = new Sprite("Resources/Pixls/Boomer.png", 1, 1, 1.f);
	m_SpriteMap[SpriteType::Flamm] = new Sprite("Resources/Characters/Flamm.png", 3, 1, 0.3f);
	m_SpriteMap[SpriteType::MarioIdle] = new Sprite("Resources/Mario/Mario_Idle.png", 1, 1, 0.075f);
	m_SpriteMap[SpriteType::MarioRun] = new Sprite("Resources/Mario/Mario.png", 4, 2, 0.075f);
	m_SpriteMap[SpriteType::MarioJump] = new Sprite("Resources/Mario/Mario_Jump.png", 1, 1, 1.f);
	m_SpriteMap[SpriteType::MarioDamaged] = new Sprite("Resources/Mario/Damaged.png", 1, 1, 1.f);
	m_SpriteMap[SpriteType::MarioPickup] = new Sprite("Resources/Mario/Mario_Pickup.png", 1, 1, 1.f);
	m_SpriteMap[SpriteType::MarioDeath] = new Sprite("Resources/Mario/Mario_Death.png", 7, 1, 0.25f);
	m_SpriteMap[SpriteType::InvincibleBubble] = new Sprite("Resources/Mario/Bubble.png", 2, 1, .05f);
	m_SpriteMap[SpriteType::MarioAttackCudge] = new Sprite("Resources/Mario/Cudge_Attack.png", 4, 1, .075f);
	m_SpriteMap[SpriteType::MarioAttackBarry] = new Sprite("Resources/Mario/Barry_Attack.png", 6, 2, .075f);
	m_SpriteMap[SpriteType::MarioAttackBoomer] = new Sprite("Resources/Mario/Boomer_Attack.png", 3, 1, .16f);
	m_SpriteMap[SpriteType::Dimentio] = new Sprite("Resources/Enemies/Dimentio.png", 3, 1, .25f);
	m_SpriteMap[SpriteType::DimentioAttack] = new Sprite("Resources/Enemies/Dimentio_Attack.png", 5, 1, .25f);
	m_SpriteMap[SpriteType::DimentioAttacked] = new Sprite("Resources/Enemies/Dimentio_Damaged.png", 3, 1, .25f);
	m_SpriteMap[SpriteType::DimentioDeath] = new Sprite("Resources/Enemies/Dimentio_Death.png", 3, 1, .25f);
	m_SpriteMap[SpriteType::Chest] = new Sprite("Resources/Backgrounds/Chest.png", 3, 1, 0.5f);
	
	m_SpriteMap[SpriteType::Chest]->Loop(0);
}

SpriteManager::~SpriteManager()
{
	for (std::pair<const SpriteType, Sprite*>& pair : m_SpriteMap)
	{
		delete pair.second;
	}
}

Sprite* SpriteManager::GetSprite(SpriteType type)
{
	if (m_SpriteMap.count(type) < 1) return nullptr;

	return m_SpriteMap[type];
}
