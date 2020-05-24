#pragma once
#include "Player.h"
#include "SpriteManager.h"
#include "EnemyManager.h"
#include "Level.h"
#include "Camera.h"
#include "TextureManager.h"
#include "ProjectileManager.h"
#include "ItemManager.h"
#include "TextManager.h"
#include "Merchant.h"

class Game
{
public:
	explicit Game( const Window& window );
	Game(const Game& other) = delete;
	Game& operator=(const Game& other) = delete;
	Game( Game&& other) = delete;
	Game& operator=(Game&& other) = delete;
	~Game();

	void Update( float elapsedSec );
	void Draw( ) const;

	// Event handling
	void ProcessKeyDownEvent( const SDL_KeyboardEvent& e );
	void ProcessKeyUpEvent( const SDL_KeyboardEvent& e );
	void ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e );
	void ProcessMouseDownEvent( const SDL_MouseButtonEvent& e );
	void ProcessMouseUpEvent( const SDL_MouseButtonEvent& e );

	enum class TitleScreenSelection
	{
		TitleScreen, EraseData, Play, Sound
	};
	enum class InGameMenuSelection
	{
		Pixl, Inventory, Settings
	};
private:
	// DATA MEMBERS
	const Window m_Window;
	Camera m_Camera;
	Player* m_pPlayer;

	Level* m_pLevel;
	int m_Level;
	bool m_InRestArea;
	Merchant* m_pMerchant;

	bool m_IsPlayerInMenu;
	InGameMenuSelection m_InGameMenuSelection;
	float m_PickUpDuration;
	float m_PickUpDurationPixl;
	float m_PickUpRem;
	bool m_PickUpIsItem;
	bool m_ChestOpened;

	std::string m_SaveFileName;
	bool m_ContinuingFromSave;
	bool m_InTitleScreen;
	TitleScreenSelection m_TitleScreenSelection;

	// FUNCTIONS
	void Initialize( );
	void InitPlayer();
	void InitManagers();
	//void InitLevel();
	void Cleanup( );
	void DestroyPlayer();
	void DestroyManagers();
	void DestroyLevel();
	void UpdatePlayer(float elapsedSec);
	void UpdateEnemies(float elapsedSec);
	void UpdateObjects(float elapsedSec);
	void UpdateParticles(float elapsedSec);
	void NotifyCameraLevelChange();
	void DrawPlayer() const;
	void DrawEnemies() const;
	void DrawProjectiles() const;
	void DrawItems() const;
	void DrawParticles() const;
	void DrawHUD() const;
	void DrawLevel() const;
	void DrawTitleScreen() const;
	void DrawMenus() const;
	void DrawShop() const;
	void DoCollisionTests();
	void NavigateTitleScreen(const SDL_KeyboardEvent& e);
	void ClearBackground( ) const;

	void PrintControlsInfo() const;
	void AdvanceToNextLevel();
	void SpawnEnemies();

	void StartGame();
	void GameOver();
	void GoBackToTitleScreen();
	void SaveGame() const;
	void LoadGame();
};