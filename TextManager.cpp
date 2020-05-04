#include "pch.h"
#include "TextManager.h"

TextManager::TextManager()
{
	Color4f black{ 0.f,0.f,0.f,1.f };
	int nameFontSize{ 20 };
	int descFontSize{ 30 };
	m_TextMap[Text::Key] = new Texture("Key", "Resources/HUD/DIN-Light.otf", nameFontSize, black);
	m_TextMap[Text::KeyDesc] = new Texture("Opens the door to the next level", "Resources/HUD/DIN-Light.otf", descFontSize, black);
	m_TextMap[Text::RedShroomshake] = new Texture("Red Shroomshake", "Resources/HUD/DIN-Light.otf", nameFontSize, black);
	m_TextMap[Text::RedShroomshakeDesc] = new Texture("Restores 10 health points", "Resources/HUD/DIN-Light.otf", descFontSize, black);
	m_TextMap[Text::Cudge] = new Texture("Cudge", "Resources/HUD/DIN-Light.otf", nameFontSize, black);
	m_TextMap[Text::CudgeDesc] = new Texture("Press 'F' to whack things with a mighty hammer", "Resources/HUD/DIN-Light.otf", descFontSize, black);
	m_TextMap[Text::Barry] = new Texture("Barry", "Resources/HUD/DIN-Light.otf", nameFontSize, black);
	m_TextMap[Text::BarryDesc] = new Texture("Use Barry to create a spiky barrier around you", "Resources/HUD/DIN-Light.otf", descFontSize, black);
	m_TextMap[Text::Boomer] = new Texture("Boomer", "Resources/HUD/DIN-Light.otf", nameFontSize, black);
	m_TextMap[Text::BoomerDesc] = new Texture("Use Boomers power to blow up foes", "Resources/HUD/DIN-Light.otf", descFontSize, black);
}

TextManager::~TextManager()
{
	for (std::pair<const Text, Texture*>& pair : m_TextMap)
	{
		delete pair.second;
	}
}

Texture* TextManager::GetTexture(Text text)
{
	return m_TextMap[text];
}
