#pragma once
#include "TextureManager.h"
#include "TextManager.h"
#include <vector>
#include "Item.h"
class ItemManager
{
public:
	ItemManager();
	~ItemManager();
	ItemManager(const ItemManager& other) = delete;
	ItemManager& operator=(const ItemManager& other) = delete;
	ItemManager(ItemManager&& other) = delete;
	ItemManager& operator=(ItemManager&& other) = delete;

	bool IsOverlapping(Player* pPlayer);
	void Spawn(Item::Type itemType, const Point2f& bottomLeft, Player* pPlayer = nullptr);

	void DrawAllFloorItems() const;
	void DrawInventoryItems(const Point2f& topLeft, bool isActive, const Rectf& descriptionRect) const;
	void DrawPickUpItem(const Rectf& itemRect) const;
	void DestroyAllFloorItems();

	void SpawnRandom(const Point2f& bottomLeft, Player* pPlayer, bool& KeySpawned);
	bool InventoryHasType(Item::Type itemType);

	void Scroll(bool up);
	void UseActiveItem();
	void RemoveKey();

	std::string ToSaveFormat() const;
	void LoadFromSave(const std::string& saveLine, Player* pPlayer);
private:
	std::vector<Item*> m_FloorItems;
	std::vector<Item*> m_Inventory;

	int m_ActiveItem;

	void Pickup(Item* item);
};

