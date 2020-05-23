#pragma once
#include "EnemyManager.h"
#include "ParticleManager.h"
class Managers
{
public:
	static Managers* GetInstance();
	~Managers();
	Managers(const Managers& other) = delete;
	Managers& operator=(const Managers& other) = delete;
	Managers(Managers&& other) = delete;
	Managers& operator=(Managers&& other) = delete;

	TextureManager* GetTextureManager();
	SpriteManager* GetSpriteManager();
	TextManager* GetTextManager();
	EnemyManager* GetEnemyManager();
	ProjectileManager* GetProjectileManager();
	ItemManager* GetItemManager();
	PixlManager* GetPixlManager();
	ParticleManager* GetParticleManager();
private:
	Managers();
	static Managers* m_Instance;

	TextureManager* m_TextureManager;
	SpriteManager* m_SpriteManager;
	TextManager* m_TextManager;
	EnemyManager* m_EnemyManager;
	ProjectileManager* m_ProjectileManager;
	ItemManager* m_ItemManager;
	PixlManager* m_PixlManager;
	ParticleManager* m_ParticleManager;
};

