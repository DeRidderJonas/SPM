#include "pch.h"
#include "SpriteManager.h"
#include <iostream>

SpriteManager::SpriteManager()
{
	m_SpriteMap[SpriteType::Coconut] = new Sprite("Resources/Projectiles/Coconut.png", 1, 1, 1.f);
	m_SpriteMap[SpriteType::Numbers] = new Sprite("Resources/HUD/Numbers.png", 10, 1, 1.f);
	m_SpriteMap[SpriteType::Operators] = new Sprite("Resources/HUD/Operators.png", 2, 1, 1.f);
	m_SpriteMap[SpriteType::Cudge] = new Sprite("Resources/Pixls/Cudge.png", 1, 1, 1.f);
	m_SpriteMap[SpriteType::Barry] = new Sprite("Resources/Pixls/Barry.png", 1, 1, 1.f);
	m_SpriteMap[SpriteType::MarioIdle] = new Sprite("Resources/Mario/Mario_Idle.png", 1, 1, 0.075f);
	m_SpriteMap[SpriteType::MarioRun] = new Sprite("Resources/Mario/Mario.png", 4, 2, 0.075f);
	m_SpriteMap[SpriteType::MarioJump] = new Sprite("Resources/Mario/Mario_Jump.png", 1, 1, 1.f);
	m_SpriteMap[SpriteType::MarioDamaged] = new Sprite("Resources/Mario/Damaged.png", 1, 1, 1.f);
	m_SpriteMap[SpriteType::MarioPickup] = new Sprite("Resources/Mario/Mario_Pickup.png", 1, 1, 1.f);
	m_SpriteMap[SpriteType::InvincibleBubble] = new Sprite("Resources/Mario/Bubble.png", 2, 1, .05f);
	m_SpriteMap[SpriteType::MarioAttackCudge] = new Sprite("Resources/Mario/Cudge_Attack.png", 4, 1, .075f);
	m_SpriteMap[SpriteType::MarioAttackBarry] = new Sprite("Resources/Mario/Barry_Attack.png", 6, 2, .075f);
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
