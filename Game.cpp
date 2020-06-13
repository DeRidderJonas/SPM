#include "pch.h"
#include "Game.h"
#include "Managers.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <stdio.h>
#include "utils.h"

Game::Game( const Window& window ) 
	: m_Window{ window }
	, m_Camera{window.width, window.height}
	, m_IsPlayerInMenu{false}
	, m_PlayerHasWon{false}
	, m_PickUpDuration{1.f}
	, m_PickUpDurationPixl{2.5f}
	, m_PickUpRem{0.f}
	, m_InGameMenuSelection{InGameMenuSelection::Inventory}
	, m_Level{0}
	, m_SaveFileName{"SaveFile.txt"}
	, m_ContinuingFromSave{false}
	, m_InTitleScreen{true}
	, m_TitleScreenSelection{TitleScreenSelection::TitleScreen}
	, m_LevelsPerRestArea{3}
	, m_BossLevel{1}
	, m_AmountOfHearts{3}
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

	Managers::GetInstance()->GetSoundManager()->PlayBackgroundMusic(SoundManager::Song::TitleScreen);

	LoadGame();
}

void Game::InitPlayer()
{
	m_pPlayer = new Player(Point2f{ 10.f,50.f });
	Managers::GetInstance()->GetEnemyManager()->SetPlayer(m_pPlayer);
	Managers::GetInstance()->GetParticleManager()->SetPlayer(m_pPlayer);
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
	delete m_pMerchant;

	if (m_Level == m_BossLevel)
	{
		delete m_pBoss;
		for (Heart* heart : m_Hearts)
		{
			delete heart;
		}
	}
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
	if (m_InTitleScreen || m_IsPlayerInMenu || (m_pMerchant && m_pMerchant->IsPlayerInShop()))
	{
		return;
	}

	if (m_PickUpRem < 0.f)
	{
		UpdatePlayer(elapsedSec);
		UpdateEnemies(elapsedSec);
		UpdateObjects(elapsedSec);
		UpdateParticles(elapsedSec);
			DoCollisionTests();
		m_pPlayer->SetIsPickingUp(false);

		if (m_pPlayer->IsDead()) GameOver();
	}
	else m_PickUpRem -= elapsedSec;
}

void Game::UpdatePlayer(float elapsedSec)
{
	m_pPlayer->Update(elapsedSec, m_pLevel);
}

void Game::UpdateEnemies(float elapsedSec)
{
	Managers::GetInstance()->GetEnemyManager()->UpdateAll(elapsedSec, m_pLevel);
	Managers::GetInstance()->GetEnemyManager()->AllAttack();

	if (m_Level == m_BossLevel)
	{
		if (m_pBoss) m_pBoss->Update(elapsedSec, m_pLevel);

		for (Heart* heart : m_Hearts)
		{
			heart->Update(elapsedSec);
		}

		if (m_pBoss && m_pBoss->IsDead())
		{
			delete m_pBoss;
			m_pBoss = nullptr;
			Managers::GetInstance()->GetSoundManager()->PlayBackgroundMusic(SoundManager::Song::AfterBossFight);
		}
	}
}

void Game::UpdateObjects(float elapsedSec)
{
	Managers::GetInstance()->GetProjectileManager()->UpdateAll(elapsedSec, m_pLevel);

	if (m_InRestArea)
	{
		Sprite* pChest{ Managers::GetInstance()->GetSpriteManager()->GetSprite(SpriteManager::SpriteType::Chest) };
		pChest->Update(elapsedSec);
		m_pMerchant->Update(elapsedSec);

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

void Game::UpdateParticles(float elapsedSec)
{
	Managers::GetInstance()->GetParticleManager()->UpdateAll(elapsedSec, m_pLevel);
}

void Game::NotifyCameraLevelChange()
{
	m_Camera.SetLevelBoundaries(m_pLevel->GetBoundaries());
}

void Game::Draw( ) const
{
	ClearBackground( );

	if (m_InTitleScreen)
	{
		DrawTitleScreen();
		return;
	}

	glPushMatrix();
		m_Camera.Transform(m_pPlayer->GetHitbox());
		DrawLevel();
		DrawPlayer();
		DrawEnemies();
		DrawProjectiles();
		DrawItems();
		DrawParticles();
	glPopMatrix();

	DrawHUD();
	DrawShop();

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

	if (m_Level == m_BossLevel)
	{
		if (m_pBoss) m_pBoss->Draw();
		for (Heart* heart : m_Hearts)
		{
			heart->Draw();
		}
	}
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

void Game::DrawParticles() const
{
	Managers::GetInstance()->GetParticleManager()->DrawAll();
}

void Game::DrawHUD() const
{
	const float margin{ 10.f };
	//Health HUD
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

	//Coins HUD
	Texture* pCoinbar{ Managers::GetInstance()->GetTextureManager()->GetTexture(TextureManager::TextureType::Coinbar) };
	destRect.left = m_Window.width - pCoinbar->GetWidth() - margin;
	destRect.height = pCoinbar->GetHeight();
	destRect.width = pCoinbar->GetWidth();
	pCoinbar->Draw(destRect);

	//Coin numbers
	int amountOfCoins{ m_pPlayer->GetAmountOfCoins() };
	int hundreds{ amountOfCoins / 100 }, tens{ (amountOfCoins % 100) / 10 }, digit{(amountOfCoins % 10)};
	bottomLeft.x = m_Window.width - 3.f * margin - pCoinbar->GetWidth() / 3;

	pNumbers->SetFrame(hundreds);
	pNumbers->Draw(bottomLeft);
	bottomLeft.x += numberWidth;

	pNumbers->SetFrame(tens);
	pNumbers->Draw(bottomLeft);
	bottomLeft.x += numberWidth;

	pNumbers->SetFrame(digit);
	pNumbers->Draw(bottomLeft);


	//Instructions
	if (m_pPlayer->IsFrozen())
	{
		Texture* pInstruction{ Managers::GetInstance()->GetTextManager()->GetTexture(TextManager::Text::FrozenInstructions) };
		float topMargin{ 200.f }, padding{ 10.f };
		Rectf instructionRect{ m_Window.width / 2 - pInstruction->GetWidth()/2, m_Window.height - topMargin, pInstruction->GetWidth() + 2 * padding, pInstruction->GetHeight() + 2 * padding };
		glColor3f(1.f, 1.f, 1.f);
		utils::FillRect(instructionRect);
		glColor3f(0.f, 0.f, 0.f);
		utils::DrawRect(instructionRect);
		instructionRect.left += padding;
		instructionRect.bottom += padding;
		instructionRect.width -= 2*padding;
		instructionRect.height -= 2*padding;
		pInstruction->Draw(instructionRect);
	}
}

void Game::DrawLevel() const
{
	m_pLevel->Draw(m_Camera.GetTranslation(m_pPlayer->GetHitbox()));

	if (m_InRestArea) m_pMerchant->Draw();
}

void Game::DrawTitleScreen() const
{
	if (m_PlayerHasWon)
	{
		Managers::GetInstance()->GetTextureManager()->GetTexture(TextureManager::TextureType::EndScreen)->Draw(Rectf{ 0.f,0.f, m_Window.width, m_Window.height });
		return;
	}

	bool showCursor{ false };
	Point2f cursorBottomLeft{};
	switch (m_TitleScreenSelection)
	{
	case Game::TitleScreenSelection::TitleScreen:
		Managers::GetInstance()->GetTextureManager()->GetTexture(TextureManager::TextureType::TitleScreen)->Draw(Rectf{ 0.f,0.f,m_Window.width,m_Window.height });
		break;
	case Game::TitleScreenSelection::EraseData:
		showCursor = true;
		cursorBottomLeft = Point2f{m_Window.width / 6, m_Window.height / 2 - 50.f};
	case Game::TitleScreenSelection::Play:
		if (!showCursor)
		{
			showCursor = true;
			cursorBottomLeft = Point2f{ m_Window.width / 3, m_Window.height / 2 - 50.f };
		}
	case Game::TitleScreenSelection::Sound:
		glColor3f(255.f/255.f, 211.f/255.f, 8.f/255.f);
		utils::FillRect(Rectf{ 0.f,0.f,m_Window.width, m_Window.height });

		{
			Texture* pSaveFile{ Managers::GetInstance()->GetTextureManager()->GetTexture(TextureManager::TextureType::SaveFile) };
			Rectf saveFileRect{ m_Window.width / 3 - pSaveFile->GetWidth() / 2, m_Window.height / 2 - pSaveFile->GetHeight() / 2, pSaveFile->GetWidth(), pSaveFile->GetHeight() };
			pSaveFile->Draw(saveFileRect);
			Sprite* pNumbers{ Managers::GetInstance()->GetSpriteManager()->GetSprite(SpriteManager::SpriteType::Numbers) };
			int tens{ m_Level / 10 }, digit{ m_Level % 10 };
			Point2f bottomLeftNumber{saveFileRect.left + pSaveFile->GetWidth()/4, saveFileRect.bottom + pSaveFile->GetHeight() / 2};
			pNumbers->SetFrame(tens);
			pNumbers->Draw(bottomLeftNumber);
			bottomLeftNumber.x += pNumbers->GetFrameWidth();
			pNumbers->SetFrame(digit);
			pNumbers->Draw(bottomLeftNumber);

			Managers::GetInstance()->GetPixlManager()->DrawUnlockedPixls(Rectf{saveFileRect.left + 25.f, saveFileRect.bottom + saveFileRect.height/3, 0.f, 20.f});

			Managers::GetInstance()->GetSoundManager()
				->DrawSettingsMenu(Point2f{ 2 * m_Window.width / 3, 2 * m_Window.height / 3 }, true, m_TitleScreenSelection == TitleScreenSelection::Sound);

		}
		break;
	}

	if (showCursor) Managers::GetInstance()->GetTextureManager()->GetTexture(TextureManager::TextureType::Pointer)->Draw(cursorBottomLeft);
}

void Game::DrawMenus() const
{
	Point2f topLeft{ m_Window.width / 9, 4 * m_Window.height / 5 };
	Rectf descriptionRect{ m_Window.width / 5, 20.f, 2 * m_Window.width / 3, 50.f};
	
	Managers::GetInstance()->GetPixlManager()->DrawPixlsMenu(topLeft, m_InGameMenuSelection == InGameMenuSelection::Pixl, descriptionRect);
	topLeft.x += 2 * m_Window.width / 7;
	
	Managers::GetInstance()->GetItemManager()->DrawInventoryItems(topLeft, m_InGameMenuSelection == InGameMenuSelection::Inventory, descriptionRect);

	topLeft.x += 2 * m_Window.width / 7;
	Managers::GetInstance()->GetSoundManager()->DrawSettingsMenu(topLeft, m_InGameMenuSelection == InGameMenuSelection::Settings, m_InGameMenuSelection == InGameMenuSelection::Settings);
}

void Game::DrawShop() const
{
	if (!m_pMerchant || !m_pMerchant->IsPlayerInShop()) return;

	Point2f topLeft{ m_Window.width / 4, 4 * m_Window.height / 5 };
	Rectf descriptionRect{ m_Window.width / 5, 20.f, 2 * m_Window.width / 3, 50.f };

	Managers::GetInstance()->GetItemManager()->DrawInventoryItems(topLeft, true, Rectf{}, false);
	topLeft.x += m_Window.width / 3;
	m_pMerchant->DrawShop(topLeft, descriptionRect);
}

void Game::DoCollisionTests()
{
	Managers::GetInstance()->GetEnemyManager()->HitPlayer();
	Managers::GetInstance()->GetEnemyManager()->AttackAll(m_pPlayer->GetAttackHitbox());

	Managers::GetInstance()->GetProjectileManager()->HitSentients(m_pPlayer);
	if (Managers::GetInstance()->GetProjectileManager()->HitHearts(m_Hearts, m_pPlayer)) m_pBoss->Attacked();

	for (Heart* heart : m_Hearts)
	{
		if (heart->WasHit(m_pPlayer->GetAttackHitbox(), m_pPlayer->CanHitHeart() ? Heart::AttackType::Hammer : Heart::AttackType::Unaffected)) m_pBoss->Attacked();
	}

	if (Managers::GetInstance()->GetItemManager()->IsOverlapping(m_pPlayer))
	{
		m_PickUpRem = m_PickUpDuration;
		m_PickUpIsItem = true;
	}
}

void Game::NavigateTitleScreen(const SDL_KeyboardEvent& e)
{
	SoundManager* pSm{ Managers::GetInstance()->GetSoundManager() };
	switch (e.keysym.sym)
	{
	case SDLK_ESCAPE:
		m_TitleScreenSelection = TitleScreenSelection::TitleScreen;
		pSm->PlaySoundEffect(SoundManager::Soundfx::MenuConfirm);
		break;
	case SDLK_SPACE:
		switch (m_TitleScreenSelection)
		{
		case TitleScreenSelection::TitleScreen:
			m_TitleScreenSelection = TitleScreenSelection::Play;
			break;
		case TitleScreenSelection::Play:
			StartGame();
			break;
		case TitleScreenSelection::EraseData:
			remove(m_SaveFileName.c_str());
			delete m_pPlayer;
			m_Level = 0;
			Managers::GetInstance()->GetPixlManager()->Reset();
			Managers::GetInstance()->GetItemManager()->Reset();
			m_ContinuingFromSave = false;
			break;
		case TitleScreenSelection::Sound:
			Managers::GetInstance()->GetSoundManager()->Confirm();
			break;
		}
		pSm->PlaySoundEffect(SoundManager::Soundfx::MenuConfirm);
		break;
	case SDLK_a:
		if(!Managers::GetInstance()->GetSoundManager()->IsAdjustingSetting()) switch (m_TitleScreenSelection)
		{
		case Game::TitleScreenSelection::Play:
			m_TitleScreenSelection = TitleScreenSelection::EraseData;
			break;
		case Game::TitleScreenSelection::Sound:
			m_TitleScreenSelection = TitleScreenSelection::Play;
			break;
		}
		pSm->PlaySoundEffect(SoundManager::Soundfx::MenuChange);
		break;
	case SDLK_d:
		if (!Managers::GetInstance()->GetSoundManager()->IsAdjustingSetting()) switch (m_TitleScreenSelection)
		{
		case Game::TitleScreenSelection::EraseData:
			m_TitleScreenSelection = TitleScreenSelection::Play;
			break;
		case Game::TitleScreenSelection::Play:
			m_TitleScreenSelection = TitleScreenSelection::Sound;
			break;
		}
		pSm->PlaySoundEffect(SoundManager::Soundfx::MenuChange);
		break;
	case SDLK_w:
	case SDLK_s:
		{
			bool up{ e.keysym.sym == SDLK_w || e.keysym.sym == SDLK_UP };
			Managers::GetInstance()->GetSoundManager()->Scroll(up);
		}
		pSm->PlaySoundEffect(SoundManager::Soundfx::MenuChange);
		break;
	}

}

void Game::ProcessKeyDownEvent( const SDL_KeyboardEvent & e )
{
	//std::cout << "KEYDOWN event: " << e.keysym.sym << std::endl;
}

void Game::ProcessKeyUpEvent( const SDL_KeyboardEvent& e )
{
	//std::cout << "KEYUP event: " << e.keysym.sym << std::endl;
	SoundManager* pSm{ Managers::GetInstance()->GetSoundManager() };
	if (m_InTitleScreen)
	{
		NavigateTitleScreen(e);
		return;
	}

	if (e.keysym.sym == SDLK_ESCAPE)
	{
		if (m_pMerchant && m_pMerchant->IsPlayerInShop()) m_pMerchant->ExitShop();
		else m_IsPlayerInMenu = !m_IsPlayerInMenu;
		pSm->PlaySoundEffect(SoundManager::Soundfx::MenuConfirm);
	}

	if (m_IsPlayerInMenu)
	{
		switch (e.keysym.sym)
		{
		case SDLK_w:
		case SDLK_s:
			{
				bool up{e.keysym.sym == SDLK_w || e.keysym.sym == SDLK_UP};
				switch (m_InGameMenuSelection)
				{
				case Game::InGameMenuSelection::Pixl:
					Managers::GetInstance()->GetPixlManager()->Scroll(up);
					break;
				case Game::InGameMenuSelection::Inventory:
					Managers::GetInstance()->GetItemManager()->Scroll(up);
					break;
				case Game::InGameMenuSelection::Settings:
					Managers::GetInstance()->GetSoundManager()->Scroll(up);
					break;
				}
			}
			pSm->PlaySoundEffect(SoundManager::Soundfx::MenuChange);
			break;
		case SDLK_a:
		case SDLK_LEFT:
			if(!Managers::GetInstance()->GetSoundManager()->IsAdjustingSetting())
			{
				int newSelectionId{ int(m_InGameMenuSelection) - 1 };
				if (newSelectionId < 0) newSelectionId = int(InGameMenuSelection::Settings);
				m_InGameMenuSelection = InGameMenuSelection(newSelectionId);
			}
			pSm->PlaySoundEffect(SoundManager::Soundfx::MenuChange);
			break;
		case SDLK_d:
		case SDLK_RIGHT:
			if (!Managers::GetInstance()->GetSoundManager()->IsAdjustingSetting())
			{
				int newSelectionId{ int(m_InGameMenuSelection) + 1 };
				newSelectionId %= int(InGameMenuSelection::Settings) + 1;
				m_InGameMenuSelection = InGameMenuSelection(newSelectionId);
			}
			pSm->PlaySoundEffect(SoundManager::Soundfx::MenuChange);
			break;
		case SDLK_SPACE:
			switch (m_InGameMenuSelection)
			{
			case Game::InGameMenuSelection::Pixl:
				Managers::GetInstance()->GetPixlManager()->ActivateSelectedPixl(m_pPlayer);
				m_IsPlayerInMenu = false;
				break;
			case Game::InGameMenuSelection::Inventory:
				Managers::GetInstance()->GetItemManager()->UseActiveItem();
				m_IsPlayerInMenu = false;
				break;
			case Game::InGameMenuSelection::Settings:
				Managers::GetInstance()->GetSoundManager()->Confirm();
				break;
			}
			pSm->PlaySoundEffect(SoundManager::Soundfx::MenuConfirm);
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
		case SDLK_SPACE:
			if (m_InRestArea && m_pMerchant->IsPlayerInShop()) m_pMerchant->Buy(m_pPlayer);
			break;
		case SDLK_s:
			if (m_InRestArea && m_pMerchant->IsPlayerInShop()) m_pMerchant->Scroll(false);
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
				if (m_pMerchant->IsPlayerInShop()) m_pMerchant->Scroll(true);

				Rectf pipe{ m_pLevel->GetPipe() };
				pipe.width += 10.f;
				if (m_pPlayer->IsOverlapping(pipe))
				{
					SaveGame();
					GoBackToTitleScreen();
					break;
				}

				if (m_pMerchant->EnterShop(m_pPlayer->GetHitbox())) break;
				if (!m_pPlayer->IsOverlapping(m_pLevel->GetDoor())) break;
			}
			else
			{
				bool playerHasKey{ Managers::GetInstance()->GetItemManager()->InventoryHasType(Item::Type::Key) };
				if (!playerHasKey || !m_pPlayer->IsOverlapping(m_pLevel->GetDoor())) break;
			}

			AdvanceToNextLevel();
			break;
		case SDLK_o:
			//TODO: Put debug code here
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
	std::cout << "Go to the left: A" << '\n';
	std::cout << "Go to the right: D" << '\n';
	std::cout << "Use/Select/Confirm: SPACEBAR" << '\n';
	std::cout << "Close Menu: ESCAPE" << '\n';
}

void Game::AdvanceToNextLevel()
{
	Managers::GetInstance()->GetItemManager()->RemoveKey();
	Managers::GetInstance()->GetEnemyManager()->SetKeySpawned(false);
	Managers::GetInstance()->GetItemManager()->DestroyAllFloorItems();
	Managers::GetInstance()->GetEnemyManager()->KillAllEnemies();
	Managers::GetInstance()->GetProjectileManager()->DestroyAll();
	Managers::GetInstance()->GetParticleManager()->DestroyAll();

	if (m_Level == m_BossLevel)
	{
		WinGame();
		return;
	}

	if (m_pMerchant)
	{
		delete m_pMerchant;
		m_pMerchant = nullptr;
	}

	if (!m_ContinuingFromSave)
	{
		m_Level++;
		m_ChestOpened = false;
		Managers::GetInstance()->GetSpriteManager()->GetSprite(SpriteManager::SpriteType::Chest)->SetFrame(0);
	}
	else m_ContinuingFromSave = false;
	
	m_InRestArea = m_Level % m_LevelsPerRestArea == 0;
	if (!m_InRestArea)
	{
		delete m_pLevel;
		m_pLevel = new Level(Managers::GetInstance()->GetTextureManager()->GetRandomBackground(), false, m_Level == m_BossLevel);
		SpawnEnemies();
		m_pPlayer->SetPosition(Point2f{10.f, 50.f});
		if (m_Level == m_BossLevel)
		{
			Managers::GetInstance()->GetEnemyManager()->SetKeySpawned(true);
			Managers::GetInstance()->GetSoundManager()->PlayBackgroundMusic(SoundManager::Song::BossFight);
		}
	}
	else
	{
		delete m_pLevel;
		m_pLevel = new Level(Managers::GetInstance()->GetTextureManager()->GetTexture(TextureManager::TextureType::RestArea), true, false);
		Sprite* pMerchant{ Managers::GetInstance()->GetSpriteManager()->GetSprite(SpriteManager::SpriteType::Flamm) };
		m_pMerchant = new Merchant(Point2f{ m_pLevel->GetBoundaries().width - pMerchant->GetFrameWidth() - 20.f, m_pLevel->GetBoundaries().bottom + 20.f }, pMerchant, m_pPlayer);
		m_pPlayer->SetPosition(Point2f{ 250.f, 50.f });
	}

	m_Camera.SetLevelBoundaries(m_pLevel->GetBoundaries());
}

void Game::SpawnEnemies()
{
	if (m_Level == m_BossLevel)
	{
		float margin{ 50.f };
		Rectf containmentfield{ m_pLevel->GetBoundaries() };
		containmentfield.left += margin;
		containmentfield.bottom += margin;
		containmentfield.width -= 2 * margin;
		containmentfield.height -= 2 * margin;

		m_pBoss = new Dimentio(Point2f{ m_Window.width / 2,m_Window.height / 2 }, m_pPlayer, containmentfield);
		Point2f bottomLeft{ m_pLevel->GetBoundaries().width / 4, 25.f };
		for (size_t i = 0; i < m_AmountOfHearts; i++)
		{
			m_Hearts.push_back(new Heart(bottomLeft));
			bottomLeft.x += m_pLevel->GetBoundaries().width / 4;
		}
		return;
	}

	EnemyManager* em{ Managers::GetInstance()->GetEnemyManager() };

	Rectf spawnBox{ m_pLevel->GetBoundaries() };
	spawnBox.bottom += 20.f;
	spawnBox.height -= 100.f;

	switch (m_Level)
	{
	case 1:
		em->Spawn(Enemy::Type::Goomba, 2, spawnBox);
		em->Spawn(Enemy::Type::Spiny, 1, spawnBox);
		em->Spawn(Enemy::Type::Cherbil, 1, spawnBox);
		break;
	case 2:
		em->Spawn(Enemy::Type::Squiglet, 1, spawnBox);
		em->Spawn(Enemy::Type::Goomba, 2, spawnBox);
		em->Spawn(Enemy::Type::Spiny, 1, spawnBox);
		break;
	case 4:
		em->Spawn(Enemy::Type::Squiglet, 2, spawnBox);
		em->Spawn(Enemy::Type::Goomba, 2, spawnBox);
		break;
	default:
		em->Spawn(Enemy::Type::Goomba, 1, spawnBox);
	}
}

void Game::StartGame()
{
	m_InTitleScreen = false;
	Managers::GetInstance()->GetSoundManager()->PlayBackgroundMusic(SoundManager::Song::MainTheme);

	if(!m_pPlayer) InitPlayer();

	AdvanceToNextLevel();

	Managers::GetInstance()->GetItemManager()->Spawn(Item::Type::IceStorm, Point2f{200.f, 10.f}, m_pPlayer);

	NotifyCameraLevelChange();
}

void Game::GameOver()
{
	m_Level = 0;
	Managers::GetInstance()->GetPixlManager()->Reset();
	Managers::GetInstance()->GetItemManager()->Reset();
	GoBackToTitleScreen();
}

void Game::WinGame()
{
	m_PlayerHasWon = true;
	GoBackToTitleScreen();
	Managers::GetInstance()->GetSoundManager()->PlayBackgroundMusic(SoundManager::Song::EndScreen);
}

void Game::GoBackToTitleScreen()
{
	m_InTitleScreen = true;
	Managers::GetInstance()->GetSoundManager()->PlayBackgroundMusic(SoundManager::Song::TitleScreen);

	delete m_pPlayer;
	m_pPlayer = nullptr;
	Managers::GetInstance()->GetSpriteManager()->GetSprite(SpriteManager::SpriteType::MarioDeath)->SetFrame(0);
	m_ContinuingFromSave = true;

}

void Game::SaveGame() const
{
	std::ofstream os{ m_SaveFileName };
	os << m_Level << '\n';
	os << m_pPlayer->GetCurrentHealth() << '\n';
	os << m_pPlayer->GetMaxHealth() << '\n';
	os << m_pPlayer->GetAmountOfCoins() << '\n';
	os << Managers::GetInstance()->GetPixlManager()->ToSaveFormat() << '\n';
	os << Managers::GetInstance()->GetItemManager()->ToSaveFormat();
}

void Game::LoadGame()
{
	if(!m_pPlayer) InitPlayer();

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
	//Coins
	std::getline(is, line);
	int coins = std::stoi(line);
	m_pPlayer->AdjustAmountOfCoins(coins);
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
