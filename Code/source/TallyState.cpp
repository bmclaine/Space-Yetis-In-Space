//*********************************************************************//
//	File:		IntroState.cpp
//	Author:		Brian McLaine
//	Course:		SGD
//	Purpose:	IntroState class handles the intro screen
//*********************************************************************//

#include "TallyState.h"

#include "Game.h"
#include "Ship.h"
#include "BitmapFont.h"
#include "CreditsState.h"
#include "GameplayState.h"

#include "../SGD Wrappers/SGD_InputManager.h"
#include "../SGD Wrappers/SGD_String.h"

#include <fstream>

#define TALLY_DELAY 0.01

//*********************************************************************//
// GetInstance
//	- create & return THE singleton object
/*static*/ TallyState* TallyState::GetInstance(void)
{
	static TallyState s_Instance;

	return &s_Instance;
}

//*********************************************************************//
// Enter
//	- called EACH time the screen is shown/switched to
//	- load resources / reset data
/*virtual*/ void TallyState::Enter(void)		/*override*/
{
	std::ifstream fin;

	fin.open("stats.txt");

	if (fin.is_open())
	{
		fin >> m_nPlayerScore >> m_fTimePlayed;

		fin.close();
	}
}

//*********************************************************************//
// Exit
//	- called EACH time the screen is hidden/switched from
//	- unload resources
/*virtual*/ void TallyState::Exit(void)		/*override*/
{

}


//*********************************************************************//
// Update
//	- called EVERY FRAME
//	- handle input & update
/*virtual*/ bool TallyState::Update(float elapsedTime)	/*override*/
{

	if (m_nScore < m_nPlayerScore)
	{
		m_fTallyTimer += elapsedTime;

		if (m_fTallyTimer >= TALLY_DELAY)
		{
			m_nScore += 10;
			m_fTallyTimer = 0.0f;
		}
	}

	if (m_nScore == m_nPlayerScore)
	{
		if (SGD::InputManager::GetInstance()->IsKeyPressed(SGD::Key::Enter) == true
			&& SGD::InputManager::GetInstance()->IsKeyDown(SGD::Key::Alt) == false)
		{
			Game::GetInstance()->ChangeState(CreditsState::GetInstance());
		}
	}

	return true;
}

//*********************************************************************//
// Render
//	- called EVERY FRAME
//	- draw menus / entities
/*virtual*/ void TallyState::Render(float elapsedTime)		/*override*/
{
	BitmapFont* pFont = Game::GetInstance()->GetFont();
	SGD::Size screenSize = Game::GetInstance()->GetScreenSize();

	SGD::OStringStream score, time;
	score << m_nScore;
	time << m_fTimePlayed;

	pFont->Draw("Final Score: ", { (screenSize.width / 2) - (10 * 32), 300 });
	pFont->Draw(score.str().c_str(), { (screenSize.width / 2) + (4 * 32), 300 });

	pFont->Draw("Time Played: ", { (screenSize.width / 2) - (10 * 32), 350 });
	pFont->Draw(time.str().c_str(), { (screenSize.width / 2) + (4 * 32), 350 });
}
