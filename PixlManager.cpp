#include "pch.h"
#include <algorithm>
#include "PixlManager.h"
#include "Managers.h"
#include "utils.h"
#include <iostream>
#include <sstream>

PixlManager::PixlManager()
	: m_SelectedPixl{Pixl::Type::Cudge}
{
	m_UnlockedPixls[Pixl::Type::Cudge] = true;
	m_UnlockedPixls[Pixl::Type::Barry] = false;
	m_UnlockedPixls[Pixl::Type::Boomer] = false;
}

void PixlManager::Unlock(const Pixl::Type& pixlType)
{
	m_UnlockedPixls[pixlType] = true;
	m_SelectedPixl = pixlType;
}

bool PixlManager::IsUnlocked(const Pixl::Type& pixlType) const
{
	return m_UnlockedPixls.at(pixlType);
}

void PixlManager::SetPixl(const Pixl::Type& pixlType, Pixl* pixl)
{
	switch (pixlType)
	{
	case Pixl::Type::Cudge:
		pixl->SetType(Pixl::Type::Cudge);
		break;
	case Pixl::Type::Barry:
		pixl->SetType(Pixl::Type::Barry);
		break;
	}
}

void PixlManager::DrawPixlsMenu(const Point2f& topLeft, bool isActive, const Rectf& descriptionRect) const
{
	const float menuWidth{ 250.f };
	const float menuMargin{ 20.f };
	const float pixlMargin{ 5.f };
	const float pixlHeight{ 40.f };
	const float descPadding{ 10.f };

	Texture* pPixlsTexture{ Managers::GetInstance()->GetTextureManager()->GetTexture(TextureManager::TextureType::PixlsBar) };
	Rectf MenuRect{ topLeft.x, topLeft.y - pPixlsTexture->GetHeight(), pPixlsTexture->GetWidth(), pPixlsTexture->GetHeight() };

	int amountOfPixlsUnlocked{ int(std::count_if(m_UnlockedPixls.begin(), m_UnlockedPixls.end(), [](const std::pair<const Pixl::Type, bool>& pixl) {return pixl.second; })) };
	Rectf pixlsBackground{ 0.f, MenuRect.bottom + pPixlsTexture->GetHeight() / 3, menuWidth,
		amountOfPixlsUnlocked* pixlHeight + (amountOfPixlsUnlocked + 1) * pixlMargin + 2 * menuMargin };
	pixlsBackground.left = MenuRect.left + MenuRect.width / 2 - pixlsBackground.width / 2;
	pixlsBackground.bottom -= pixlsBackground.height;

	if (isActive)
	{
		glColor3f(1.f, 1.f, 1.f);
		utils::FillRect(pixlsBackground);
		utils::FillRect(descriptionRect);
		glColor3f(0.f, 0.f, 0.f);
		utils::DrawRect(pixlsBackground, 2.f);
		utils::DrawRect(descriptionRect, 2.f);
	}

	pPixlsTexture->Draw(MenuRect);
	if (!isActive) return;

	Rectf pixlRect{ topLeft.x + menuMargin, MenuRect.bottom - menuMargin * 2 - pixlMargin, menuWidth - 2 * menuMargin, pixlHeight };
	Rectf descRect{ descriptionRect.left + descPadding, descriptionRect.bottom + descPadding, descriptionRect.width - 2 * descPadding, descriptionRect.height - 2 * descPadding };

	Texture* activePointer{ Managers::GetInstance()->GetTextureManager()->GetTexture(TextureManager::TextureType::Pointer) };
	Pixl drawPixl{ Pixl::Type::Cudge };
	for (const std::pair<const Pixl::Type, bool>& pair : m_UnlockedPixls)
	{
		drawPixl.SetType(pair.first);
		if (pair.first == m_SelectedPixl) activePointer->Draw(Point2f{ pixlRect.left - activePointer->GetWidth(), pixlRect.bottom + activePointer->GetHeight() / 2 });
		if (pair.second) drawPixl.DrawInMenu(pixlRect, pair.first == m_SelectedPixl, descRect);
		pixlRect.bottom -= pixlMargin + pixlHeight;
	}
}

void PixlManager::DrawUnlockingPixl(const Rectf& drawRect) const
{
	Texture* pStar{ Managers::GetInstance()->GetTextureManager()->GetTexture(TextureManager::TextureType::PickupStar) };
	Sprite* pPixl{};
	switch (m_SelectedPixl)
	{
	case Pixl::Type::Cudge:
		pPixl = Managers::GetInstance()->GetSpriteManager()->GetSprite(SpriteManager::SpriteType::Cudge);
		break;
	case Pixl::Type::Barry:
		pPixl = Managers::GetInstance()->GetSpriteManager()->GetSprite(SpriteManager::SpriteType::Barry);
		break;
	}

	pStar->Draw(drawRect);
	pPixl->Draw(Point2f{ drawRect.left + drawRect.width / 2 - pPixl->GetFrameWidth() / 2, drawRect.bottom + drawRect.height / 2 - pPixl->GetFrameHeight() / 2 });

	Texture* pPixlName{};
	switch (m_SelectedPixl)
	{
	case Pixl::Type::Cudge:
		pPixlName = Managers::GetInstance()->GetTextManager()->GetTexture(TextManager::Text::Cudge);
		break;
	case Pixl::Type::Barry:
		pPixlName = Managers::GetInstance()->GetTextManager()->GetTexture(TextManager::Text::Barry);
		break;
	}
	Rectf nameRect{drawRect.left + drawRect.width/2 - pPixlName->GetWidth()/2, 
		drawRect.bottom + drawRect.height /2 - pPixl->GetFrameHeight()/2 - pPixlName->GetHeight() - 5.f, 
		pPixlName->GetWidth(), pPixlName->GetHeight()};
	float padding{ 5.f };
	Rectf nameRectBg{ nameRect.left - padding, nameRect.bottom - padding, nameRect.width + 2 * padding, nameRect.height + 2 * padding };
	glColor3f(1.f, 1.f, 1.f);
	utils::FillRect(nameRectBg);
	glColor3f(0.f, 0.f, 0.f);
	utils::DrawRect(nameRectBg);
	pPixlName->Draw(nameRect);
}

void PixlManager::Scroll(bool up)
{
	Pixl::Type nextPixlType{ GetNextPixlType(m_SelectedPixl, up) };
	while (!m_UnlockedPixls.at(nextPixlType))
	{
		nextPixlType = GetNextPixlType(nextPixlType, up);
	}
	m_SelectedPixl = nextPixlType;
}

void PixlManager::ActivateSelectedPixl(Player* pPlayer)
{
	pPlayer->SetActivePixlType(m_SelectedPixl);
}

Pixl::Type PixlManager::GetNextUnlockablePixl() const
{
	std::map<Pixl::Type, bool>::const_reverse_iterator currLastUnlocked{ std::find_if(m_UnlockedPixls.rbegin(), m_UnlockedPixls.rend(), 
		[](const std::pair<const Pixl::Type, bool>& pair) {return pair.second; }) };
	return GetNextPixlType(currLastUnlocked->first, true);
}

std::string PixlManager::ToSaveFormat() const
{
	std::stringstream ss{};
	for (const std::pair<Pixl::Type, bool>& pair : m_UnlockedPixls)
	{
		ss << pair.second << ',';
	}
	return ss.str();
}

void PixlManager::LoadFromSave(std::string saveLine)
{
	std::stringstream ss{ saveLine };
	for (std::map<Pixl::Type, bool>::iterator it{ m_UnlockedPixls.begin() }; it != m_UnlockedPixls.end(); ++it)
	{
		std::string unlocked{};
		std::getline(ss, unlocked, ',');
		it->second = unlocked == "1";
	}
}

Pixl::Type PixlManager::GetNextPixlType(Pixl::Type type, bool up) const
{
	int index{ int(type) };
	if (up) index++;
	else index--;

	if (index < 0) return Pixl::Type::Boomer;
	if (index > int(Pixl::Type::Boomer)) return Pixl::Type::Cudge;

	return Pixl::Type(index);
}
