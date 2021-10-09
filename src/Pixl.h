#pragma once
#include "Sprite.h"
class Pixl
{
public:
	enum class Type
	{
		Cudge, Barry, Boomer
	};
	Pixl(Type type);

	void Update(float elapsedSec);
	void Draw(const Point2f& bottomLeft) const;
	void DrawInMenu(const Rectf& menuRect, bool drawDesc, const Rectf& descRect) const;
	void DrawInTitleScreen(const Rectf& drawRect);
	void SetType(Type newType);
	Type GetType() const;
	float GetWidth() const;
	float GetHeight() const;
private:
	Type m_Type;
	Sprite* m_pSprite;
	Texture* m_pNameTexture;
	Texture* m_pDescriptionTexture;
};

