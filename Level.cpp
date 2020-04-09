#include "pch.h"
#include "Level.h"
#include "utils.h"
#include <iostream>
#include "SVGParser.h"

Level::Level(const Texture* pBackground, const Texture* pBrick)
	: m_pBackgroundTexture{pBackground}
	, m_pBrick{pBrick}
	, m_Boundaries{0.f,0.f, pBackground->GetWidth(), pBackground->GetHeight()}
{
	Rectf boundaries{ 0.f,0.f, pBackground->GetWidth(), pBackground->GetHeight() };
	float wallSize{ 10.f };

	SVGParser::GetVerticesFromSvgFile("Resources/Level/test.svg", m_Platforms);
	/*m_Platforms.push_back(std::vector<Point2f>());
	m_Platforms[0].push_back(Point2f{ boundaries.left, boundaries.bottom + wallSize });
	m_Platforms[0].push_back(Point2f{ boundaries.left + boundaries.width, boundaries.bottom + wallSize});
	m_Platforms[0].push_back(Point2f{ boundaries.left + boundaries.width, boundaries.bottom});
	m_Platforms[0].push_back(Point2f{ boundaries.left, boundaries.bottom});
	m_Platforms[0].push_back(Point2f{ boundaries.left, boundaries.bottom + wallSize });*/
}

void Level::Draw() const
{
	m_pBackgroundTexture->Draw();

	DrawPlatforms();
}

void Level::SetBackgroundTexture(const Texture* pBackground)
{
	m_pBackgroundTexture = pBackground;
}

void Level::HandleCollision(Rectf& actorShape, Vector2f& actorVelocity) const
{
	if (actorShape.left < m_Boundaries.left)
	{
		actorVelocity.x = 0.f;
		actorShape.left = m_Boundaries.left;
	}
	if (actorShape.left + actorShape.width >= m_Boundaries.left + m_Boundaries.width)
	{
		actorVelocity.x = 0.f;
		actorShape.left = m_Boundaries.left + m_Boundaries.width - actorShape.width;
	}

	for (std::vector<Point2f> platformVertices : m_Platforms)
	{
		utils::HitInfo hitInfo{};
		Point2f RayP1{ actorShape.left + actorShape.width / 2, actorShape.bottom };
		Point2f RayP2{ RayP1.x, actorShape.bottom + actorShape.height };
		utils::Raycast(platformVertices, RayP1, RayP2, hitInfo);

		if (hitInfo.intersectPoint.x > 0.0001f && hitInfo.intersectPoint.y > 0.0001f)
		{
			if(actorVelocity.y <= 0) actorShape.bottom = hitInfo.intersectPoint.y;
			actorVelocity.y = (actorVelocity.y <= 0) ? 0.f : -actorVelocity.y;
			return;
		}

	}
}

bool Level::IsOnGround(const Rectf& actorShape) const
{
	for (std::vector<Point2f> platformVertices : m_Platforms)
	{
		utils::HitInfo hitInfo{};
		Point2f RayP1{ actorShape.left + actorShape.width / 2, actorShape.bottom + actorShape.height / 2 };
		Point2f RayP2{ RayP1.x, actorShape.bottom - 1.f };
		utils::Raycast(platformVertices, RayP1, RayP2, hitInfo);

		bool isOnGround{ !(hitInfo.intersectPoint.x < 0.0001f && hitInfo.intersectPoint.y < 0.0001f) };
		
		if (isOnGround) return true;
	}

	return false;
}

Rectf Level::GetBoundaries() const
{
	return m_Boundaries;
}

void Level::DrawPlatforms() const
{
	for (std::vector<Point2f> platform : m_Platforms)
	{
		Rectf platformRect{GetRectfForVertices(platform)};
		int amountOfTiles{ int(platformRect.width / m_pBrick->GetWidth()) };
		for (size_t i = 0; i < amountOfTiles; i++)
		{
			Rectf brick{ platformRect.left + i * m_pBrick->GetWidth(), platformRect.bottom, m_pBrick->GetWidth(), m_pBrick->GetHeight() };
			m_pBrick->Draw(brick);
			glColor3f(1.f, 0.f, 0.f);
			utils::DrawRect(brick);
		}
	}
}

Rectf Level::GetRectfForVertices(std::vector<Point2f> vertices) const
{
	float xValues[4]{ vertices[0].x, vertices[1].x, vertices[2].x, vertices[3].x };
	float left{ min(xValues, 4) };

	float yValues[4]{vertices[0].y, vertices[1].y, vertices[2].y, vertices[3].y};
	float bottom{min(yValues, 4)};
	
	float width{ max(xValues, 4) - left };
	float height{ max(yValues, 4) - bottom };

	return Rectf{ left, bottom, width, height };
}

float Level::min(float* pNumbers, int size) const
{
	float min{pNumbers[0]};

	for (size_t i = 1; i < size; i++)
	{
		if (pNumbers[i] < min) min = pNumbers[i];
	}

	return min;
}

float Level::max(float* pNumbers, int size) const
{
	float max{ pNumbers[0] };

	for (size_t i = 1; i < size; i++)
	{
		if (pNumbers[i] > max) max = pNumbers[i];
	}

	return max;
}
