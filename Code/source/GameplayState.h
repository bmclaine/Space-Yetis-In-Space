//*********************************************************************//
//	File:		GameplayState.h
//	Author:		Brian McLaine
//	Course:		SGD
//	Purpose:	GameplayState class initializes & runs the game logic
//*********************************************************************//

#pragma once

#include "IGameState.h"							// uses IGameState
#include "../SGD Wrappers/SGD_Handle.h"			// uses HTexture & HAudio
#include "../SGD Wrappers/SGD_Declarations.h"	// uses Message


//*********************************************************************//
// Forward class declaration
//	- tells the compiler that the type exists
//	- can make pointers or references to the type
//	- MUST include their headers in the .cpp to dereference
class Entity;
class EntityManager;
class AIShip;


//*********************************************************************//
// GameplayState class
//	- runs the game logic
//	- SINGLETON! (Static allocation, not dynamic)
class GameplayState : public IGameState
{
public:
	//*****************************************************************//
	// Singleton Accessor:
	static GameplayState* GetInstance(void);


	//*****************************************************************//
	// IGameState Interface:
	virtual void	Enter(void)				override;	// load resources / reset data
	virtual void	Exit(void)				override;	// unload resources

	virtual bool	Update(float elapsedTime)	override;	// handle input & update game entities
	virtual void	Render(float elapsedTime)	override;	// render game entities / menus

	//*****************************************************************//
	// Accessors:
	Entity* GetPlayer(void) { return m_pShip; }
	bool GetGameOver(void) { return m_bGameOver; }
	bool GetGameWon(void) { return m_bGameWon; }
	bool GetIsPaused(void) { return m_bIsPaused; }
	// Mutators:
	void SetGameOver(bool gameOver) { m_bGameOver = gameOver; }
	void SetGameWon(bool gameWon) { m_bGameWon = gameWon; }
	void SetIsPaused(bool isPaused) { m_bIsPaused = isPaused; }


private:
	//*****************************************************************//
	// SINGLETON (not-dynamically allocated)
	GameplayState(void) = default;	// default constructor
	virtual ~GameplayState(void) = default;	// destructor

	GameplayState(const GameplayState&) = delete;	// copy constructor
	GameplayState& operator= (const GameplayState&) = delete;	// assignment operator

	//*****************************************************************//
	// Members
	float m_fSpawnTimer = 5.0f;
	int m_nCursor = 0;
	unsigned int m_unSpawnedEnemies = 0;
	unsigned int m_unDefeatedEnemies = 0;
	bool m_bGameWon = false;
	bool m_bGameOver = false;
	bool m_bIsPaused = false;


	//*****************************************************************//
	// Assets
	SGD::HTexture m_hShipImg = SGD::INVALID_HANDLE; // Player Ship Image
	SGD::HTexture m_hAIShipImg = SGD::INVALID_HANDLE; // AI Ship Image
	SGD::HTexture m_hBossShipImg = SGD::INVALID_HANDLE; // Boss Ship Image
	SGD::HTexture m_hPSLaserImg = SGD::INVALID_HANDLE; // Player Ship Laser Image
	SGD::HTexture m_hESLaserImg = SGD::INVALID_HANDLE; // Enemy Ship Laser Image

	SGD::HAudio m_hBackgroundMus = SGD::INVALID_HANDLE; // Background Music

	SGD::HAudio m_hVictorySfx = SGD::INVALID_HANDLE; // Victory Sound effect
	SGD::HAudio m_hGameOverSfx = SGD::INVALID_HANDLE; // GameOver sound effect
	SGD::HAudio m_hPlayerLaserShotSfx = SGD::INVALID_HANDLE; // Player laser shot sound effect
	SGD::HAudio m_hEnemyLaserShotSfx = SGD::INVALID_HANDLE; // Enemy laser shot sound effect
	SGD::HAudio m_hShipCollisionSfx = SGD::INVALID_HANDLE; // Ship to Ship Collision sound effect
	SGD::HAudio m_hLaserCollisionSfx = SGD::INVALID_HANDLE; // Laser Collision sound effect

	//*****************************************************************//
	// Game Entities
	EntityManager*	m_pEntities = nullptr;
	Entity*	m_pShip = nullptr;
	Entity* m_pBossShip = nullptr;
	Entity* m_pMoonA = nullptr;
	Entity* m_pMoonB = nullptr;

	//*****************************************************************//
	// Factory Methods
	Entity* CreateShip(void) const;
	Entity* CreateAIShip(void) const;
	Entity* CreateBossShip(void) const;
	Entity* CreateLaser(float posX, float posY, float rotation, bool isPlayerShipLaser) const;
	Entity* CreateMoonA(void) const;
	Entity* CreateMoonB(void) const;
	SGD::Point RandomPosition(AIShip* aiShip) const;


	//*****************************************************************//
	// Message Callback Procedure
	static void MessageProc(const SGD::Message* pMsg);

};
