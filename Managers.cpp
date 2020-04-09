#include "pch.h"
#include "Managers.h"

Managers* Managers::m_Instance{ nullptr };

Managers* Managers::GetInstance()
{
	if (m_Instance == nullptr) m_Instance = new Managers();

	return m_Instance;
}

Managers::~Managers()
{
	delete m_TextureManager;
	delete m_SpriteManager;
	delete m_TextManager;
	delete m_EnemyManager;
	delete m_ProjectileManager;
	delete m_ItemManager;
	delete m_PixlManager;
}

TextureManager* Managers::GetTextureManager()
{
	return m_TextureManager;
}

SpriteManager* Managers::GetSpriteManager()
{
	return m_SpriteManager;
}

TextManager* Managers::GetTextManager()
{
	return m_TextManager;
}

EnemyManager* Managers::GetEnemyManager()
{
	return m_EnemyManager;
}

ProjectileManager* Managers::GetProjectileManager()
{
	return m_ProjectileManager;
}

ItemManager* Managers::GetItemManager()
{
	return m_ItemManager;
}

PixlManager* Managers::GetPixlManager()
{
	return m_PixlManager;
}

Managers::Managers()
	: m_TextureManager{new TextureManager()}
	, m_SpriteManager{new SpriteManager()}
	, m_TextManager{new TextManager()}
	, m_EnemyManager{new EnemyManager()}
	, m_ProjectileManager{new ProjectileManager()}
	, m_ItemManager{new ItemManager()}
	, m_PixlManager{new PixlManager()}
{
}