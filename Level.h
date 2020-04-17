#pragma once
#include <vector>
#include "Texture.h"
#include "Vector2f.h"
class Level
{
public:
	Level(const Texture* pBackground);
	
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
private:
	std::vector<std::vector<Point2f>> m_Platforms;
	const Texture* m_pBackgroundTexture;
	const Texture* m_pBrick;
	const Texture* m_pDoor;
	Rectf m_Boundaries;
	Rectf m_Door;

	void DrawPlatforms() const;
	Rectf GetRectfForVertices(std::vector<Point2f> vertices) const;
	float min(float* pNumbers, int size) const;
	float max(float* pNumbers, int size) const;
};

