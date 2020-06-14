#include "pch.h"
#include "Merchant.h"
#include "ItemFactory.h"
#include "utils.h"
#include "Managers.h"

const int Merchant::m_MaxAmountOfItems{ 8 };
const int Merchant::m_MinAmountOfItems{ 3 };

Merchant::Merchant(const Point2f& bottomLeft, Sprite* pSprite, Player* pPlayer)
	: m_pSprite{pSprite}
	, m_Shape{bottomLeft.x, bottomLeft.y, pSprite->GetFrameWidth(), pSprite->GetFrameHeight()}
	, m_IsPlayerInShop{false}
	, m_ActiveItem{0}
{
	int amountOfItems{ (rand() % (m_MaxAmountOfItems - m_MinAmountOfItems)) + m_MinAmountOfItems };
	for (size_t i = 0; i < amountOfItems; i++)
	{
		m_ShopItems.push_back(ItemFactory::CreateRandom(Point2f{}, pPlayer, false));
	}
}

Merchant::~Merchant()
{
	for (Item* item : m_ShopItems)
	{
		delete item;
	}
}

void Merchant::Update(float elapsedSec)
{
	m_pSprite->Update(elapsedSec);
}

void Merchant::Draw() const
{
	m_pSprite->Draw(m_Shape);
}

void Merchant::DrawShop(const Point2f& topLeft, const Rectf& descriptionRect) const
{
	if (!m_IsPlayerInShop) return;

	//TODO: draw shop
	Texture* pShop{ Managers::GetInstance()->GetTextureManager()->GetTexture(TextureManager::TextureType::ShopBar) };
	Point2f bottomLeft{ topLeft.x, topLeft.y - pShop->GetHeight() };

	const float menuWidth{ 350.f };
	const float menuMargin{ 20.f };
	const float itemMargin{ 5.f };
	const float itemHeight{ 40.f };
	const float descPadding{ 10.f };

	Rectf shopRect{ bottomLeft.x - menuMargin, bottomLeft.y + menuMargin, menuWidth, (m_ShopItems.size()+2) * itemHeight };
	shopRect.bottom -= shopRect.height;
	glColor3f(1.f, 1.f, 1.f);
	utils::FillRect(shopRect);
	glColor3f(0.f, 0.f, 0.f);
	utils::DrawRect(shopRect, 2.f);

	pShop->Draw(bottomLeft);

	Rectf itemRect{ topLeft.x + itemMargin, bottomLeft.y - itemHeight - menuMargin, menuWidth - menuMargin, itemHeight };
	Rectf descRect{ descriptionRect.left + descPadding, descriptionRect.bottom + descPadding, descriptionRect.width - 2 * descPadding, descriptionRect.height - 2 * descPadding };

	Texture* activePointer{ Managers::GetInstance()->GetTextureManager()->GetTexture(TextureManager::TextureType::Pointer) };
	for (size_t i = 0; i < m_ShopItems.size(); i++)
	{
		if (i == m_ActiveItem) activePointer->Draw(Point2f{ itemRect.left - activePointer->GetWidth(), itemRect.bottom + activePointer->GetHeight() / 2 });
		m_ShopItems[i]->DrawInInventory(itemRect, i == m_ActiveItem, descRect, true);
		itemRect.bottom -= itemMargin + itemHeight;
	}
}

bool Merchant::EnterShop(const Rectf& playerShape)
{
	m_IsPlayerInShop = utils::IsOverlapping(playerShape, m_Shape);
	return m_IsPlayerInShop;
}

void Merchant::ExitShop()
{
	m_IsPlayerInShop = false;
}

bool Merchant::IsPlayerInShop() const
{
	return m_IsPlayerInShop;
}

void Merchant::Scroll(bool up)
{
	m_ActiveItem += up ? -1 : 1;
	if (m_ActiveItem < 0) m_ActiveItem = int(m_ShopItems.size()) - 1;
	if (m_ActiveItem >= m_ShopItems.size()) m_ActiveItem = 0;
}

void Merchant::Buy(Player* pPlayer)
{
	Item* toBuy{ m_ShopItems[m_ActiveItem] };
	if (toBuy->GetCost() > pPlayer->GetAmountOfCoins()) return;

	std::vector<Item*>::iterator it{ std::find(m_ShopItems.begin(), m_ShopItems.end(), toBuy) };
	if (it == m_ShopItems.end()) return;

	m_ShopItems.erase(it);
	Managers::GetInstance()->GetItemManager()->AddItemToInventory(toBuy);
	pPlayer->AdjustAmountOfCoins(-toBuy->GetCost());
	Managers::GetInstance()->GetSoundManager()->PlaySoundEffect(SoundManager::Soundfx::Buy);
}
