#pragma once
#include "structs.h"
class Camera
{
public:
	Camera(float width, float height);
	void SetLevelBoundaries(const Rectf& levelBoundaries);
	void Transform(const Rectf& target) const;

	Point2f GetTranslation(const Rectf& target) const;
private:
	float m_Width;
	float m_Height;
	Rectf m_LevelBoundaries;

	Point2f Track(const Rectf& target) const;
	void Clamp(Point2f& bottomLeft) const;
};

