//*********************************************************************//
//	File:		IntroState.cpp
//	Author:		Brian McLaine
//	Course:		SGD
//	Purpose:	IntroState class handles the intro screen
//*********************************************************************//

#include "IntroState.h"

#include "Game.h"
#include "BitmapFont.h"
#include "MainMenuState.h"

#include "../SGD Wrappers/SGD_InputManager.h"

//*********************************************************************//
// GetInstance
//	- create & return THE singleton object
/*static*/ IntroState* IntroState::GetInstance(void)
{
	static IntroState s_Instance;

	return &s_Instance;
}

//*********************************************************************//
// Enter
//	- called EACH time the screen is shown/switched to
//	- load resources / reset data
/*virtual*/ void IntroState::Enter(void)		/*override*/
{
	SGD::Size screenSize = Game::GetInstance()->GetScreenSize();

	m_fTitlePosX = screenSize.width + 128;
	m_fSubTitlePosX = -128;
}

//*********************************************************************//
// Exit
//	- called EACH time the screen is hidden/switched from
//	- unload resources
/*virtual*/ void IntroState::Exit(void)		/*override*/
{

}


//*********************************************************************//
// Update
//	- called EVERY FRAME
//	- handle input & update
/*virtual*/ bool IntroState::Update(float elapsedTime)	/*override*/
{
	if (SGD::InputManager::GetInstance()->IsAnyKeyPressed() == true
		&& SGD::InputManager::GetInstance()->IsKeyDown(SGD::Key::Alt) == false)
	{
		Game::GetInstance()->ChangeState(MainMenuState::GetInstance());
	}

	SGD::Size screenSize = Game::GetInstance()->GetScreenSize();

	if (m_fTitlePosX > (screenSize.width - (6.5f * 32 * 4.0f)))
		m_fTitlePosX -= 200 * elapsedTime;
	
	if (m_fSubTitlePosX < (screenSize.width - (-2.3f * 32 * 4.0f)))
		m_fSubTitlePosX += 300 * elapsedTime;

	if (m_fTitlePosX <= (screenSize.width - (6.5f * 32 * 4.0f))
		&& m_fSubTitlePosX >= (screenSize.width - (-2.3f * 32 * 4.0f)))
	{
		Game::GetInstance()->ChangeState(MainMenuState::GetInstance());
	}

	return true;
}

//*********************************************************************//
// Render
//	- called EVERY FRAME
//	- draw menus / entities
/*virtual*/ void IntroState::Render(float elapsedTime)		/*override*/
{
	BitmapFont* pFont = Game::GetInstance()->GetFont();
	SGD::Size screenSize = Game::GetInstance()->GetScreenSize();

	pFont->Draw("Space Yetis!!", { (m_fTitlePosX) / 2, 50 },
		2.0f, { 255, 255, 255, 255 });
	pFont->Draw("In Space", { (m_fSubTitlePosX) / 2, 120 },
		1.0f, { 255, 255, 255, 255 });
}
