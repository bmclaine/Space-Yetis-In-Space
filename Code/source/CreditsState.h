//*********************************************************************//
//	File:		CreditsState.h
//	Author:		Brian McLaine
//	Course:		SGD
//	Purpose:	CreditsState class handles the credits
//*********************************************************************//

#pragma once

#include "IGameState.h"

//*********************************************************************//
// MainMenuState class
//	- runs the main menu
//	- SINGLETON! (Static allocation, not dynamic)
class CreditsState : public IGameState
{
public:
	//*****************************************************************//
	// Singleton Accessor:
	static CreditsState* GetInstance(void);


	//*****************************************************************//
	// IGameState Interface:
	virtual void Enter(void)				override;	// load resources / reset data
	virtual void Exit(void)				override;	// unload resources

	virtual bool Update(float elapsedTime)	override;	// handle input / update entities
	virtual void Render(float elapsedTime)	override;	// draw entities / menu

private:
	//*****************************************************************//
	// SINGLETON (not-dynamically allocated)
	//	- Hide the "Quadrilogy-of-Evil" so they cannot be called
	//	  by outside functions
	CreditsState(void) = default;	// default constructor
	virtual ~CreditsState(void) = default;	// destructor

	CreditsState(const CreditsState&) = delete;	// copy constructor
	CreditsState& operator= (const CreditsState&) = delete;	// assignment operator

	//*****************************************************************//
	// Members
	float m_fPosY = 0.0f;
};
