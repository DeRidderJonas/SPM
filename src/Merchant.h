#pragma once
#include "structs.h"
#include "Sprite.h"
#include <vector>
#include "Item.h"
class Merchant
{
public:
	Merchant(const Point2f& bottomLeft, Sprite* pSprite, Player* pPlayer);
	~Merchant();

	Merchant(const Merchant& other) = delete;
	Merchant& operator=(const Merchant& other) = delete;
	Merchant(Merchant&& other) = delete;
	Merchant& operator=(Merchant&& other) = delete;

	void Update(float elapsedSec);
	void Draw() const;

	void DrawShop(const Point2f& topLeft, const Rectf& descriptionRect) const;

	bool EnterShop(const Rectf& playerShape);
	void ExitShop();
	bool IsPlayerInShop() const;

	void Scroll(bool up);
	void Buy(Player* pPlayer);

private:
	Rectf m_Shape;
	Sprite* m_pSprite;

	bool m_IsPlayerInShop;
	std::vector<Item*> m_ShopItems;

	int m_ActiveItem;

	static const int m_MaxAmountOfItems;
	static const int m_MinAmountOfItems;
};

