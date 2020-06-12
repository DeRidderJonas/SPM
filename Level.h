#pragma once
#include <vector>
#include "Texture.h"
#include "Sprite.h"
#include "Vector2f.h"
class Level
{
public:
	Level(const Texture* pBackground, bool isRestArea);
	
	Level(const Level& other) = delete;
	Level& operator=(const Level& other) = delete;
	Level(Level&& other) = delete;
	Level& operator=(Level&& other) = delete;

	void Draw() const;

	void HandleCollision(Rectf& actorShape, Vector2f& actorVelocity) const;
	void HandleWallCollision(Rectf& actorShape, Vector2f& actorVelocity) const;
	bool IsOnGround(const Rectf& actorShape) const;
	bool IsNextToWall(const Rectf& actorShape) const;

	Rectf GetBoundaries() const;
	Rectf GetDoor() const;
	Rectf GetChest() const;
	Rectf GetPipe() const;
private:
	std::vector<std::vector<Point2f>> m_Platforms;
	const Texture* m_pBackgroundTexture;
	const Texture* m_pBrick;
	const Texture* m_pDoor;
	const Texture* m_pPipe;
	Sprite* m_pChest;
	Rectf m_Boundaries;
	Rectf m_Door;
	Rectf m_Chest;
	Rectf m_Pipe;

	bool m_IsRestArea;

	static const float m_MinWallHeight;

	void DrawPlatforms() const;
	Rectf GetRectfForVertices(std::vector<Point2f> vertices) const;
};

