#include "pch.h"
#include "Camera.h"
#include "utils.h"

Camera::Camera(float width, float height)
	: m_Width{width}
	, m_Height{height}
	, m_LevelBoundaries{0.f,0.f, width, height}
{
}

void Camera::SetLevelBoundaries(const Rectf& levelBoundaries)
{
	m_LevelBoundaries = levelBoundaries;
}

void Camera::Transform(const Rectf& target) const
{
	Point2f cameraCenter{ Track(target) };
	Clamp(cameraCenter);
	
	Point2f cameraBottomLeft{ cameraCenter.x - m_Width / 2, cameraCenter.y - m_Height / 2 };

	glTranslatef(-cameraBottomLeft.x, -cameraBottomLeft.y, 0);
}

Point2f Camera::Track(const Rectf& target) const
{
	return Point2f{ target.left + target.width / 2, target.bottom + target.height / 2 };
}

void Camera::Clamp(Point2f& bottomLeft) const
{
	if (bottomLeft.x - m_Width / 2 < m_LevelBoundaries.left) bottomLeft.x = m_LevelBoundaries.left + m_Width / 2;
	if (bottomLeft.x + m_Width / 2 > m_LevelBoundaries.left + m_LevelBoundaries.width) bottomLeft.x = m_LevelBoundaries.left + m_LevelBoundaries.width - m_Width / 2;

	if (bottomLeft.y - m_Height / 2 < m_LevelBoundaries.bottom) bottomLeft.y = m_LevelBoundaries.bottom + m_Height / 2;
	if (bottomLeft.y + m_Height / 2 > m_LevelBoundaries.bottom + m_LevelBoundaries.height) bottomLeft.y = m_LevelBoundaries.bottom + m_LevelBoundaries.height - m_Height / 2;
}
