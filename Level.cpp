#include "pch.h"
#include "Level.h"
#include "utils.h"
#include <iostream>
#include "SVGParser.h"
#include "Managers.h"

const float Level::m_MinWallHeight{ 100.f };
const float Level::m_ParalaxFactor{ 0.2f };

Level::Level(const Texture* pBackground, bool isRestArea, bool isBossLevel)
	: m_pBackgroundTexture{pBackground}
	, m_pBrick{Managers::GetInstance()->GetTextureManager()->GetTexture(TextureManager::TextureType::Brick)}
	, m_pDoor{Managers::GetInstance()->GetTextureManager()->GetTexture(TextureManager::TextureType::Door)}
	, m_pChest{Managers::GetInstance()->GetSpriteManager()->GetSprite(SpriteManager::SpriteType::Chest)}
	, m_pPipe{Managers::GetInstance()->GetTextureManager()->GetTexture(TextureManager::TextureType::Pipe)}
	, m_Boundaries{0.f,0.f, pBackground->GetWidth(), pBackground->GetHeight()}
	, m_IsRestArea{isRestArea}
	, m_IsBossLevel{isBossLevel}
{
	Rectf boundaries{ 0.f,0.f, pBackground->GetWidth(), pBackground->GetHeight() };
	float wallSize{ 10.f };

	if (isRestArea)
	{
		SVGParser::GetVerticesFromSvgFile("Resources/Level/restArea.svg", m_Platforms);
		m_Door = Rectf{ boundaries.left + boundaries.width - m_pDoor->GetWidth() - 300.f, boundaries.bottom + 18.f, m_pDoor->GetWidth(), m_pDoor->GetHeight() };
		m_Chest = Rectf{ boundaries.left + boundaries.width / 2, boundaries.bottom + 45.f, m_pChest->GetFrameWidth(), m_pChest->GetFrameHeight() };
		m_Pipe = Rectf{ boundaries.left + m_pBrick->GetWidth(), boundaries.bottom + m_pBrick->GetHeight(), m_pPipe->GetWidth(), m_pPipe->GetHeight() };
	}
	else if (isBossLevel)
	{
		SVGParser::GetVerticesFromSvgFile("Resources/Level/level_boss.svg", m_Platforms);
		m_Door = Rectf{ boundaries.left + boundaries.width - m_pDoor->GetWidth() - 150.f, boundaries.left + 10.f, m_pDoor->GetWidth(), m_pDoor->GetHeight() };
	}
	else
	{
		const int amountOfLevels{ 5 }; //edit this when new levels are added
		int randLevel{ rand() % amountOfLevels };
		SVGParser::GetVerticesFromSvgFile("Resources/Level/level_" + std::to_string(randLevel + 1) + ".svg", m_Platforms);

		m_Door.height = m_pDoor->GetHeight();
		m_Door.width = m_pDoor->GetWidth();

		int randPlatform{ rand() % int(m_Platforms.size()) };
		Rectf platformRect{ GetRectfForVertices(m_Platforms[randPlatform]) };
		bool isWall{ platformRect.height > m_MinWallHeight };
		while (isWall)
		{
			randPlatform = rand() % int(m_Platforms.size());
			platformRect = GetRectfForVertices(m_Platforms[randPlatform]);
			isWall = platformRect.height > m_MinWallHeight;
		}

		m_Door.bottom = platformRect.bottom + platformRect.height;
		m_Door.left = platformRect.left + platformRect.width / 2 - m_Door.width;
	}
}

void Level::Draw(const Point2f& cameraTransform) const
{
	glPushMatrix();
		glScalef(1.f + m_ParalaxFactor, 1.f + m_ParalaxFactor, 1.f);
		glTranslatef(m_ParalaxFactor * cameraTransform.x, m_ParalaxFactor * cameraTransform.y, 0.f);
		m_pBackgroundTexture->Draw();
	glPopMatrix();
	m_pDoor->Draw(m_Door);
	if (m_IsRestArea)
	{
		m_pChest->Draw(m_Chest);
		m_pPipe->Draw(m_Pipe);
	}
	DrawPlatforms();
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

	for (const std::vector<Point2f>& platformVertices : m_Platforms)
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

	if (m_IsRestArea)
	{
		utils::HitInfo hitInfo{};
		Point2f RayP1{ actorShape.left + actorShape.width / 2, actorShape.bottom };
		Point2f RayP2{ RayP1.x, actorShape.bottom + actorShape.height };
		std::vector<Point2f> vertices{ Point2f{m_Chest.left, m_Chest.bottom},
			Point2f{m_Chest.left + m_Chest.width, m_Chest.bottom},
			Point2f{m_Chest.left + m_Chest.width, m_Chest.bottom + m_Chest.height},
			Point2f{m_Chest.left, m_Chest.bottom + m_Chest.height} };
		utils::Raycast(vertices, RayP1, RayP2, hitInfo);

		if (hitInfo.intersectPoint.x > 0.0001f && hitInfo.intersectPoint.y > 0.0001f)
		{
			if (actorVelocity.y <= 0.f)
			{
				actorShape.bottom = hitInfo.intersectPoint.y - 1.f;
			}
			actorVelocity.y = 0.f;
		}
	}
}

void Level::HandleWallCollision(Rectf& actorShape, Vector2f& actorVelocity) const
{
	for (const std::vector<Point2f>& platformVertices : m_Platforms)
	{
		Rectf wall{ GetRectfForVertices(platformVertices) };
		bool isWall{ wall.height > m_MinWallHeight };

		if (isWall && utils::IsOverlapping(wall, actorShape))
		{
			bool actorLeftOfWall{ actorShape.left + actorShape.width - 2.f <= wall.left };
			if (actorLeftOfWall) actorShape.left = wall.left - actorShape.width - 2.f;
			else actorShape.left = wall.left + wall.width + 2.f;
		}
	}

	if (m_IsRestArea)
	{
		if (utils::IsOverlapping(actorShape, m_Chest)) 
		{
			if (abs(actorShape.bottom - (m_Chest.bottom + m_Chest.height)) < 25.f) return;
			bool actorLeftOfChest{ actorShape.left + actorShape.width - 2.f <= m_Chest.left };
			if (actorLeftOfChest) actorShape.left = m_Chest.left - actorShape.width - 2.f;
			else actorShape.left = m_Chest.left + m_Chest.width + 2.f;
			actorVelocity.x = 0.f;
		}
	}
}

bool Level::IsOnGround(const Rectf& actorShape) const
{
	for (const std::vector<Point2f>& platformVertices : m_Platforms)
	{
		utils::HitInfo hitInfo{};
		Point2f RayP1{ actorShape.left + actorShape.width / 2, actorShape.bottom + actorShape.height / 2 };
		Point2f RayP2{ RayP1.x, actorShape.bottom - 1.f };
		utils::Raycast(platformVertices, RayP1, RayP2, hitInfo);

		bool isOnGround{ !(hitInfo.intersectPoint.x < 0.0001f && hitInfo.intersectPoint.y < 0.0001f) };
		
		if (isOnGround) return true;
	}

	if (m_IsRestArea)
	{
		utils::HitInfo hitInfo{};
		Point2f RayP1{ actorShape.left + actorShape.width / 2, actorShape.bottom };
		Point2f RayP2{ RayP1.x, actorShape.bottom + actorShape.height };
		std::vector<Point2f> vertices{ Point2f{m_Chest.left, m_Chest.bottom},
			Point2f{m_Chest.left + m_Chest.width, m_Chest.bottom},
			Point2f{m_Chest.left + m_Chest.width, m_Chest.bottom + m_Chest.height},
			Point2f{m_Chest.left, m_Chest.bottom + m_Chest.height} };
		utils::Raycast(vertices, RayP1, RayP2, hitInfo);

		if (hitInfo.intersectPoint.x > 0.0001f && hitInfo.intersectPoint.y > 0.0001f)
		{
			return true;
		}
	}

	return false;
}

bool Level::IsNextToWall(const Rectf& actorShape) const
{
	for (const std::vector<Point2f>& platformVertices : m_Platforms)
	{
		Rectf wall{ GetRectfForVertices(platformVertices) };
		bool isWall{ wall.height > m_MinWallHeight };

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

Rectf Level::GetDoor() const
{
	return m_Door;
}

Rectf Level::GetChest() const
{
	return m_Chest;
}

Rectf Level::GetPipe() const
{
	return m_Pipe;
}

void Level::DrawPlatforms() const
{
	for (const std::vector<Point2f>& platform : m_Platforms)
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
	std::vector<float> xValues{ vertices[0].x, vertices[1].x, vertices[2].x, vertices[3].x };
	float left{ *std::min_element(xValues.begin(), xValues.end()) };

	std::vector<float> yValues{vertices[0].y, vertices[1].y, vertices[2].y, vertices[3].y};
	float bottom{*std::min_element(yValues.begin(), yValues.end())};
	
	float width{ *std::max_element(xValues.begin(), xValues.end()) - left };
	float height{ *std::max_element(yValues.begin(), yValues.end()) - bottom };

	return Rectf{ left, bottom, width, height };
}
