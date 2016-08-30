//*********************************************************************//
//	File:		OptionsMenuState.cpp
//	Author:		Brian McLaine
//	Course:		SGD
//	Purpose:	OptionsMenuState class handles the options menu
//*********************************************************************//

#include "OptionsMenuState.h"
#include "MainMenuState.h"
#include "BitmapFont.h"
#include "Game.h"

#include "../SGD Wrappers/SGD_AudioManager.h"
#include "../SGD Wrappers/SGD_InputManager.h"
#include "../SGD Wrappers/SGD_String.h"

#include <fstream>

//*********************************************************************//
// GetInstance
//	- create & return THE singleton object
/*static*/ OptionsMenuState* OptionsMenuState::GetInstance(void)
{
	static OptionsMenuState s_Instance;

	return &s_Instance;
}

//*********************************************************************//
// Enter
//	- called EACH time the screen is shown/switched to
//	- load resources / reset data
/*virtual*/ void OptionsMenuState::Enter(void) /*override*/
{
	m_unDefaultMusLvl = m_unMusicLevel = SGD::AudioManager::GetInstance()->GetMasterVolume(SGD::AudioGroup::Music);
	m_unDefaultSfxLvl = m_unSfxLevel = SGD::AudioManager::GetInstance()->GetMasterVolume(SGD::AudioGroup::SoundEffects);

	m_hMusic = SGD::AudioManager::GetInstance()->LoadAudio(L"./resource/audio/BM_BackgroundLoop.xwm");
	m_hSFX = SGD::AudioManager::GetInstance()->LoadAudio(L"./resource/audio/BM_PlayerLaser.wav");
}

//*********************************************************************//
// Exit
//	- called EACH time the screen is hidden/switched from
//	- unload resources
/*virtual*/ void OptionsMenuState::Exit(void) /*override*/
{
	SGD::AudioManager::GetInstance()->UnloadAudio(m_hMusic);
	SGD::AudioManager::GetInstance()->UnloadAudio(m_hSFX);
}


//*********************************************************************//
// Update
//	- called EVERY FRAME
//	- handle input & update
/*virtual*/ bool OptionsMenuState::Update(float elapsedTime) /*override*/
{
	SGD::InputManager* pInput = SGD::InputManager::GetInstance();
	SGD::AudioManager* pAudio = SGD::AudioManager::GetInstance();

	pAudio->SetMasterVolume(SGD::AudioGroup::Music, m_unMusicLevel);
	pAudio->SetMasterVolume(SGD::AudioGroup::SoundEffects, m_unSfxLevel);

	if (pInput->IsKeyPressed(SGD::Key::Escape) == true)
	{
		pAudio->SetMasterVolume(SGD::AudioGroup::Music, m_unDefaultMusLvl);
		pAudio->SetMasterVolume(SGD::AudioGroup::SoundEffects, m_unDefaultSfxLvl);
		Game::GetInstance()->ChangeState(MainMenuState::GetInstance());
		return true;
	}

	if (pInput->IsKeyPressed(SGD::Key::DownArrow) == true || pInput->IsKeyPressed(SGD::Key::S) == true)
	{
		m_nCursor++;
		if (m_nCursor > 2)
		{
			m_nCursor = 0;
		}
	}
	else if (pInput->IsKeyPressed(SGD::Key::UpArrow) == true || pInput->IsKeyPressed(SGD::Key::W) == true)
	{
		m_nCursor--;
		if (m_nCursor < 0)
		{
			m_nCursor = 2;
		}
	}

	if (pInput->IsKeyPressed(SGD::Key::LeftArrow) == true || pInput->IsKeyPressed(SGD::Key::A) == true)
	{
		if (m_nCursor == 0 && m_unMusicLevel > 0)
		{
			m_unMusicLevel -= 5;

			if (pAudio->IsAudioPlaying(m_hMusic))
				pAudio->StopAudio(m_hMusic);
			else if (pAudio->IsAudioPlaying(m_hSFX))
				pAudio->StopAudio(m_hSFX);

			pAudio->PlayAudio(m_hMusic);
		}

		if (m_nCursor == 1 && m_unSfxLevel > 0)
		{
			m_unSfxLevel -= 5;

			if (pAudio->IsAudioPlaying(m_hMusic))
				pAudio->StopAudio(m_hMusic);
			else if (pAudio->IsAudioPlaying(m_hSFX))
				pAudio->StopAudio(m_hSFX);

			pAudio->PlayAudio(m_hSFX);
		}
	}
	else if (pInput->IsKeyPressed(SGD::Key::RightArrow) == true || pInput->IsKeyPressed(SGD::Key::D) == true)
	{
		if (m_nCursor == 0 && m_unMusicLevel < 100)
		{
			m_unMusicLevel += 5;

			if (pAudio->IsAudioPlaying(m_hMusic))
				pAudio->StopAudio(m_hMusic);
			else if (pAudio->IsAudioPlaying(m_hSFX))
				pAudio->StopAudio(m_hSFX);

			pAudio->PlayAudio(m_hMusic);
		}

		if (m_nCursor == 1 && m_unSfxLevel < 100)
		{
			m_unSfxLevel += 5;

			if (pAudio->IsAudioPlaying(m_hMusic))
				pAudio->StopAudio(m_hMusic);
			else if (pAudio->IsAudioPlaying(m_hSFX))
				pAudio->StopAudio(m_hSFX);

			pAudio->PlayAudio(m_hSFX);
		}
	}

	if (pInput->IsKeyPressed(SGD::Key::Enter) && m_nCursor == 2)
	{
		SaveVolumes();
		Game::GetInstance()->ChangeState(MainMenuState::GetInstance());
		return true;
	}

	return true;
}

//*********************************************************************//
// Render
//	- called EVERY FRAME
//	- draw menus / entities
/*virtual*/ void OptionsMenuState::Render(float elapsedTime) /*override*/
{
	BitmapFont* pFont = Game::GetInstance()->GetFont();
	SGD::Size screenSize = Game::GetInstance()->GetScreenSize();

	SGD::OStringStream musicVol, sfxVol;

	musicVol << m_unMusicLevel;
	sfxVol << m_unSfxLevel;

	pFont->Draw("Music Volume: ", { (screenSize.width / 2) - (8 * 32), 250 }, 1.0f, { 255, 255, 255, 255 });
	pFont->Draw(musicVol.str().c_str(), { (screenSize.width / 2) + (6 * 32), 250 }, 1.0f, { 255, 255, 255, 255 });
	pFont->Draw("SFX Volume: ", { (screenSize.width / 2) - (8 * 32), 300 }, 1.0f, { 255, 255, 255, 255 });
	pFont->Draw(sfxVol.str().c_str(), { (screenSize.width / 2) + (6 * 32), 300 }, 1.0f, { 255, 255, 255, 255 });
	pFont->Draw("Save and Exit", { (screenSize.width / 2) - (8 * 32), 350 }, 1.0f, { 255, 255, 255, 255 });

	pFont->Draw("=", { (screenSize.width - (18.5f * 32)) / 2, 250.0f + 50 * m_nCursor }, 1.0f, { 255, 255, 255, 255 });
}

void OptionsMenuState::SaveVolumes(void)
{
	SGD::AudioManager::GetInstance()->SetMasterVolume(SGD::AudioGroup::Music, m_unMusicLevel);
	SGD::AudioManager::GetInstance()->SetMasterVolume(SGD::AudioGroup::SoundEffects, m_unSfxLevel);

	std::ofstream fout;

	fout.open("volumes.txt", std::ios_base::trunc);

	if (fout.is_open())
	{
		fout << m_unMusicLevel << '\t' << m_unSfxLevel;

		fout.close();
	}

}

