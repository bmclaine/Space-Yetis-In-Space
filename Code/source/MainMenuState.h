//*********************************************************************//
//	File:		MainMenuState.h
//	Author:		Brian McLaine
//	Course:		SGD
//	Purpose:	MainMenuState class handles the main menu
//*********************************************************************//

#pragma once

#include "IGameState.h"

#include "../SGD Wrappers/SGD_Handle.h"

//*********************************************************************//
// MainMenuState class
//	- runs the main menu
//	- SINGLETON! (Static allocation, not dynamic)
class MainMenuState : public IGameState
{
public:
	//*****************************************************************//
	// Singleton Accessor:
	static MainMenuState* GetInstance( void );

	
	//*****************************************************************//
	// IGameState Interface:
	virtual void Enter	( void )				override;	// load resources / reset data
	virtual void Exit	( void )				override;	// unload resources

	virtual bool Update	( float elapsedTime )	override;	// handle input / update entities
	virtual void Render	( float elapsedTime )	override;	// draw entities / menu

private:
	//*****************************************************************//
	// SINGLETON (not-dynamically allocated)
	//	- Hide the "Quadrilogy-of-Evil" so they cannot be called
	//	  by outside functions
	MainMenuState( void )			= default;	// default constructor
	virtual ~MainMenuState( void )	= default;	// destructor

	MainMenuState( const MainMenuState& )				= delete;	// copy constructor
	MainMenuState& operator= ( const MainMenuState& )	= delete;	// assignment operator

	//*****************************************************************//
	// Assests
	SGD::HAudio m_hMenuLoopMus = SGD::INVALID_HANDLE;

	//*****************************************************************//
	// cursor index / position
	int m_nCursor = 0;
};

