//*********************************************************************//
//	File:		Ship.h
//	Author:		Brian McLaine
//	Course:		SGD
//	Purpose:	Ship class handles the ship entity
//*********************************************************************//

#pragma once

#include "Entity.h"

#include "../SGD Wrappers/SGD_Event.h"
#include "../SGD Wrappers/SGD_IListener.h"

class CellAnimation;

class Ship : public Entity, public SGD::IListener
{
	//*******************************************************************
	// Default constructor and destructor
public: Ship(void);
protected:
	virtual ~Ship(void); // Protected to enforce reference counting

public:
	//*******************************************************************
	// IEntity Interface:
	virtual void Update(float elapsedTime) override;
	virtual void Render(void) override;

	virtual int GetType(void) const override { return ENT_PLAYER_SHIP; }
	virtual void HandleCollision(const IEntity* pOther)	override;

	//*******************************************************************
	// Listener Interface:
	virtual void HandleEvent(const SGD::Event* pEvent) override; // Callback function to process events

	//*******************************************************************
	// Ship Interface:
	void Respawn(void);

	//*******************************************************************
	// Accessors:
	SGD::HAudio GetShipExplosionSfx(void) const { return m_hShipExplosionSfx; }
	SGD::HAudio GetGameOverSfx(void) const { return m_hGameOverSfx; }
	int GetHealth(void) const { return m_nHealth; }
	int GetLives(void) const { return m_nLives; }
	int GetScore(void) const { return m_nScore; }
	float GetTimePlayed(void) const { return m_fTimePlayed; }

	// Mutators:
	void SetShipExplosionSfx(SGD::HAudio sfx) { m_hShipExplosionSfx = sfx; }
	void SetGameOverSfx(SGD::HAudio sfx) { m_hGameOverSfx = sfx; }
	void SetHealth(int health) { m_nHealth = health; }
	void SetLives(int lives) { m_nLives = lives; }
	void SetScore(int score) { m_nScore = score; }

private:
	//*******************************************************************
	// Members:
	SGD::HAudio m_hGameOverSfx = SGD::INVALID_HANDLE;
	int m_nLives = 3;
	int m_nScore = 0;
	int m_nScoreMod = 1;

protected:
	CellAnimation* m_pShipExplosionAnim = nullptr;
	SGD::HAudio m_hShipExplosionSfx = SGD::INVALID_HANDLE;
	int m_nHealth = 1000;
	float m_fFireTimer = 0.0f;
	float m_fTimePlayed = 0.0f;
};

