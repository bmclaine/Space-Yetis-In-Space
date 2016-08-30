//*********************************************************************//
//	File:		OptionsMenuState.h
//	Author:		Brian McLaine
//	Course:		SGD
//	Purpose:	OptionsMenuState class handles the options menu
//*********************************************************************//

#pragma once

#include "IGameState.h"

#include "../SGD Wrappers/SGD_Handle.h"

//*********************************************************************//
// MainMenuState class
//	- runs the main menu
//	- SINGLETON! (Static allocation, not dynamic)
class OptionsMenuState : public IGameState
{
public:
	//*****************************************************************//
	// Singleton Accessor:
	static OptionsMenuState* GetInstance(void);


	//*****************************************************************//
	// IGameState Interface:
	virtual void Enter(void)				override;	// load resources / reset data
	virtual void Exit(void)				override;	// unload resources

	virtual bool Update(float elapsedTime)	override;	// handle input / update entities
	virtual void Render(float elapsedTime)	override;	// draw entities / menu

	//*****************************************************************//
	// Options Interface:
	void SaveVolumes(void);

private:
	//*****************************************************************//
	// SINGLETON (not-dynamically allocated)
	//	- Hide the "Quadrilogy-of-Evil" so they cannot be called
	//	  by outside functions
	OptionsMenuState(void) = default;	// default constructor
	virtual ~OptionsMenuState(void) = default;	// destructor

	OptionsMenuState(const OptionsMenuState&) = delete;	// copy constructor
	OptionsMenuState& operator= (const OptionsMenuState&) = delete;	// assignment operator

	//*****************************************************************//
	// Members:
	SGD::HAudio m_hMusic = SGD::INVALID_HANDLE;
	SGD::HAudio m_hSFX = SGD::INVALID_HANDLE;
	unsigned int m_unDefaultMusLvl = 100;
	unsigned int m_unMusicLevel = 100;
	unsigned int m_unDefaultSfxLvl = 100;
	unsigned int m_unSfxLevel = 100;
	int m_nCursor = 0;
};
