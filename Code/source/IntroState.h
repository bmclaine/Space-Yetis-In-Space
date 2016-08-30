//*********************************************************************//
//	File:		IntroState.h
//	Author:		Brian McLaine
//	Course:		SGD
//	Purpose:	IntroState class handles the intro screen
//*********************************************************************//

#pragma once

#include "IGameState.h"

class IntroState : public IGameState
{
public:
	//*****************************************************************//
	// Singleton Accessor:
	static IntroState* GetInstance(void);


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
	IntroState(void) = default;	// default constructor
	virtual ~IntroState(void) = default;	// destructor

	IntroState(const IntroState&) = delete;	// copy constructor
	IntroState& operator= (const IntroState&) = delete;	// assignment operator

	//*****************************************************************//
	// Members:
	float m_fTitlePosX = 0.0f;
	float m_fSubTitlePosX = 0.0f;

};

