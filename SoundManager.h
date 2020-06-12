#pragma once
#include <map>
#include "SoundStream.h"
#include "SoundEffect.h"
class SoundManager
{
public:
	enum class Song
	{
		TitleScreen,
		MainTheme
	};

	enum class Soundfx
	{
		GameOver, 
		ItemPickup,
		PixlUnlock,
		Damage,
		Jump,
		Frozen,
		CudgeAttack,
		BarryAttack,
		BoomerAttack,
		BoomerFuse,
		BoomerExplode,
		EnemyDeath,
		CherbilDeath,
		SpinyDeath,
		CherbilAttack,
		SquigletAttack,
		Pow,
		Shroomshake,
		IceStorm,
		ProjectileBounce,
		MenuChange,
		MenuConfirm
	};

	SoundManager();
	~SoundManager();

	SoundManager(const SoundManager& other) = delete;
	SoundManager& operator=(const SoundManager& other) = delete;
	SoundManager(SoundManager&& other) = delete;
	SoundManager& operator=(SoundManager&& other) = delete;

	void PlayBackgroundMusic(Song song);
	void PlaySoundEffect(Soundfx sfx);

	void DrawSettingsMenu(const Point2f& topLeft, bool isActive, bool showCursor) const;
	void Scroll(bool up);
	void Confirm();

	bool IsAdjustingSetting() const;
private:
	enum class SettingSelection
	{
		MusicVolume, SfxVolume, Muted
	};
	std::map<Song, SoundStream*> m_Songs;
	std::map<Soundfx, Sfx*> m_SoundEffects;

	SettingSelection m_SettingSelection;
	bool m_IsAdjustingSetting;

	bool m_IsSoundOn;
	int m_MusicVolume;
	int m_SfxVolume;
	const static int m_MaxVolume;

	void SetSfxVolume(int volume);
	void SetMusicVolume(int volume);
	void ToggleMuted();
};

