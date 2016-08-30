//*********************************************************************//
//	File:		TallyState.h
//	Author:		Brian McLaine
//	Course:		SGD
//	Purpose:	TallyState class handles the tally score screen
//*********************************************************************//

#pragma once

#include "IGameState.h"

class TallyState : public IGameState
{
public:
	//*****************************************************************//
	// Singleton Accessor:
	static TallyState* GetInstance(void);


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
	TallyState(void) = default;	// default constructor
	virtual ~TallyState(void) = default;	// destructor

	TallyState(const TallyState&) = delete;	// copy constructor
	TallyState& operator= (const TallyState&) = delete;	// assignment operator

	//*****************************************************************//
	// Members:
	int m_nPlayerScore = 0;
	int m_nScore = 0;
	float m_fTimePlayed = 0.0f;
	float m_fTallyTimer = 0.0f;
};

