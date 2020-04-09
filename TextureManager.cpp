#include "pch.h"
#include "TextureManager.h"

TextureManager::TextureManager()
{
	m_TextureMap[TextureType::Background1] = new Texture("Resources/Backgrounds/bg01.png");
	m_TextureMap[TextureType::Background2] = new Texture("Resources/Backgrounds/bg02.png");
	m_TextureMap[TextureType::Background3] = new Texture("Resources/Backgrounds/bg03.png");
	m_TextureMap[TextureType::Background4] = new Texture("Resources/Backgrounds/bg04.png");
	m_TextureMap[TextureType::Brick] = new Texture("Resources/Backgrounds/bricks.png");
	m_TextureMap[TextureType::Healthbar] = new Texture("Resources/HUD/Healthbar.png");
	m_TextureMap[TextureType::Pointer] = new Texture("Resources/HUD/Pointer.png");
	m_TextureMap[TextureType::ItemsBar] = new Texture("Resources/HUD/Items.png");
	m_TextureMap[TextureType::PixlsBar] = new Texture("Resources/HUD/Pixls.png");
	m_TextureMap[TextureType::PickupStar] = new Texture("Resources/HUD/Pickup_Star.png");
	m_TextureMap[TextureType::Key] = new Texture("Resources/Items/Key.png");
	m_TextureMap[TextureType::RedShroomshake] = new Texture("Resources/Items/RedShroomshake.png");
}

TextureManager::~TextureManager()
{
	for (std::pair<const TextureType, Texture*>& pair : m_TextureMap)
	{
		delete pair.second;
	}
}

Texture* TextureManager::GetTexture(TextureType type)
{
	return m_TextureMap[type];
}

Texture* TextureManager::GetRandomBackground()
{
	int amountOfBackgrounds{ 4 };
	int randBackground{ rand() % amountOfBackgrounds };
	return GetTexture(TextureType(randBackground));
}
