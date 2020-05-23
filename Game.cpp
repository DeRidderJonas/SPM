#include "pch.h"
#include "Game.h"
#include "Managers.h"
#include <iostream>
#include <sstream>
#include <fstream>

Game::Game( const Window& window ) 
	: m_Window{ window }
	, m_Camera{window.width, window.height}
	, m_IsPlayerInMenu{false}
	, m_PickUpDuration{1.f}
	, m_PickUpDurationPixl{2.5f}
	, m_PickUpRem{0.f}
	, m_IsInventoryActive{true}
	, m_Level{0}
	, m_SaveFileName{"SaveFile.txt"}
	, m_ContinuingFromSave{false}
{
	Initialize( );
}

Game::~Game( )
{
	Cleanup( );
}

void Game::Initialize( )
{
	InitManagers();
	InitPlayer();

	LoadGame();

	AdvanceToNextLevel();

	Managers::GetInstance()->GetItemManager()->Spawn(Item::Type::RedShroomshake, Point2f{200.f, 10.f}, m_pPlayer);

	NotifyCameraLevelChange();
}

void Game::InitPlayer()
{
	m_pPlayer = new Player(Point2f{ 10.f,50.f });
	Managers::GetInstance()->GetEnemyManager()->SetPlayer(m_pPlayer);
}

void Game::InitManagers()
{
	Managers::GetInstance();
}

void Game::Cleanup( )
{
	DestroyPlayer();
	DestroyManagers();
	DestroyLevel();
}

void Game::DestroyPlayer()
{
	delete m_pPlayer;
}

void Game::DestroyManagers()
{
	delete Managers::GetInstance();
}

void Game::DestroyLevel()
{
	delete m_pLevel;
}

void Game::Update( float elapsedSec )
{
	if (m_IsPlayerInMenu)
	{
	}
	else
	{
		if (m_PickUpRem < 0.f)
		{
			UpdatePlayer(elapsedSec);
			UpdateEnemies(elapsedSec);
			UpdateObjects(elapsedSec);

			DoCollisionTests();
			m_pPlayer->SetIsPickingUp(false);
		}
		else m_PickUpRem -= elapsedSec;
	}
}

void Game::UpdatePlayer(float elapsedSec)
{
	m_pPlayer->Update(elapsedSec, m_pLevel);
}

void Game::UpdateEnemies(float elapsedSec)
{
	Managers::GetInstance()->GetEnemyManager()->UpdateAll(elapsedSec, m_pLevel);
	Managers::GetInstance()->GetEnemyManager()->AllAttack();
}

void Game::UpdateObjects(float elapsedSec)
{
	Managers::GetInstance()->GetProjectileManager()->UpdateAll(elapsedSec, m_pLevel);

	if (m_InRestArea)
	{
		Sprite* pChest{ Managers::GetInstance()->GetSpriteManager()->GetSprite(SpriteManager::SpriteType::Chest) };
		pChest->Update(elapsedSec);

		if (pChest->HasEnded() && !m_ChestOpened)
		{
			PixlManager* pPixlManager{ Managers::GetInstance()->GetPixlManager() };
			Pixl::Type toUnlock{ pPixlManager->GetNextUnlockablePixl() };
			pPixlManager->Unlock(toUnlock);
			m_ChestOpened = true;
			m_PickUpRem = m_PickUpDurationPixl;
			m_PickUpIsItem = false;
			m_pPlayer->SetIsPickingUp(true);
		}
	}


}

void Game::NotifyCameraLevelChange()
{
	m_Camera.SetLevelBoundaries(m_pLevel->GetBoundaries());
}

void Game::Draw( ) const
{
	ClearBackground( );

	glPushMatrix();
		m_Camera.Transform(m_pPlayer->GetHitbox());
		DrawLevel();
		DrawPlayer();
		DrawEnemies();
		DrawProjectiles();
		DrawItems();
	glPopMatrix();

	DrawHUD();

	if (m_IsPlayerInMenu)
	{
		DrawMenus();
	}
}

void Game::DrawPlayer() const
{
	m_pPlayer->Draw();
}

void Game::DrawEnemies() const
{
	Managers::GetInstance()->GetEnemyManager()->DrawAll();
}

void Game::DrawProjectiles() const
{
	Managers::GetInstance()->GetProjectileManager()->DrawAll();
}

void Game::DrawItems() const
{
	Managers::GetInstance()->GetItemManager()->DrawAllFloorItems();
	if (m_PickUpRem > 0.f)
	{
		float margin{ 20.f };
		Rectf drawRect{ m_pPlayer->GetHitbox().left - 2 * margin, m_pPlayer->GetHitbox().bottom + m_pPlayer->GetHitbox().height - margin, 
			m_pPlayer->GetHitbox().width + 4 * margin, m_pPlayer->GetHitbox().height + 2 * margin };
		
		if(m_PickUpIsItem) Managers::GetInstance()->GetItemManager()->DrawPickUpItem(drawRect);
		else Managers::GetInstance()->GetPixlManager()->DrawUnlockingPixl(drawRect);
	}
}

void Game::DrawHUD() const
{
	const float margin{ 10.f };
	Texture* pHealthbar{ Managers::GetInstance()->GetTextureManager()->GetTexture(TextureManager::TextureType::Healthbar) };
	Rectf destRect{ margin, m_Window.height - pHealthbar->GetHeight() - margin, pHealthbar->GetWidth(), pHealthbar->GetHeight() };
	pHealthbar->Draw(destRect);

	//Health numbers
	Point2f bottomLeft{ 2.f * margin + pHealthbar->GetWidth() / 3, m_Window.height - pHealthbar->GetHeight() + margin * 0.5f };
	Sprite* pNumbers = Managers::GetInstance()->GetSpriteManager()->GetSprite(SpriteManager::SpriteType::Numbers);
	const float numberWidth{ pNumbers->GetFrameWidth() };
	Sprite* pOperator = Managers::GetInstance()->GetSpriteManager()->GetSprite(SpriteManager::SpriteType::Operators);
	pOperator->SetFrame(1);

	int CurrHealthNum1{ m_pPlayer->GetCurrentHealth() / 10 }, CurrHealthNum2{ m_pPlayer->GetCurrentHealth() % 10 };
	pNumbers->SetFrame(CurrHealthNum1);
	pNumbers->Draw(bottomLeft);
	bottomLeft.x += numberWidth;
	pNumbers->SetFrame(CurrHealthNum2);
	pNumbers->Draw(bottomLeft);
	bottomLeft.x += numberWidth;

	pOperator->Draw(bottomLeft);
	bottomLeft.x += pOperator->GetFrameWidth();
	
	int MaxHealthNum1{ m_pPlayer->GetMaxHealth() / 10 }, MaxHealthNum2{ m_pPlayer->GetMaxHealth() % 10 };
	pNumbers->SetFrame(MaxHealthNum1);
	pNumbers->Draw(bottomLeft);
	bottomLeft.x += numberWidth;
	pNumbers->SetFrame(MaxHealthNum2);
	pNumbers->Draw(bottomLeft);
	bottomLeft.x += numberWidth;
}

void Game::DrawLevel() const
{
	m_pLevel->Draw();
}

void Game::DrawMenus() const
{
	Point2f topLeft{ 2 * m_Window.width / 7, 4 * m_Window.height / 5 };
	Rectf descriptionRect{ m_Window.width / 5, 20.f, 2 * m_Window.width / 3, 50.f};
	
	Managers::GetInstance()->GetPixlManager()->DrawPixlsMenu(topLeft, !m_IsInventoryActive, descriptionRect);
	topLeft.x += 2 * m_Window.width / 7;
	
	Managers::GetInstance()->GetItemManager()->DrawInventoryItems(topLeft, m_IsInventoryActive, descriptionRect);
}

void Game::DoCollisionTests()
{
	Managers::GetInstance()->GetEnemyManager()->HitPlayer();
	Managers::GetInstance()->GetEnemyManager()->AttackAll(m_pPlayer->GetAttackHitbox());
	Managers::GetInstance()->GetProjectileManager()->HitSentients(m_pPlayer);
	if (Managers::GetInstance()->GetItemManager()->IsOverlapping(m_pPlayer))
	{
		m_PickUpRem = m_PickUpDuration;
		m_PickUpIsItem = true;
	}
}

void Game::ProcessKeyDownEvent( const SDL_KeyboardEvent & e )
{
	//std::cout << "KEYDOWN event: " << e.keysym.sym << std::endl;
}

void Game::ProcessKeyUpEvent( const SDL_KeyboardEvent& e )
{
	//std::cout << "KEYUP event: " << e.keysym.sym << std::endl;
	if(e.keysym.sym == SDLK_ESCAPE)	m_IsPlayerInMenu = !m_IsPlayerInMenu;

	if (m_IsPlayerInMenu)
	{
		switch (e.keysym.sym)
		{
		case SDLK_w:
		case SDLK_UP:
			if (m_IsInventoryActive) Managers::GetInstance()->GetItemManager()->Scroll(true);
			else Managers::GetInstance()->GetPixlManager()->Scroll(true);
			break;
		case SDLK_s:
		case SDLK_DOWN:
			if (m_IsInventoryActive) Managers::GetInstance()->GetItemManager()->Scroll(false);
			else Managers::GetInstance()->GetPixlManager()->Scroll(false);
			break;
		case SDLK_a:
		case SDLK_LEFT:
		case SDLK_d:
		case SDLK_RIGHT:
			m_IsInventoryActive = !m_IsInventoryActive;
			break;
		case SDLK_SPACE:
			if (m_IsInventoryActive)	Managers::GetInstance()->GetItemManager()->UseActiveItem();
			else Managers::GetInstance()->GetPixlManager()->ActivateSelectedPixl(m_pPlayer);
			m_IsPlayerInMenu = false;
			break;
		}
	}
	else
	{
		switch (e.keysym.sym)
		{
		case SDLK_i:
			PrintControlsInfo();
			break;
		case SDLK_w:
			if (m_InRestArea)
			{
				Rectf chest{ m_pLevel->GetChest() };
				Rectf chestInteractableArea{ chest.left - 25.f, chest.bottom, chest.width + 50.f, chest.height };
				if (m_pPlayer->IsOverlapping(chestInteractableArea))
				{
					Managers::GetInstance()->GetSpriteManager()->GetSprite(SpriteManager::SpriteType::Chest)->Loop(1);
					break;
				}
				if (!m_pPlayer->IsOverlapping(m_pLevel->GetDoor())) break;
			}
			else
			{
				bool playerHasKey{ Managers::GetInstance()->GetItemManager()->InventoryHasType(Item::Type::Key) };
				if (!playerHasKey || !m_pPlayer->IsOverlapping(m_pLevel->GetDoor())) break;
			}

			AdvanceToNextLevel();
			break;
		case SDLK_p:
			SaveGame();
			break;
		}
	}
}

void Game::ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e )
{
	//std::cout << "MOUSEMOTION event: " << e.x << ", " << e.y << std::endl;
}

void Game::ProcessMouseDownEvent( const SDL_MouseButtonEvent& e )
{
	//std::cout << "MOUSEBUTTONDOWN event: ";
	//switch ( e.button )
	//{
	//case SDL_BUTTON_LEFT:
	//	std::cout << " left button " << std::endl;
	//	break;
	//case SDL_BUTTON_RIGHT:
	//	std::cout << " right button " << std::endl;
	//	break;
	//case SDL_BUTTON_MIDDLE:
	//	std::cout << " middle button " << std::endl;
	//	break;
	//}
}

void Game::ProcessMouseUpEvent( const SDL_MouseButtonEvent& e )
{
	//std::cout << "MOUSEBUTTONUP event: ";
	//switch ( e.button )
	//{
	//case SDL_BUTTON_LEFT:
	//	std::cout << " left button " << std::endl;
	//	break;
	//case SDL_BUTTON_RIGHT:
	//	std::cout << " right button " << std::endl;
	//	break;
	//case SDL_BUTTON_MIDDLE:
	//	std::cout << " middle button " << std::endl;
	//	break;
	//}
}

void Game::ClearBackground( ) const
{
	glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT );
}

void Game::PrintControlsInfo() const
{
	std::cout << "Welcome to Super Paper Mario!" << '\n';
	std::cout << "----Ingame controls----" << '\n';
	std::cout << "Move left: A" << '\n';
	std::cout << "Move right: D" << '\n';
	std::cout << "Jump: SPACEBAR" << '\n';
	std::cout << "Attack (with currently activated Pixl): F" << '\n';
	std::cout << "Interact: W" << '\n';
	std::cout << "Open Menu: ESCAPE" << '\n';
	std::cout << "----Menu controls----" << '\n';
	std::cout << "Go up: W" << '\n';
	std::cout << "Go down: S" << '\n';
	std::cout << "Go to menu to the left: A" << '\n';
	std::cout << "Go to menu to the right: D" << '\n';
	std::cout << "Use/Select: SPACEBAR" << '\n';
}

void Game::AdvanceToNextLevel()
{
	Managers::GetInstance()->GetItemManager()->RemoveKey();
	Managers::GetInstance()->GetEnemyManager()->SetKeySpawned(false);
	Managers::GetInstance()->GetItemManager()->DestroyAllFloorItems();
	Managers::GetInstance()->GetEnemyManager()->KillAllEnemies();
	Managers::GetInstance()->GetProjectileManager()->DestroyAll();

	if (!m_ContinuingFromSave)
	{
		m_Level++;
		m_ChestOpened = false;
		Managers::GetInstance()->GetSpriteManager()->GetSprite(SpriteManager::SpriteType::Chest)->SetFrame(0);
	}
	else m_ContinuingFromSave = false;
	m_pPlayer->SetPosition(Point2f{10.f, 50.f});
	m_InRestArea = m_Level % 3 == 0;
	if (!m_InRestArea)
	{
		delete m_pLevel;
		m_pLevel = new Level(Managers::GetInstance()->GetTextureManager()->GetRandomBackground(), false);
		SpawnEnemies();
	}
	else
	{
		delete m_pLevel;
		m_pLevel = new Level(Managers::GetInstance()->GetTextureManager()->GetTexture(TextureManager::TextureType::RestArea), true);
	}

	m_Camera.SetLevelBoundaries(m_pLevel->GetBoundaries());
}

void Game::SpawnEnemies()
{
	EnemyManager* em{ Managers::GetInstance()->GetEnemyManager() };

	Rectf spawnBox{ m_pLevel->GetBoundaries() };
	spawnBox.bottom += 20.f;
	spawnBox.height -= 100.f;

	switch (m_Level)
	{
	case 1:
		em->Spawn(Enemy::Type::Goomba, 2, spawnBox);
		em->Spawn(Enemy::Type::Spiny, 1, spawnBox);
		break;
	case 2:
		em->Spawn(Enemy::Type::Squiglet, 1, spawnBox);
		em->Spawn(Enemy::Type::Goomba, 2, spawnBox);
		em->Spawn(Enemy::Type::Spiny, 1, spawnBox);
		break;
	default:
		em->Spawn(Enemy::Type::Goomba, 1, spawnBox);
	}
}

void Game::SaveGame() const
{
	std::ofstream os{ m_SaveFileName };
	os << m_Level << '\n';
	os << m_pPlayer->GetCurrentHealth() << '\n';
	os << m_pPlayer->GetMaxHealth() << '\n';
	os << Managers::GetInstance()->GetPixlManager()->ToSaveFormat() << '\n';
	os << Managers::GetInstance()->GetItemManager()->ToSaveFormat();
}

void Game::LoadGame()
{
	std::ifstream is{ m_SaveFileName };
	if (!is) return;

	m_ContinuingFromSave = true;

	std::string line;
	//level
	std::getline(is, line);
	m_Level = std::stoi(line);
	//HP
	std::getline(is, line);
	int currentHealth = std::stoi(line);
	std::getline(is, line);
	m_pPlayer->HealthFromLoad(currentHealth, std::stoi(line));
	//Pixls
	std::getline(is, line);
	Managers::GetInstance()->GetPixlManager()->LoadFromSave(line);
	//Items
	std::getline(is, line);
	Managers::GetInstance()->GetItemManager()->LoadFromSave(line, m_pPlayer);

	//Chest should already be open
	m_ChestOpened = true;
	Managers::GetInstance()->GetSpriteManager()->GetSprite(SpriteManager::SpriteType::Chest)->SetFrame(2);
}
