//*********************************************************************//
//	File:		GameplayState.cpp
//	Author:		Brian McLaine
//	Course:		SGD
//	Purpose:	GameplayState class initializes & runs the game logic
//*********************************************************************//

#include "GameplayState.h"

#include "Game.h"
#include "MainMenuState.h"
#include "CreditsState.h"
#include "TallyState.h"
#include "BitmapFont.h"

#include "../SGD Wrappers/SGD_AudioManager.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_InputManager.h"
#include "../SGD Wrappers/SGD_String.h"
#include "../SGD Wrappers/SGD_Utilities.h"

#include "../SGD Wrappers/SGD_EventManager.h"
#include "../SGD Wrappers/SGD_Event.h"

#include "../SGD Wrappers/SGD_MessageManager.h"
#include "../SGD Wrappers/SGD_Message.h"
#include "MessageID.h"
#include "DestroyEntityMessage.h"
#include "CreateLaserMessage.h"

#include "EntityManager.h"
#include "Entity.h"
#include "Ship.h"
#include "AIShip.h"
#include "BossShip.h"
#include "Moon.h"

#include "Laser.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <cstdlib>
#include <cassert>
#include <fstream>

#define NUM_ENEMIES 4
#define WAVE_ONE 1
#define WAVE_BOSS 3

//***********************************************************************
// Entity Manager Buckets
enum EntityBucket { BUCKET_PLAYER_SHIP, BUCKET_ENEMY_SHIP, BUCKET_PLAYER_SHIP_LASERS, BUCKET_ENEMY_SHIP_LASERS, BUCKET_MOON };

//*********************************************************************//
// GetInstance
//	- allocate static global instance
//	- return THE instance
/*static*/ GameplayState* GameplayState::GetInstance(void)
{
	static GameplayState s_Instance;	// stored in global memory once
	return &s_Instance;
}


//*********************************************************************//
// Enter
//	- reset game
//	- load resources / assets
//	- allocate & initialize game entities
/*virtual*/ void GameplayState::Enter(void)	/*override*/
{
	// Initialize the Event & Message Managers
	SGD::EventManager::GetInstance()->Initialize();
	SGD::MessageManager::GetInstance()->Initialize(&GameplayState::MessageProc);

	// Load Assets
	SGD::GraphicsManager* pGraphics = SGD::GraphicsManager::GetInstance();
	SGD::AudioManager* pAudio = SGD::AudioManager::GetInstance();

	m_hShipImg = pGraphics->LoadTexture(L"./resource/graphics/BM_PlayerShip.png", { 255, 255, 255, 255 });
	m_hAIShipImg = pGraphics->LoadTexture(L"./resource/graphics/BM_EnemyShip.png", { 255, 255, 255, 255 });
	m_hBossShipImg = pGraphics->LoadTexture(L"./resource/graphics/BM_BossShip.png", { 255, 255, 255, 255 });
	m_hPSLaserImg = pGraphics->LoadTexture(L"./resource/graphics/BM_GreenLaser.png");
	m_hESLaserImg = pGraphics->LoadTexture(L"./resource/graphics/BM_RedLaser.png");

	m_hBackgroundMus = pAudio->LoadAudio(L"./resource/audio/BM_BackgroundLoop.xwm");

	m_hPlayerLaserShotSfx = pAudio->LoadAudio(L"./resource/audio/BM_PlayerLaser.wav");
	m_hEnemyLaserShotSfx = pAudio->LoadAudio(L"./resource/audio/BM_EnemyLaser.wav");
	m_hLaserCollisionSfx = pAudio->LoadAudio(L"./resource/audio/BM_LaserCollision.wav");
	m_hShipCollisionSfx = pAudio->LoadAudio(L"./resource/audio/BM_ShipExplosion.wav");
	m_hVictorySfx = pAudio->LoadAudio(L"./resource/audio/BM_VictorySfx.wav");
	m_hGameOverSfx = pAudio->LoadAudio(L"./resource/audio/BM_GameOverSfx.wav");

	//// Set volumes
	//pAudio->SetMasterVolume(SGD::AudioGroup::Music, 30); // ALL music (xwm) are at 30% volume
	//pAudio->SetMasterVolume(SGD::AudioGroup::SoundEffects, 50); // All sound effects are at 50% volume

	// Play the background music
	pAudio->PlayAudio(m_hBackgroundMus, true);

	// Allocate the Entity Manager
	m_pEntities = new EntityManager;

	// Initialize the Player Ship and store it into the entity manager
	m_pShip = CreateShip();
	m_pEntities->AddEntity(m_pShip, BUCKET_PLAYER_SHIP);
}


//*********************************************************************//
// Exit
//	- deallocate entities
//	- unload resources / assets
/*virtual*/ void GameplayState::Exit(void)	/*override*/
{
	SGD::Event* pEvent = new SGD::Event("GAME_OVER");
	pEvent->SendEventNow();
	delete pEvent;
	pEvent = nullptr;

	// Release game entities
	if (m_pShip != nullptr)
	{
		m_pShip->Release();
		m_pShip = nullptr;
	}
	if (m_pMoonA != nullptr)
	{
		m_pMoonA->Release();
		m_pMoonA = nullptr;
	}
	if (m_pMoonB != nullptr)
	{
		m_pMoonB->Release();
		m_pMoonB = nullptr;
	}	
	if (m_pBossShip != nullptr)
	{
		m_pBossShip->Release();
		m_pBossShip = nullptr;
	}

	if (m_pEntities != nullptr)
	{
		m_pEntities->RemoveAll();
		delete m_pEntities;
		m_pEntities = nullptr;
	}

	//Unload all Assets
	SGD::GraphicsManager* pGraphics = SGD::GraphicsManager::GetInstance();
	SGD::AudioManager* pAudio = SGD::AudioManager::GetInstance();

	pGraphics->UnloadTexture(m_hShipImg);
	pGraphics->UnloadTexture(m_hAIShipImg);
	pGraphics->UnloadTexture(m_hBossShipImg);
	pGraphics->UnloadTexture(m_hPSLaserImg);
	pGraphics->UnloadTexture(m_hESLaserImg);

	pAudio->UnloadAudio(m_hBackgroundMus);
	pAudio->UnloadAudio(m_hPlayerLaserShotSfx);
	pAudio->UnloadAudio(m_hEnemyLaserShotSfx);
	pAudio->UnloadAudio(m_hLaserCollisionSfx);
	pAudio->UnloadAudio(m_hShipCollisionSfx);
	pAudio->UnloadAudio(m_hVictorySfx);
	pAudio->UnloadAudio(m_hGameOverSfx);

	// Terminate & deallocate the SGD wrappers
	SGD::MessageManager::GetInstance()->Terminate();
	SGD::MessageManager::DeleteInstance();

	SGD::EventManager::GetInstance()->Terminate();
	SGD::EventManager::DeleteInstance();

	// Reset Members
	m_fSpawnTimer = 5.0f;
	m_nCursor = 0;
	m_unSpawnedEnemies = 0;
	m_unDefeatedEnemies = 0;
	m_bGameWon = false;
	m_bGameOver = false;
	m_bIsPaused = false;
}


//*********************************************************************//
// Update
//	- handle input & update entities
/*virtual*/ bool GameplayState::Update(float elapsedTime)	/*override*/
{
	SGD::InputManager* pInput = SGD::InputManager::GetInstance();

	// Press Escape to return to MainMenuState
	if (pInput->IsKeyPressed(SGD::Key::Escape) == true)
	{
		if (m_bIsPaused == false)
			m_bIsPaused = true;
		else
			m_bIsPaused = false;
	}

	if (m_bIsPaused == false)
	{
		// Spawn Wave 1
		if (m_unSpawnedEnemies < WAVE_ONE)
		{
			m_fSpawnTimer -= elapsedTime;

			if (m_fSpawnTimer <= 0.0f)
			{
				m_fSpawnTimer = 3.0f;

				Entity* pAIShip = CreateAIShip();
				m_pEntities->AddEntity(pAIShip, BUCKET_ENEMY_SHIP);
				m_unSpawnedEnemies++;
				pAIShip->Release();
			}
		}

		if (m_unSpawnedEnemies >= WAVE_ONE && m_unDefeatedEnemies >= WAVE_ONE)
		{
			if (m_unSpawnedEnemies < NUM_ENEMIES)
			{
				m_pMoonA = CreateMoonA();
				m_pEntities->AddEntity(m_pMoonA, BUCKET_MOON);
				m_unSpawnedEnemies++;
				m_pMoonB = CreateMoonB();
				m_pEntities->AddEntity(m_pMoonB, BUCKET_MOON);
				m_unSpawnedEnemies++;
				m_pBossShip = CreateBossShip();
				m_pEntities->AddEntity(m_pBossShip, BUCKET_ENEMY_SHIP);
				m_unSpawnedEnemies++;
			}
		}

		if (m_unSpawnedEnemies == NUM_ENEMIES && m_unDefeatedEnemies == NUM_ENEMIES && m_bGameWon == false)
		{
			m_bGameWon = true;
			SGD::AudioManager::GetInstance()->PlayAudio(m_hVictorySfx);
		}

		// if the game is won and enter is hit, return to the mainmenu
		if (pInput->IsKeyPressed(SGD::Key::Enter) == true && pInput->IsKeyDown(SGD::Key::Alt) == false)
		{
			if ((m_bGameWon == true || m_bGameOver == true))
			{
				Ship* pShip = reinterpret_cast<Ship*>(m_pShip);

				int score = pShip->GetScore();
				float time = pShip->GetTimePlayed();

				std::ofstream fout;

				fout.open("stats.txt", std::ios_base::trunc);

				if (fout.is_open())
				{
					fout << score << '\t' << time;

					fout.close();
				}

				Game::GetInstance()->ChangeState(TallyState::GetInstance());

				return true;
			}
		}

		// Update the entities
		m_pEntities->UpdateAll(elapsedTime);

		m_pEntities->CheckCollisions(BUCKET_PLAYER_SHIP_LASERS, BUCKET_ENEMY_SHIP_LASERS); // Player lasers vs Enemy Lasers
		m_pEntities->CheckCollisions(BUCKET_PLAYER_SHIP_LASERS, BUCKET_ENEMY_SHIP); // Player lasers vs Enemy Ship
		m_pEntities->CheckCollisions(BUCKET_PLAYER_SHIP_LASERS, BUCKET_MOON); // Player lasers vs Enemy Ship
		m_pEntities->CheckCollisions(BUCKET_ENEMY_SHIP_LASERS, BUCKET_PLAYER_SHIP); // Enemy lasers vs Player Ship
		m_pEntities->CheckCollisions(BUCKET_PLAYER_SHIP, BUCKET_ENEMY_SHIP); // Player ship vs Enemy Ship
		m_pEntities->CheckCollisions(BUCKET_PLAYER_SHIP, BUCKET_MOON); // Player ship vs Enemy Ship

		// Process the Event Manager
		//	- all the events will be sent to the registered IListeners' HandleEvent methods
		SGD::EventManager::GetInstance()->Update();

		// Process the Message Manager
		//	- all the messages will be sent to our MessageProc
		SGD::MessageManager::GetInstance()->Update();

		return true;	// keep playing
	}
	else
	{
		if (pInput->IsKeyPressed(SGD::Key::Enter) == true && pInput->IsKeyDown(SGD::Key::Alt) == false)
		{
			if (m_nCursor == 0)
				m_bIsPaused = false;
			else
			{
				Game::GetInstance()->ChangeState(MainMenuState::GetInstance());
				return true;
			}
		}

		if (pInput->IsKeyPressed(SGD::Key::UpArrow) == true || pInput->IsKeyPressed(SGD::Key::W) == true)
		{
			m_nCursor--;
			if (m_nCursor < 0)
				m_nCursor = 1;
		}
		else if (pInput->IsKeyPressed(SGD::Key::DownArrow) == true || pInput->IsKeyPressed(SGD::Key::S) == true)
		{
			m_nCursor++;
			if (m_nCursor > 1)
				m_nCursor = 0;
		}
	}
	return true;
}


//*********************************************************************//
// Render
//	- render the game entities
/*virtual*/ void GameplayState::Render(float elapsedTime)	/*override*/
{
	BitmapFont* pFont = Game::GetInstance()->GetFont();
	SGD::Size screenSize = Game::GetInstance()->GetScreenSize();

		if (m_bGameWon == true)
		{
			pFont->Draw("YOU WIN!!", { (screenSize.width / 2) - (5 * 32), screenSize.height / 2 }, 1.0f, { 255, 127, 127, 255 });
			pFont->Draw("PRESS ENTER", { (screenSize.width / 2) - (5.5f * 32), (screenSize.height / 2) + 32 }, 1.0f, { 255, 255, 255, 255 });
		}

		SGD::OStringStream enemies;
		enemies << NUM_ENEMIES - m_unDefeatedEnemies;

		pFont->Draw("ENEMIES LEFT:    OUT OF 15", { screenSize.width / 2 - (7 * 32), 32 }, 0.5f, { 255, 255, 255, 255 });
		pFont->Draw(enemies.str().c_str(), { screenSize.width / 2, 32 }, 0.5f, { 255, 255, 255, 255 });

		// Render the entities
		m_pEntities->RenderAll();

		if (m_bIsPaused == true)
		{
			SGD::Rectangle pauseMenu = { SGD::Point((screenSize.width / 2) - 80, (screenSize.height / 2) - 30), SGD::Size(160, 100) };

			SGD::GraphicsManager::GetInstance()->DrawRectangle(pauseMenu, { 130, 0, 0, 0 }, { 160, 255, 0, 0 }, 2);

			pFont->Draw("Play", { (screenSize.width / 2) - 32, (screenSize.height / 2) }, 0.5f, { 255, 255, 255, 255 });
			pFont->Draw("Exit", { (screenSize.width / 2) - 32, (screenSize.height / 2) + 32 }, 0.5f, { 255, 255, 255, 255 });
			pFont->Draw("=", { (screenSize.width / 2) - 56, (screenSize.height / 2) + (32 * m_nCursor) }, 0.5f, { 255, 0, 255, 0 });
		}
}

//*********************************************************************//
// CreateShip
//	- Create the ship entity
Entity* GameplayState::CreateShip(void) const
{
	Ship* pShip = new Ship();
	pShip->SetImage(m_hShipImg);
	pShip->SetShipExplosionSfx(m_hShipCollisionSfx);
	pShip->SetGameOverSfx(m_hGameOverSfx);
	pShip->SetSize(SGD::Size(64, 64));

	SGD::Size screenSize = Game::GetInstance()->GetScreenSize();
	pShip->SetPosition(SGD::Point((screenSize.width - 64) / 2, (screenSize.height - 64) / 1.5f));

	return pShip;
}

Entity* GameplayState::CreateAIShip(void) const
{
	AIShip* pAIShip = new AIShip();
	pAIShip->SetImage(m_hAIShipImg);
	pAIShip->SetShipExplosionSfx(m_hShipCollisionSfx);
	pAIShip->SetSize({ 64, 64 });
	pAIShip->SetHealth(500);
	
	SGD::Point point = RandomPosition(pAIShip);

	while (point.IsPointInRectangle(m_pShip->GetRect()))
		point = RandomPosition(pAIShip);

	pAIShip->SetPosition(point);

	return pAIShip;
}

Entity* GameplayState::CreateBossShip(void) const
{
	BossShip* pBossShip = new BossShip();

	pBossShip->SetHealth(2000);
	pBossShip->SetImage(m_hBossShipImg);

	SGD::Size screenSize = Game::GetInstance()->GetScreenSize();
	pBossShip->SetPosition({ (screenSize.width / 2) - 64, -128 });
	pBossShip->SetShipExplosionSfx(m_hShipCollisionSfx);
	pBossShip->SetSize({ 128, 128 });
	pBossShip->SetRotation(SGD::PI);
	pBossShip->SetMoonA(m_pMoonA);
	pBossShip->SetMoonB(m_pMoonB);

	return pBossShip;
}

Entity* GameplayState::CreateLaser(float posX, float posY, float rotation, bool isPlayerShipLaser) const
{
	Laser* pLaser = new Laser();

	if (isPlayerShipLaser)
		pLaser->SetImage(m_hPSLaserImg);
	else
		pLaser->SetImage(m_hESLaserImg);

	pLaser->SetLaserCollideSfx(m_hLaserCollisionSfx);

	pLaser->SetSize({ 4, 8 });
	pLaser->SetPosition({ posX - 4 / 2, posY - 8 / 2 });

	SGD::Vector velocity = { 0, -1 };
	velocity.Rotate(rotation);
	velocity *= 400;

	pLaser->SetVelocity(velocity);
	pLaser->SetRotation(rotation);

	return pLaser;
}

Entity* GameplayState::CreateMoonA(void) const
{
	Moon* pMoon = new Moon();

	pMoon->SetPosition({ 64, -128 });
	pMoon->SetSize({ 64, 64 });
	pMoon->SetRotation(SGD::PI);
	pMoon->SetIsMoonA(true);
	pMoon->SetMoonExplosionSfx(m_hShipCollisionSfx);

	return pMoon;
}

Entity* GameplayState::CreateMoonB(void) const
{
	Moon* pMoon = new Moon();

	SGD::Size screenSize = Game::GetInstance()->GetScreenSize();

	pMoon->SetPosition({ screenSize.width - 128, -128 });
	pMoon->SetSize({ 64, 64 });
	pMoon->SetRotation(SGD::PI);
	pMoon->SetIsMoonA(false);
	pMoon->SetMoonExplosionSfx(m_hShipCollisionSfx);

	return pMoon;
}

SGD::Point GameplayState::RandomPosition(AIShip* aiShip) const
{
	SGD::Size screenSize = Game::GetInstance()->GetScreenSize();
	SGD::Point point =
	{
		float(rand() % int(screenSize.width - aiShip->GetSize().width) + int(aiShip->GetSize().width * 0.5f)),
		float(rand() % int(screenSize.height - aiShip->GetSize().height) + int(aiShip->GetSize().height * 0.5f))
	};
	return point;
}


//*********************************************************************//
// MessageProc
//	- process messages queued in the MessageManager
//	- STATIC METHOD!!!
//		- no invoking object!
//		- MUST USE THE SINGLETON
/*static*/ void GameplayState::MessageProc(const SGD::Message* pMsg)
{
	/* Show warning when a Message ID enumerator is not handled */
#pragma warning( push )
#pragma warning( 1 : 4061 )

	// What type of message?
	switch (pMsg->GetMessageID())
	{
	case MessageID::MSG_DESTROY_ENTITY:
	{
		// Downcast the message to the actual child type
		const DestroyEntityMessage* pDestroyMsg = dynamic_cast<const DestroyEntityMessage*>(pMsg);

		SGD_ASSERT(pDestroyMsg != nullptr,
			"GameplayState::MessageProc - MSG_DESTROY_ENTITY is not a DestroyEntityMessage!");

		// Access the entity attached to the child message
		Entity* pEntity = pDestroyMsg->GetEntity();

		// if the entity being removed is ENT_ENEMY_SHIP
		if (pEntity->GetType() == 2 || pEntity->GetType() == 4)
			GameplayState::GetInstance()->m_unDefeatedEnemies++;

		// Use the singleton to access members!
		GameplayState::GetInstance()->m_pEntities->RemoveEntity(pEntity);
	}
	break;
	case MessageID::MSG_CREATE_LASER:
	{
		// Downcast the message to the actual child type
		const CreateLaserMessage* pCreateMsg = dynamic_cast<const CreateLaserMessage*>(pMsg);

		SGD_ASSERT(pCreateMsg != nullptr,
			"GameplayState::MessageProc - MSG_CREATE_LASER is not actually a CreateLaserMessage");

		// Access our own singleton
		GameplayState* self = GameplayState::GetInstance();

		// Play sfx
		if (pCreateMsg->IsPlayerShipLaser() == true)
			SGD::AudioManager::GetInstance()->PlayAudio(self->m_hPlayerLaserShotSfx);
		else
			SGD::AudioManager::GetInstance()->PlayAudio(self->m_hEnemyLaserShotSfx);

		// Create a new laser entity using the message attributes
		Entity* pLaser = self->CreateLaser(pCreateMsg->GetPosX(), pCreateMsg->GetPosY(), pCreateMsg->GetRotation(), pCreateMsg->IsPlayerShipLaser());

		// Add the entity to the Entity Manager
		if (pCreateMsg->IsPlayerShipLaser() == true)
			self->m_pEntities->AddEntity(pLaser, BUCKET_PLAYER_SHIP_LASERS);
		else
			self->m_pEntities->AddEntity(pLaser, BUCKET_ENEMY_SHIP_LASERS);

		// Release the local pointer
		pLaser->Release();
		pLaser = nullptr;
	}
	break;

	default:
	case MessageID::MSG_UNKNOWN:
		SGD_PRINT(L"GameplayState::MessageProc - unknown message id\n");
		break;
	}


	/* Restore previous warning levels */
#pragma warning( pop )
}
