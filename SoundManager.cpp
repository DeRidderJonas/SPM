#include "pch.h"
#include "SoundManager.h"
#include "Managers.h"
#include "utils.h"

const int SoundManager::m_MaxVolume{ 10 };

SoundManager::SoundManager()
	: m_SettingSelection{SettingSelection::MusicVolume}
	, m_IsSoundOn{true}
	, m_MusicVolume{0}
	, m_SfxVolume{0}
	, m_IsAdjustingSetting{false}
{
	m_Songs[Song::TitleScreen] = new SoundStream("Resources/Sound/TitleScreen.mp3");
	m_Songs[Song::MainTheme] = new SoundStream("Resources/Sound/MainTheme.mp3");
	m_Songs[Song::BossFight] = new SoundStream("Resources/Sound/BossFight.mp3");
	m_Songs[Song::AfterBossFight] = new SoundStream("Resources/Sound/AfterBossFight.mp3");
	m_Songs[Song::EndScreen] = new SoundStream("Resources/Sound/EndScreen.mp3");

	m_SoundEffects[Soundfx::GameOver] = new Sfx("Resources/Sound/GameOver.mp3");
	m_SoundEffects[Soundfx::ItemPickup] = new Sfx("Resources/Sound/ItemPickup.mp3");
	m_SoundEffects[Soundfx::PixlUnlock] = new Sfx("Resources/Sound/PixlUnlock.mp3");
	m_SoundEffects[Soundfx::Damage] = new Sfx("Resources/Sound/damage.wav");
	m_SoundEffects[Soundfx::Jump] = new Sfx("Resources/Sound/Jump.oga");
	m_SoundEffects[Soundfx::Frozen] = new Sfx("Resources/Sound/MarioFrozen.wav");
	m_SoundEffects[Soundfx::CudgeAttack] = new Sfx("Resources/Sound/cudgeAttack.oga");
	m_SoundEffects[Soundfx::BarryAttack] = new Sfx("Resources/Sound/BarryAttack.oga");
	m_SoundEffects[Soundfx::BoomerAttack] = new Sfx("Resources/Sound/BoomerAttack.ogg");
	m_SoundEffects[Soundfx::BoomerFuse] = new Sfx("Resources/Sound/BoomerFuse.wav");
	m_SoundEffects[Soundfx::BoomerExplode] = new Sfx("Resources/Sound/BoomerExplode.wav");
	m_SoundEffects[Soundfx::EnemyDeath] = new Sfx("Resources/Sound/EnemyDeath.oga");
	m_SoundEffects[Soundfx::CherbilDeath] = new Sfx("Resources/Sound/CherbilDeath.wav");
	m_SoundEffects[Soundfx::SpinyDeath] = new Sfx("Resources/Sound/spinyDeath.wav");
	m_SoundEffects[Soundfx::CherbilAttack] = new Sfx("Resources/Sound/CherbilAttack.ogg");
	m_SoundEffects[Soundfx::SquigletAttack] = new Sfx("Resources/Sound/SquigletAttack.ogg");
	m_SoundEffects[Soundfx::DimentioAttacked] = new Sfx("Resources/Sound/oof.mp3");
	m_SoundEffects[Soundfx::Pow] = new Sfx("Resources/Sound/Pow.oga");
	m_SoundEffects[Soundfx::Shroomshake] = new Sfx("Resources/Sound/ShroomShake.wav");
	m_SoundEffects[Soundfx::IceStorm] = new Sfx("Resources/Sound/IceStorm.flac");
	m_SoundEffects[Soundfx::ProjectileBounce] = new Sfx("Resources/Sound/ProjectileBounce.wav");
	m_SoundEffects[Soundfx::MenuChange] = new Sfx("Resources/Sound/MenuChange.wav");
	m_SoundEffects[Soundfx::MenuConfirm] = new Sfx("Resources/Sound/MenuConfirm.wav");
	m_SoundEffects[Soundfx::Buy] = new Sfx("Resources/Sound/buy.wav");

	SetMusicVolume(5);
	SetSfxVolume(8);
}

SoundManager::~SoundManager()
{
	for (std::pair<const Song, SoundStream*>& pair : m_Songs)
	{
		delete pair.second;
	}

	for (std::pair<const Soundfx, Sfx*>& pair : m_SoundEffects)
	{
		delete pair.second;
	}
}

void SoundManager::PlayBackgroundMusic(Song song)
{
	SoundStream::Stop();
	m_Songs[song]->Play(true);
}

void SoundManager::PlaySoundEffect(Soundfx sfx)
{
	m_SoundEffects[sfx]->Play(0);
}

void SoundManager::DrawSettingsMenu(const Point2f& topLeft, bool isActive, bool showCursor) const
{
	TextureManager* pTM{ Managers::GetInstance()->GetTextureManager() };

	Texture* pSettings{ pTM->GetTexture(TextureManager::TextureType::Settingsbar) };
	Point2f bottomLeft{ topLeft.x, topLeft.y - pSettings->GetHeight() };

	float margin{ 5.f };
	float menuHeight{ 250.f }, menuWidth{ pSettings->GetWidth() };
	Rectf menuRect{ bottomLeft.x - margin, bottomLeft.y - menuHeight + 3 * margin, menuWidth + 2 * margin, menuHeight };
	if (isActive)
	{
		glColor3f(1.f, 1.f, 1.f);
		utils::FillRect(menuRect);
		glColor3f(0.f, 0.f, 0.f);
		utils::DrawRect(menuRect, 2.f);
	}

	pSettings->Draw(bottomLeft);
	if (!isActive) return;
	Texture* pCursor{ pTM->GetTexture(TextureManager::TextureType::Pointer) };

	TextManager* pTxt{ Managers::GetInstance()->GetTextManager() };
	bottomLeft.x += margin;
	bottomLeft.y -= pSettings->GetHeight();
	pTxt->GetTexture(TextManager::Text::MusicVolume)->Draw(bottomLeft);
	if (showCursor && m_SettingSelection == SettingSelection::MusicVolume) pCursor->Draw(Point2f{bottomLeft.x - pCursor->GetWidth(), bottomLeft.y});
	Texture* pFull{ pTM->GetTexture(TextureManager::TextureType::SettingFull) };
	Texture* pEmpty{ pTM->GetTexture(TextureManager::TextureType::SettingEmpty) };

	Point2f bottomLeftSetting{ bottomLeft.x, bottomLeft.y - pFull->GetHeight() - margin };
	for (size_t i = 0; i <= m_MaxVolume; i++)
	{
		if (i < m_MusicVolume) pFull->Draw(bottomLeftSetting);
		else pEmpty->Draw(bottomLeftSetting);
		bottomLeftSetting.x += pFull->GetWidth();
	}

	bottomLeft.y = bottomLeftSetting.y - margin;
	utils::DrawLine(Point2f{menuRect.left, bottomLeft.y}, Point2f{menuRect.left + menuRect.width, bottomLeft.y}, 1.f);

	bottomLeft.y -= pFull->GetHeight() + margin;
	pTxt->GetTexture(TextManager::Text::SfxVolume)->Draw(bottomLeft);
	if (showCursor && m_SettingSelection == SettingSelection::SfxVolume) pCursor->Draw(Point2f{bottomLeft.x - pCursor->GetWidth(), bottomLeft.y});
	bottomLeftSetting.y = bottomLeft.y - margin - pFull->GetHeight();
	bottomLeftSetting.x = bottomLeft.x;
	for (size_t i = 0; i <= m_MaxVolume; i++)
	{
		if (i < m_SfxVolume) pFull->Draw(bottomLeftSetting);
		else pEmpty->Draw(bottomLeftSetting);
		bottomLeftSetting.x += pFull->GetWidth();
	}

	bottomLeft.y = bottomLeftSetting.y - margin;
	utils::DrawLine(Point2f{menuRect.left, bottomLeft.y}, Point2f{menuRect.left + menuRect.width, bottomLeft.y}, 1.f);

	bottomLeft.y -= pFull->GetHeight() + margin;
	Texture* pSound{ pTxt->GetTexture(TextManager::Text::Sound) };
	if (showCursor && m_SettingSelection == SettingSelection::Muted) pCursor->Draw(Point2f{bottomLeft.x - pCursor->GetWidth(), bottomLeft.y});
	pSound->Draw(bottomLeft);
	bottomLeft.y -= pSound->GetHeight();
	bottomLeft.x += 2 * margin;
	Texture* pOn{ pTxt->GetTexture(TextManager::Text::On) };
	pOn->Draw(bottomLeft);
	if (m_IsSoundOn) utils::DrawLine(Point2f{bottomLeft.x, bottomLeft.y - 2.f}, Point2f{bottomLeft.x + pOn->GetWidth(), bottomLeft.y - 2.f});
	bottomLeft.x += pSound->GetWidth();
	pTxt->GetTexture(TextManager::Text::Off)->Draw(bottomLeft);
	if (!m_IsSoundOn) utils::DrawLine(Point2f{bottomLeft.x, bottomLeft.y - 2.f}, Point2f{bottomLeft.x + pOn->GetWidth(), bottomLeft.y - 2.f});


}

void SoundManager::Scroll(bool up)
{
	if (!m_IsAdjustingSetting)
	{
		int newSelection{ int(m_SettingSelection) + (up ? -1 : 1) };
		newSelection %= int(SettingSelection::Muted) + 1;
		if (newSelection < 0) newSelection = int(SettingSelection::Muted);
		m_SettingSelection = SettingSelection(newSelection);
	}
	else
	{
		switch (m_SettingSelection)
		{
		case SoundManager::SettingSelection::MusicVolume:
			if (up && m_MusicVolume + 1 <= m_MaxVolume + 1)	SetMusicVolume(m_MusicVolume + 1);
			else if (!up && m_MusicVolume - 1 >= 0) SetMusicVolume(m_MusicVolume - 1);
			break;
		case SoundManager::SettingSelection::SfxVolume:
			if(up && m_SfxVolume + 1 <= m_MaxVolume + 1) SetSfxVolume(m_SfxVolume + 1);
			if(!up && m_SfxVolume - 1 >= 0) SetSfxVolume(m_SfxVolume - 1);
			break;
		case SoundManager::SettingSelection::Muted:
			ToggleMuted();
			break;
		}
	}
}

void SoundManager::Confirm()
{
	m_IsAdjustingSetting = !m_IsAdjustingSetting;
}

bool SoundManager::IsAdjustingSetting() const
{
	return m_IsAdjustingSetting;
}

void SoundManager::SetSfxVolume(int volume)
{
	m_SfxVolume = volume;
	for (std::pair<const Soundfx, Sfx*>& pair : m_SoundEffects)
	{
		pair.second->SetVolume(m_SfxVolume);
	}
}

void SoundManager::SetMusicVolume(int volume)
{
	m_MusicVolume = volume;
	SoundStream::SetVolume(volume);
}

void SoundManager::ToggleMuted()
{
	m_IsSoundOn = !m_IsSoundOn;
	if (!m_IsSoundOn)
	{
		SoundStream::SetVolume(0);
		for (std::pair<const Soundfx, Sfx*>& pair : m_SoundEffects)
		{
			pair.second->SetVolume(0);
		}
	}
	else
	{
		SetMusicVolume(m_MusicVolume);
		SetSfxVolume(m_SfxVolume);
	}
}
