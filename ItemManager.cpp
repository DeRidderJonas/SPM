#include "pch.h"
#include "ItemManager.h"
#include "ItemFactory.h"
#include "utils.h"
#include <iostream>
#include <sstream>
#include "Managers.h"
#include "Key.h"

ItemManager::ItemManager()
	: m_ActiveItem{0}
{
}

ItemManager::~ItemManager()
{
	for (Item* item : m_FloorItems)
	{
		delete item;
	}

	for (Item* item : m_Inventory)
	{
		delete item;
	}
}

bool ItemManager::IsOverlapping(Player* pPlayer)
{
	for (Item* item : m_FloorItems)
	{
		if (utils::IsOverlapping(item->GetHitbox(), pPlayer->GetHitbox()))
		{
			Pickup(item);
			pPlayer->SetIsPickingUp(true);
			m_ActiveItem = int(m_Inventory.size())-1;
			return true;
		}
	}
	return false;
}

void ItemManager::Spawn(Item::Type itemType, const Point2f& bottomLeft, Player* pPlayer)
{
	m_FloorItems.push_back(ItemFactory::CreateItem(itemType, bottomLeft, pPlayer));
}

void ItemManager::DrawAllFloorItems() const
{
	for (Item* item : m_FloorItems)
	{
		item->Draw();
	}
}

void ItemManager::DrawInventoryItems(const Point2f& topLeft, bool isActive, const Rectf& descriptionRect, bool showCursor) const
{
	const float MenuWidth{ 250.f };
	const float MenuMargin{ 20.f };
	const float itemMargin{ 5.f };
	const float itemHeight{ 40.f };
	const float descPadding{ 10.f };

	Texture* pItemsTexture{ Managers::GetInstance()->GetTextureManager()->GetTexture(TextureManager::TextureType::ItemsBar) };
	Rectf MenuRect{ topLeft.x, topLeft.y - pItemsTexture->GetHeight(), pItemsTexture->GetWidth(), pItemsTexture->GetHeight() };

	Rectf inventoryBackground{ 0.f, MenuRect.bottom + pItemsTexture->GetHeight() / 3, MenuWidth,  
		m_Inventory.size() * itemHeight + (m_Inventory.size() + 1) * itemMargin + 2 * MenuMargin };
	inventoryBackground.left = MenuRect.left + MenuRect.width/2 - inventoryBackground.width / 2;
	inventoryBackground.bottom -= inventoryBackground.height;

	if (isActive)
	{
		glColor3f(1.f, 1.f, 1.f);
		utils::FillRect(inventoryBackground);
		if (m_Inventory.size() > 0) utils::FillRect(descriptionRect);
		glColor3f(0.f, 0.f, 0.f);
		utils::DrawRect(inventoryBackground, 2.f);
		if (m_Inventory.size() > 0) utils::DrawRect(descriptionRect, 2.f);
	}

	pItemsTexture->Draw(MenuRect);
	if (!isActive) return;

	Rectf itemRect{ topLeft.x + MenuMargin, MenuRect.bottom - MenuMargin*2, MenuWidth - 2 * MenuMargin, itemHeight };
	Rectf descRect{ descriptionRect.left + descPadding, descriptionRect.bottom + descPadding, descriptionRect.width - 2 * descPadding, descriptionRect.height - 2 * descPadding };

	Texture* activePointer{ Managers::GetInstance()->GetTextureManager()->GetTexture(TextureManager::TextureType::Pointer) };
	for (size_t i = 0; i < m_Inventory.size(); i++)
	{
		if (showCursor && i == m_ActiveItem) activePointer->Draw(Point2f{ itemRect.left - activePointer->GetWidth(), itemRect.bottom + activePointer->GetHeight() / 2 });
		m_Inventory[i]->DrawInInventory(itemRect, i == m_ActiveItem, descRect);
		itemRect.bottom -= itemMargin + itemHeight;
	}
}

void ItemManager::DrawPickUpItem(const Rectf& itemRect) const
{
	Managers::GetInstance()->GetTextureManager()->GetTexture(TextureManager::TextureType::PickupStar)->Draw(itemRect);
	Point2f itemCenter{ itemRect.left + itemRect.width/2, itemRect.bottom + itemRect.height/2 };
	m_Inventory.at(m_ActiveItem)->DrawAtPos(itemCenter);
}

void ItemManager::DestroyAllFloorItems()
{
	for (Item* item : m_FloorItems) delete item;
	m_FloorItems.clear();
}

void ItemManager::AddItemToInventory(Item* item)
{
	m_Inventory.push_back(item);
}

void ItemManager::SpawnRandom(const Point2f& bottomLeft, Player* pPlayer, bool& KeySpawned)
{
	Item* randItem{ ItemFactory::CreateRandom(bottomLeft, pPlayer, !KeySpawned) };
	if (randItem == nullptr) return;

	m_FloorItems.push_back(randItem);

	if (randItem->GetType() == Item::Type::Key) KeySpawned = true;
}

bool ItemManager::InventoryHasType(Item::Type itemType)
{
	return std::find_if(m_Inventory.begin(), m_Inventory.end(), [itemType](const Item* item) {return item->GetType() == itemType; }) != m_Inventory.end();
}

void ItemManager::Scroll(bool up)
{
	if (m_Inventory.size() < 1) return;

	if (up)
	{
		m_ActiveItem--;
		if (m_ActiveItem < 0) m_ActiveItem = int(m_Inventory.size()) - 1;
	}
	else
	{
		m_ActiveItem++;
		m_ActiveItem %= m_Inventory.size();
	}
}

void ItemManager::UseActiveItem()
{
	if (m_Inventory.size() < 1 || m_Inventory[m_ActiveItem]->GetType() == Item::Type::Key) return;
	m_Inventory[m_ActiveItem]->Use();
	delete m_Inventory[m_ActiveItem];
	m_Inventory.erase(m_Inventory.begin() + m_ActiveItem);
	m_ActiveItem = 0;
}

void ItemManager::RemoveKey()
{
	std::vector<Item*>::iterator it{ std::find_if(m_Inventory.begin(), m_Inventory.end(), [](const Item* item) {return item->GetType() == Item::Type::Key; }) };
	if (it == m_Inventory.end()) return;

	int index{ int(std::distance(m_Inventory.begin(), it)) };
	delete m_Inventory[index];
	m_Inventory.erase(m_Inventory.begin() + index);
}

std::string ItemManager::ToSaveFormat() const
{
	std::stringstream ss{};
	for (Item* item : m_Inventory)
	{
		if(item->GetType() != Item::Type::Key) ss << int(item->GetType()) << ',';
	}
	return ss.str();
}

void ItemManager::LoadFromSave(const std::string& saveLine, Player* pPlayer)
{
	std::stringstream ss{ saveLine };
	std::string item{};
	while (std::getline(ss, item, ','))
	{
		m_Inventory.push_back(ItemFactory::CreateItem(Item::Type(std::stoi(item)), Point2f{}, pPlayer));
	}
}

void ItemManager::Pickup(Item* item)
{
	std::vector<Item*>::iterator it{ std::find(m_FloorItems.begin(), m_FloorItems.end(), item) };
	if (it == m_FloorItems.end()) return;
	
	//Item was found
	int index{ int(std::distance(m_FloorItems.begin(), it)) };

	Item* toTransfer{ m_FloorItems[index] };
	m_FloorItems.erase(m_FloorItems.begin() + index);

	m_Inventory.push_back(toTransfer);

	Managers::GetInstance()->GetSoundManager()->PlaySoundEffect(SoundManager::Soundfx::ItemPickup);
}
