#pragma once
#include <map>
#include "Sprite.h"
#include "SpriteManager.h"
#include "Pixl.h"
#include "Player.h"
class PixlManager
{
public:
	PixlManager();

	void Unlock(const Pixl::Type& pixlType);
	bool IsUnlocked(const Pixl::Type& pixlType) const;
	void SetPixl(const Pixl::Type& pixlType, Pixl* pixl);

	void DrawPixlsMenu(const Point2f& topLeft, bool isActive, const Rectf& descriptionRect) const;
	void Scroll(bool up);
	void ActivateSelectedPixl(Player* pPlayer);
	Pixl::Type GetNextUnlockablePixl() const;
private:
	std::map<Pixl::Type, bool> m_UnlockedPixls;

	Pixl::Type m_SelectedPixl;

	Pixl::Type GetNextPixlType(Pixl::Type type, bool up) const;
};

