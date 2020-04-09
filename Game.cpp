#include "pch.h"
#include "Game.h"
#include "Managers.h"
#include <iostream>

Game::Game( const Window& window ) 
	: m_Window{ window }
	, m_Camera{window.width, window.height}
	, m_IsPlayerInMenu{false}
	, m_PickUpDuration{1.f}
	, m_PickUpRem{0.f}
	, m_IsInventoryActive{true}
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
	InitLevel();
	InitPlayer();
	NotifyCameraLevelChange();

	//TODO: actually spawn enemies according to floor lvl
	Rectf spawnBox{ m_pLevel->GetBoundaries() };
	spawnBox.bottom += 20.f;
	Managers::GetInstance()->GetEnemyManager()->Spawn(Enemy::Type::Goomba, 3, spawnBox);
	Managers::GetInstance()->GetEnemyManager()->Spawn(Enemy::Type::Spiny, 1, spawnBox);
	Managers::GetInstance()->GetEnemyManager()->Spawn(Enemy::Type::Squiglet, 1, spawnBox);

	Managers::GetInstance()->GetItemManager()->Spawn(Item::Type::RedShroomshake, Point2f{200.f, 10.f}, m_pPlayer);
	Managers::GetInstance()->GetItemManager()->Spawn(Item::Type::RedShroomshake, Point2f{10.f, 10.f}, m_pPlayer);
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

void Game::InitLevel()
{
	m_pLevel = new Level(Managers::GetInstance()->GetTextureManager()->GetRandomBackground(), 
		Managers::GetInstance()->GetTextureManager()->GetTexture(TextureManager::TextureType::Brick));
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
			UpdateProjectiles(elapsedSec);

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

void Game::UpdateProjectiles(float elapsedSec)
{
	Managers::GetInstance()->GetProjectileManager()->UpdateAll(elapsedSec, m_pLevel);
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
		Rectf itemRect{ m_pPlayer->GetHitbox().left - 2 * margin, m_pPlayer->GetHitbox().bottom + m_pPlayer->GetHitbox().height - margin, 
			m_pPlayer->GetHitbox().width + 4 * margin, m_pPlayer->GetHitbox().height + 2 * margin };
		
		Managers::GetInstance()->GetItemManager()->DrawPickUpItem(itemRect);
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
	if (Managers::GetInstance()->GetItemManager()->IsOverlapping(m_pPlayer)) m_PickUpRem = m_PickUpDuration;
}

void Game::ProcessKeyDownEvent( const SDL_KeyboardEvent & e )
{
	//std::cout << "KEYDOWN event: " << e.keysym.sym << std::endl;
}

void Game::ProcessKeyUpEvent( const SDL_KeyboardEvent& e )
{
	//std::cout << "KEYUP event: " << e.keysym.sym << std::endl;
	if(e.keysym.sym == SDLK_ESCAPE)	m_IsPlayerInMenu = !m_IsPlayerInMenu;
	if (e.keysym.sym == SDLK_i) PrintControlsInfo();

	if (!m_IsPlayerInMenu) return;
	
	switch ( e.keysym.sym )
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
	std::cout << "Open Menu: ESCAPE" << '\n';
	std::cout << "----Menu controls----" << '\n';
	std::cout << "Go up: W" << '\n';
	std::cout << "Go down: S" << '\n';
	std::cout << "Go to menu to the left: A" << '\n';
	std::cout << "Go to menu to the right: D" << '\n';
	std::cout << "Use/Select: SPACEBAR" << '\n';
}
