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

	SVGParser::GetVerticesFromSvgFile("Resources/Level/level1.svg", m_Platforms);
	/*m_Platforms.push_back(std::vector<Point2f>());
	m_Platforms[0].push_back(Point2f{ boundaries.left, boundaries.bottom + wallSize });
	m_Platforms[0].push_back(Point2f{ boundaries.left + boundaries.width, boundaries.bottom + wallSize});
	m_Platforms[0].push_back(Point2f{ boundaries.left + boundaries.width, boundaries.bottom});
	m_Platforms[0].push_back(Point2f{ boundaries.left, boundaries.bottom});
	m_Platforms[0].push_back(Point2f{ boundaries.left, boundaries.bottom + wallSize });*/
	std::vector<Point2f> p{};
	p.push_back(Point2f{ 0.f, 450.f });
	p.push_back(Point2f{ 0.f, 470.f });
	p.push_back(Point2f{ 300.f, 470.f });
	p.push_back(Point2f{ 300.f, 450.f });
	m_Platforms.push_back(p);
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
			if (actorVelocity.y <= 0)
			{
				actorShape.bottom = hitInfo.intersectPoint.y;
				Rectf platformRect{ GetRectfForVertices(platformVertices) };
				if (actorShape.bottom < platformRect.bottom + platformRect.height / 2) actorShape.bottom = platformRect.bottom + platformRect.height;
			}
			actorVelocity.y = (actorVelocity.y <= 0) ? 0.f : -actorVelocity.y;
			return;
		}
	}
}

void Level::HandleWallCollision(Rectf& actorShape, Vector2f& actorVelocity) const
{
	for (std::vector<Point2f> platformVertices : m_Platforms)
	{
		Rectf wall{ GetRectfForVertices(platformVertices) };
		bool isWall{ wall.height > 50.f };

		if (isWall && utils::IsOverlapping(wall, actorShape))
		{
			bool actorLeftOfWall{ actorShape.left + actorShape.width - 2.f <= wall.left };
			if (actorLeftOfWall) actorShape.left = wall.left - actorShape.width - 2.f;
			else actorShape.left = wall.left + wall.width + 2.f;
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

bool Level::IsNextToWall(const Rectf& actorShape) const
{
	for (std::vector<Point2f> platformVertices : m_Platforms)
	{
		Rectf wall{ GetRectfForVertices(platformVertices) };
		bool isWall{ wall.height > 50.f };

		if (isWall && utils::IsOverlapping(wall, actorShape))
		{
			return true;
		}
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
		}

		int verticalAmountOfTiles{ int(platformRect.height / m_pBrick->GetHeight()) };
		for (size_t i = 0; i < verticalAmountOfTiles; i++)
		{
			Rectf brick{ platformRect.left, platformRect.bottom + i * m_pBrick->GetHeight(), m_pBrick->GetWidth(), m_pBrick->GetHeight() };
			m_pBrick->Draw(brick);
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
