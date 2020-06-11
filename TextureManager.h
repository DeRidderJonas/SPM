#pragma once
#include "Texture.h"
#include <map>
class TextureManager
{
public:
	//Backgrounds should be the first textures (for GetRandomBackground)
	enum class TextureType
	{
		Background1,
		Background2,
		Background3,
		Background4,
		Door,
		DoorShadow,
		RestArea,
		Brick,
		Pipe,
		TitleScreen,
		SaveFile,
		Healthbar,
		Coinbar,
		Pointer,
		ItemsBar,
		ShopBar,
		PixlsBar,
		Settingsbar,
		SettingFull,
		SettingEmpty,
		PickupStar,
		Key,
		RedShroomshake,
		PowerBlock,
		IceStorm,
		Frozen,
		NotFound
	};

	TextureManager();
	~TextureManager();

	TextureManager(const TextureManager& other) = delete;
	TextureManager& operator=(const TextureManager& other) = delete;
	TextureManager(TextureManager&& other) = delete;
	TextureManager& operator=(TextureManager&& other) = delete;

	Texture* GetTexture(TextureType type);
	Texture* GetRandomBackground();
private:
	std::map<TextureType, Texture*> m_TextureMap;
};

