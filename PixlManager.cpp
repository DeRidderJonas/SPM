#include "pch.h"
#include "PixlManager.h"
#include "Managers.h"
#include "utils.h"

PixlManager::PixlManager()
	: m_SelectedPixl{Pixl::Type::Cudge}
{
	m_UnlockedPixls[Pixl::Type::Cudge] = true;
	m_UnlockedPixls[Pixl::Type::Barry] = true;
}

void PixlManager::Unlock(const Pixl::Type& pixlType)
{
	m_UnlockedPixls[pixlType] = true;
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

	Rectf pixlsBackground{ 0.f, MenuRect.bottom + pPixlsTexture->GetHeight() / 3, menuWidth,
		m_UnlockedPixls.size() * pixlHeight + (m_UnlockedPixls.size() + 1) * pixlMargin + 2 * menuMargin };
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
		if (pair.first == m_SelectedPixl) activePointer->Draw(Point2f{ pixlRect.left - activePointer->GetWidth(), pixlRect.bottom + activePointer->GetHeight() / 2 });
		if (pair.second) drawPixl.DrawInMenu(pixlRect, pair.first == m_SelectedPixl, descRect);
		drawPixl.SetType(GetNextPixlType(drawPixl.GetType(), false));
		pixlRect.bottom -= pixlMargin + pixlHeight;
	}
}

void PixlManager::Scroll(bool up)
{
	m_SelectedPixl = GetNextPixlType(m_SelectedPixl, up);
}

void PixlManager::ActivateSelectedPixl(Player* pPlayer)
{
	pPlayer->SetActivePixlType(m_SelectedPixl);
}

Pixl::Type PixlManager::GetNextPixlType(Pixl::Type type, bool up) const
{
	int index{ int(type) };
	if (up) index++;
	else index--;

	if (index < 0) return Pixl::Type::Barry;
	if (index > int(Pixl::Type::Barry)) return Pixl::Type::Cudge;

	return Pixl::Type(index);
}
