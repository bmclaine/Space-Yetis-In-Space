//*********************************************************************//
//	File:		MainMenuState.cpp
//	Author:		Brian McLaine
//	Course:		SGD
//	Purpose:	MainMenuState class handles the main menu
//*********************************************************************//

#include "MainMenuState.h"

#include "Game.h"
#include "BitmapFont.h"
#include "GameplayState.h"
#include "OptionsMenuState.h"
#include "InstructionsState.h"
#include "CreditsState.h"

#include "../SGD Wrappers/SGD_InputManager.h"
#include "../SGD Wrappers/SGD_AudioManager.h"

#include <fstream>

//*********************************************************************//
// GetInstance
//	- create & return THE singleton object
/*static*/ MainMenuState* MainMenuState::GetInstance(void)
{
	static MainMenuState s_Instance;

	return &s_Instance;
}

//*********************************************************************//
// Enter
//	- called EACH time the screen is shown/switched to
//	- load resources / reset data
/*virtual*/ void MainMenuState::Enter(void)		/*override*/
{
	// Load in assets
	m_hMenuLoopMus = SGD::AudioManager::GetInstance()->LoadAudio(L"./resource/audio/BM_MenuLoop.xwm");

	SGD::AudioManager::GetInstance()->PlayAudio(m_hMenuLoopMus, true);

	// Reset the cursor to the top
	m_nCursor = 0;

	// Load in saved volumes
	std::ifstream fin;

	fin.open("volumes.txt");

	if (fin.is_open())
	{
		unsigned int musicLevel, sfxLevel;

		fin >> musicLevel >> sfxLevel;

		SGD::AudioManager::GetInstance()->SetMasterVolume(SGD::AudioGroup::Music, musicLevel);
		SGD::AudioManager::GetInstance()->SetMasterVolume(SGD::AudioGroup::SoundEffects, sfxLevel);

		fin.close();
	}
}

//*********************************************************************//
// Exit
//	- called EACH time the screen is hidden/switched from
//	- unload resources
/*virtual*/ void MainMenuState::Exit(void)		/*override*/
{
	SGD::AudioManager::GetInstance()->UnloadAudio(m_hMenuLoopMus);
}


//*********************************************************************//
// Update
//	- called EVERY FRAME
//	- handle input & update
/*virtual*/ bool MainMenuState::Update(float elapsedTime)	/*override*/
{
	SGD::InputManager* pInput = SGD::InputManager::GetInstance();

	// Press Escape to move the cursor to EXIT
	if (pInput->IsKeyPressed(SGD::Key::Escape) == true)
		m_nCursor = 4;


	// Move the cursor?
	if (pInput->IsKeyPressed(SGD::Key::Down) == true || pInput->IsKeyPressed(SGD::Key::S) == true)
	{
		// next option
		m_nCursor++;

		// wrap around
		if (m_nCursor > 4)
			m_nCursor = 0;
	}
	else if (pInput->IsKeyPressed(SGD::Key::Up) == true || pInput->IsKeyPressed(SGD::Key::W) == true)
	{
		// prev option
		m_nCursor--;

		// wrap around
		if (m_nCursor < 0)
			m_nCursor = 4;
	}


	// Select an option?
	if (pInput->IsKeyPressed(SGD::Key::Enter) == true && pInput->IsKeyDown(SGD::Key::Alt) == false)
	{
		SGD::AudioManager::GetInstance()->StopAudio(m_hMenuLoopMus);

		// Which option is chosen?
		if (m_nCursor == 0) // Play
		{
			// ChangeState is VERY VOLATILE!!!
			//	- can only be safely called by a game state's
			//	 Update or Render methods!
			Game::GetInstance()->ChangeState(GameplayState::GetInstance());

			// Exit this state immediately
			return true;	// keep playing in the new state
		}
		else if (m_nCursor == 1) // Options
		{
			Game::GetInstance()->ChangeState(OptionsMenuState::GetInstance());
			return true;
		}
		else if (m_nCursor == 2) // Instructions
		{
			Game::GetInstance()->ChangeState(InstructionsState::GetInstance());
			return true;
		}
		else if (m_nCursor == 3) // Credits
		{
			Game::GetInstance()->ChangeState(CreditsState::GetInstance());
			return true;
		}
		else //m_nCursor == 4
		{
			return false;	// quit the game
		}
	}


	return true;	// keep playing
}

//*********************************************************************//
// Render
//	- called EVERY FRAME
//	- draw menus / entities
/*virtual*/ void MainMenuState::Render(float elapsedTime)		/*override*/
{
	// Access the bitmap font
	BitmapFont* pFont = Game::GetInstance()->GetFont();


	// Align text based on window width
	float width = Game::GetInstance()->GetScreenSize().width;


	// Display the game title centered at 4x scale
	pFont->Draw("Space Yetis!!", { (width - (6.5f * 32 * 4.0f)) / 2, 50 },
		2.0f, { 255, 255, 255, 255 });
	pFont->Draw("In Space", { (width - (-2.3f * 32 * 4.0f)) / 2, 120 },
		1.0f, { 255, 255, 255, 255 });

	// Display the menu options centered at 1x scale
	pFont->Draw("Play", { (width - (20 * 32)) / 2, 250 },
		1.0f, { 255, 255, 255, 255 });
	pFont->Draw("Options", { (width - (20 * 32)) / 2, 300 },
		1.0f, { 255, 255, 255, 255 });
	pFont->Draw("Instructions", { (width - (20 * 32)) / 2, 350 },
		1.0f, { 255, 255, 255, 255 });
	pFont->Draw("Credits", { (width - (20 * 32)) / 2, 400 },
		1.0f, { 255, 255, 255, 255 });
	pFont->Draw("Exit", { (width - (20 * 32)) / 2, 450 },
		1.0f, { 255, 255, 255, 255 });

	// Display the cursor next to the option
	pFont->Draw("=", { (width - (22 * 32)) / 2, 250.0f + 50 * m_nCursor },
		1.0f, { 255, 255, 255, 255 });
}
