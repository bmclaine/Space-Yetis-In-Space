//*********************************************************************//
//	File:		CreditsState.cpp
//	Author:		Brian McLaine
//	Course:		SGD
//	Purpose:	CreditsState class handles the credits
//*********************************************************************//

#include "CreditsState.h"
#include "MainMenuState.h"
#include "BitmapFont.h"
#include "Game.h"

#include "../SGD Wrappers/SGD_InputManager.h"

//*********************************************************************//
// GetInstance
//	- create & return THE singleton object
/*static*/ CreditsState* CreditsState::GetInstance(void)
{
	static CreditsState s_Instance;

	return &s_Instance;
}

//*********************************************************************//
// Enter
//	- called EACH time the screen is shown/switched to
//	- load resources / reset data
/*virtual*/ void CreditsState::Enter(void)		/*override*/
{
	m_fPosY = Game::GetInstance()->GetScreenSize().height + 16;
}

//*********************************************************************//
// Exit
//	- called EACH time the screen is hidden/switched from
//	- unload resources
/*virtual*/ void CreditsState::Exit(void)		/*override*/
{

}


//*********************************************************************//
// Update
//	- called EVERY FRAME
//	- handle input & update
/*virtual*/ bool CreditsState::Update(float elapsedTime)	/*override*/
{
	SGD::InputManager* pInput = SGD::InputManager::GetInstance();
	// skip the credits
	if (pInput->IsKeyPressed(SGD::Key::Escape) == true || (m_fPosY + 256) <= 0)
	{
		Game::GetInstance()->ChangeState(MainMenuState::GetInstance());

		return true;
	}

	m_fPosY -= 100 * elapsedTime;

	return true;
}

//*********************************************************************//
// Render
//	- called EVERY FRAME
//	- draw menus / entities
/*virtual*/ void CreditsState::Render(float elapsedTime)		/*override*/
{
	BitmapFont* pFont = Game::GetInstance()->GetFont();
	float screenWidth = Game::GetInstance()->GetScreenSize().width;

	pFont->Draw("Credits", { screenWidth / 2 - (4 * 32), m_fPosY }, 1.0f, { 255, 255, 255, 255 });
	pFont->Draw("Brian McLaine", { screenWidth / 2 - (7 * 32), m_fPosY + 64 }, 1.0f, { 255, 255, 255, 255 });
	pFont->Draw("Structure of Game Design", { screenWidth / 2 - (12 * 32), m_fPosY + 128 }, 1.0f, { 255, 255, 255, 255 });
	pFont->Draw("At Full Sail University", { screenWidth / 2 - (11.5f * 32), m_fPosY + 192 }, 1.0f, { 255, 255, 255, 255 });
}
