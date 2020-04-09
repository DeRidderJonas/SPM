#pragma once
#include "Texture.h"
#include "structs.h"
class Sprite
{
public:
	Sprite(const std::string& filename, int nrCols, int nrRows, float frameSec);
	~Sprite();
	void Update(float elapsedSec);
	void Draw(const Point2f& pos, float scale = 1.f);
	void Draw(const Rectf& destRect);
	float GetFrameWidth();
	float GetFrameHeight();

	void SetFrame(int frameNr);
	void Randomize();
private:
	Texture* m_pTexture;
	int m_Cols, m_Rows;
	float m_FrameSec, m_AccuSec;
	int m_ActFrame;
};

