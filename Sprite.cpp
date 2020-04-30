#include "pch.h"
#include "Sprite.h"

Sprite::Sprite(const std::string& filename, int nrCols, int nrRows, float frameSec)
	: m_pTexture(new Texture(filename))
	, m_Cols{nrCols}
	, m_Rows{nrRows}
	, m_FrameSec{frameSec}
	, m_AccuSec{0.f}
	, m_ActFrame{0}
	, m_AmountOfLoopsRemaining{-1}
{
}

Sprite::~Sprite()
{
	delete m_pTexture;
	m_pTexture = nullptr;
}

void Sprite::Update(float elapsedSec)
{
	m_AccuSec += elapsedSec;
	if (m_AccuSec >= m_FrameSec && m_AmountOfLoopsRemaining != 0)
	{
		if (m_ActFrame + 1 == m_Rows * m_Cols)
		{
			m_AmountOfLoopsRemaining--;
			if (m_AmountOfLoopsRemaining == 0) return;
		}
		++m_ActFrame %= (m_Rows * m_Cols);
		m_AccuSec = 0.f;
	}
}

void Sprite::Draw(const Point2f& pos, float scale)
{
	Rectf srcRect{ GetFrameWidth() * (m_ActFrame % m_Cols), GetFrameHeight() * (m_ActFrame / m_Rows), GetFrameWidth() , GetFrameHeight() };
	Rectf dest{ pos.x, pos.y, GetFrameWidth() * scale, GetFrameHeight() * scale };
	m_pTexture->Draw(dest, srcRect);
}

void Sprite::Draw(const Rectf& destRect)
{
	Rectf srcRect{ GetFrameWidth() * (m_ActFrame % m_Cols), GetFrameHeight() * (m_ActFrame / m_Rows), GetFrameWidth() , GetFrameHeight() };
	m_pTexture->Draw(destRect, srcRect);
}

float Sprite::GetFrameWidth()
{
	return m_pTexture->GetWidth() / m_Cols;
}

float Sprite::GetFrameHeight()
{
	return m_pTexture->GetHeight() / m_Rows;
}

void Sprite::SetFrame(int frameNr)
{
	m_ActFrame = frameNr;
}

void Sprite::Randomize()
{
	m_ActFrame = rand() % (m_Rows * m_Cols - 1);
}

//Set to -1 for infinite loops
void Sprite::Loop(int amountOfLoops)
{
	m_AmountOfLoopsRemaining = amountOfLoops;
}

bool Sprite::HasEnded() const
{
	return m_ActFrame == m_Rows * m_Cols - 1;
}
