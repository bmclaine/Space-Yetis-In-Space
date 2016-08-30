//*********************************************************************//
//	File:		InstructionsState.cpp
//	Author:		Brian McLaine
//	Course:		SGD
//	Purpose:	InstructionsState class handles the game instructions
//*********************************************************************//

#include "InstructionsState.h"
#include "MainMenuState.h"
#include "BitmapFont.h"
#include "Game.h"

#include "../SGD Wrappers/SGD_InputManager.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"

//*********************************************************************//
// GetInstance
//	- create & return THE singleton object
/*static*/ InstructionsState* InstructionsState::GetInstance(void)
{
	static InstructionsState s_Instance;

	return &s_Instance;
}

//*********************************************************************//
// Enter
//	- called EACH time the screen is shown/switched to
//	- load resources / reset data
/*virtual*/ void InstructionsState::Enter(void)		/*override*/
{
	m_hEnemyImg = SGD::GraphicsManager::GetInstance()->LoadTexture(L"./resource/graphics/BM_EnemyShip.png", { 255, 255, 255, 255 });
	m_hPlayerImg = SGD::GraphicsManager::GetInstance()->LoadTexture(L"./resource/graphics/BM_PlayerShip.png", { 255, 255, 255, 255 });
}

//*********************************************************************//
// Exit
//	- called EACH time the screen is hidden/switched from
//	- unload resources
/*virtual*/ void InstructionsState::Exit(void)		/*override*/
{
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_hEnemyImg);
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_hPlayerImg);
	m_nCurrSlide = 0;
}


//*********************************************************************//
// Update
//	- called EVERY FRAME
//	- handle input & update
/*virtual*/ bool InstructionsState::Update(float elapsedTime)	/*override*/
{
	SGD::InputManager* pInput = SGD::InputManager::GetInstance();

	if (pInput->IsKeyPressed(SGD::Key::Escape) == true)
		Game::GetInstance()->ChangeState(MainMenuState::GetInstance());

	if (pInput->IsKeyPressed(SGD::Key::LeftArrow) == true || pInput->IsKeyPressed(SGD::Key::A) == true)
	{
		m_nCurrSlide--;
		if (m_nCurrSlide < 0)
		{
			m_nCurrSlide = 2;
		}
	}
	else if (pInput->IsKeyPressed(SGD::Key::RightArrow) == true || pInput->IsKeyPressed(SGD::Key::D) == true)
	{
		m_nCurrSlide++;
		if (m_nCurrSlide > 2)
		{
			m_nCurrSlide = 0;
		}
	}

	return true;
}

//*********************************************************************//
// Render
//	- called EVERY FRAME
//	- draw menus / entities
/*virtual*/ void InstructionsState::Render(float elapsedTime)		/*override*/
{
	BitmapFont* pFont = Game::GetInstance()->GetFont();
	SGD::Size screenSize = Game::GetInstance()->GetScreenSize();

	switch (m_nCurrSlide)
	{
	case 0:
	{
		pFont->Draw("Welcome to Space Yetis", { screenSize.width / 2 - (8 * 32), 100 }, 0.75f, { 255, 255, 255, 255 });
		pFont->Draw("Your mission is destroy all of the enemy space yetis.", { screenSize.width / 2 - (13 * 32), 200 }, 0.5f, { 255, 255, 255, 255 });
		pFont->Draw("Enemy yetis will spawn every few seconds, quickly", { screenSize.width / 2 - (13 * 32), 248 }, 0.5f, { 255, 255, 255, 255 });
		pFont->Draw("overwhelming the player. You'll have to defeat them", { screenSize.width / 2 - (13 * 32), 296 }, 0.5f, { 255, 255, 255, 255 });
		pFont->Draw("all before they can manage to defeat you. The game", { screenSize.width / 2 - (13 * 32), 344 }, 0.5f, { 255, 255, 255, 255 });
		pFont->Draw("is lost when the players health drops to zero and", { screenSize.width / 2 - (13 * 32), 392 }, 0.5f, { 255, 255, 255, 255 });
		pFont->Draw("all lives have been lost. The player win when all", { screenSize.width / 2 - (13 * 32), 440 }, 0.5f, { 255, 255, 255, 255 });
		pFont->Draw("enemy space yetis have been eliminated from te sky.", { screenSize.width / 2 - (13 * 32), 488 }, 0.5f, { 255, 255, 255, 255 });
		pFont->Draw("Good luck, have fun and for earths sake, don't die!", { screenSize.width / 2 - (13 * 32), 536 }, 0.5f, { 255, 255, 255, 255 });
	}
	break;
	case 1:
	{
		pFont->Draw("Player Ship", { screenSize.width / 2 - (4.2f * 32), 100 }, 0.75f, { 255, 255, 255, 255 });
		SGD::GraphicsManager::GetInstance()->DrawTexture(m_hPlayerImg, { (screenSize.width / 2) - 32, 200 }, SGD::PI, { 64 / 2, 64 / 2 });
		pFont->Draw("Move Forward using the W or Up Arrow key.", { screenSize.width / 2 - (11 * 32), 300 }, 0.5f, { 255, 255, 255, 255 });
		pFont->Draw("Move Backward using the S or Down Arrow key.", { screenSize.width / 2 - (11 * 32), 332 }, 0.5f, { 255, 255, 255, 255 });
		pFont->Draw("Rotate Left using the A or Left Arrow key.", { screenSize.width / 2 - (11 * 32), 396 }, 0.5f, { 255, 255, 255, 255 });
		pFont->Draw("Rotate Right using the D or Right Arrow key.", { screenSize.width / 2 - (11 * 32), 428 }, 0.5f, { 255, 255, 255, 255 });
		pFont->Draw("Fire Your Lasers using the Space key.", { screenSize.width / 2 - (11 * 32), 492 }, 0.5f, { 255, 255, 255, 255 });
	}
	break;
	case 2:
	{
		pFont->Draw("Enemy Ship", { screenSize.width / 2 - (4.2f * 32), 100 }, 0.75f, { 255, 255, 255, 255 });
		SGD::GraphicsManager::GetInstance()->DrawTexture(m_hEnemyImg, { (screenSize.width / 2) - 32, 200 }, SGD::PI, { 64 / 2, 64 / 2 });
		pFont->Draw("Enemy Space Yetis spawn every few seconds and begin", { screenSize.width / 2 - (13 * 32), 300 }, 0.5f, { 255, 255, 255, 255 });
		pFont->Draw("to move about space in search of any human players.", { screenSize.width / 2 - (13 * 32), 332 }, 0.5f, { 255, 255, 255, 255 });
		pFont->Draw("If you come within viewing range of the terrible yetis,", { screenSize.width / 2 - (13 * 32), 364 }, 0.5f, { 255, 255, 255, 255 });
		pFont->Draw("they will begin to fire upon you. If you manage to get", { screenSize.width / 2 - (13 * 32), 396 }, 0.5f, { 255, 255, 255, 255 });
		pFont->Draw("far enough away the yeti will lose you and begin his", { screenSize.width / 2 - (13 * 32), 428 }, 0.5f, { 255, 255, 255, 255 });
		pFont->Draw("hunt all over again.", { screenSize.width / 2 - (13 * 32), 460 }, 0.5f, { 255, 255, 255, 255 });
	}
	break;
	//case 3:
	//{

	//}
	break;
	default:
		break;
	}
}

