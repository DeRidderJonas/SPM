#pragma once
#include "Texture.h"
#include <map>
class TextManager
{
public:
	enum class Text
	{
		Key,
		KeyDesc,
		RedShroomshake,
		RedShroomshakeDesc,
		Cudge,
		CudgeDesc,
		Barry,
		BarryDesc,
		Boomer,
		BoomerDesc,
		NotFound
	};

	TextManager();
	~TextManager();
	TextManager(const TextManager& other) = delete;
	TextManager& operator=(const TextManager& other) = delete;
	TextManager(TextManager&& other) = delete;
	TextManager& operator=(TextManager&& other) = delete;

	Texture* GetTexture(Text text);
private:
	std::map<Text, Texture*> m_TextMap;
};

