#include "pch.h"
#include "ItemFactory.h"
#include "Managers.h"
#include "Key.h"
#include "RedShroomshake.h"

Item* ItemFactory::CreateItem(Item::Type itemType, const Point2f& bottomLeft, Player* pPlayer)
{
	switch (itemType)
	{
	case Item::Type::Key:
		return new Key(GetTexture(itemType), 
			Managers::GetInstance()->GetTextManager()->GetTexture(TextManager::Text::Key), 
			Managers::GetInstance()->GetTextManager()->GetTexture(TextManager::Text::KeyDesc), bottomLeft
		);
		break;
	case Item::Type::RedShroomshake:
		return new RedShroomshake(GetTexture(itemType), 
			Managers::GetInstance()->GetTextManager()->GetTexture(TextManager::Text::RedShroomshake),
			Managers::GetInstance()->GetTextManager()->GetTexture(TextManager::Text::RedShroomshakeDesc), bottomLeft, pPlayer
		);
		break;
	default:
		return nullptr;
		break;
	}
}

Texture* ItemFactory::GetTexture(Item::Type itemType)
{
	switch (itemType)
	{
	case Item::Type::Key:
		return Managers::GetInstance()->GetTextureManager()->GetTexture(TextureManager::TextureType::Key);
		break;
	case Item::Type::RedShroomshake:
		return Managers::GetInstance()->GetTextureManager()->GetTexture(TextureManager::TextureType::RedShroomshake);
		break;
	default:
		return Managers::GetInstance()->GetTextureManager()->GetTexture(TextureManager::TextureType::NotFound);
		break;
	}
}

