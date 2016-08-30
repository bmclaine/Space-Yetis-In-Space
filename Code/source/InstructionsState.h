//*********************************************************************//
//	File:		InstructionsState.h
//	Author:		Brian McLaine
//	Course:		SGD
//	Purpose:	InstructionsState class handles the game instructions
//*********************************************************************//

#pragma once

#include "IGameState.h"

#include "../SGD Wrappers/SGD_Handle.h"

//*********************************************************************//
// Instructions class
//	- runs the Instructions menu
//	- SINGLETON! (Static allocation, not dynamic)
class InstructionsState : public IGameState
{
public:
	//*****************************************************************//
	// Singleton Accessor:
	static InstructionsState* GetInstance(void);


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
	InstructionsState(void) = default;	// default constructor
	virtual ~InstructionsState(void) = default;	// destructor

	InstructionsState(const InstructionsState&) = delete;	// copy constructor
	InstructionsState& operator= (const InstructionsState&) = delete;	// assignment operator

	//*****************************************************************//
	// Members:
	SGD::HTexture m_hEnemyImg = SGD::INVALID_HANDLE;
	SGD::HTexture m_hPlayerImg = SGD::INVALID_HANDLE;
	
	int m_nCurrSlide = 0;
};
