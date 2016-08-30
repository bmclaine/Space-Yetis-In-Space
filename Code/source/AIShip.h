//*********************************************************************//
//	File:		AIShip.h
//	Author:		Brian McLaine
//	Course:		SGD
//	Purpose:	AIShip class handles the enemy ship entity
//*********************************************************************//

#pragma once

#include "Ship.h"

#include "../SGD Wrappers/SGD_IListener.h"

class AIShip : public Ship
{
public: AIShip(void);
protected: 
	virtual ~AIShip(void);
	
public:
	//*******************************************************************
	// IEntity Interface:
	virtual void Update(float elapsedTime) override;
	virtual void Render(void) override;

	virtual int GetType(void) const override { return ENT_ENEMY_SHIP; }
	virtual void HandleCollision(const IEntity* pOther)	override;

	//*******************************************************************
	// Listener Interface:
	virtual void HandleEvent(const SGD::Event* pEvent) override; // Callback function to process events

	//*******************************************************************
	// LesserAIShip Interface:
	void RandomizeWayPoint(void);

	//*******************************************************************
	// Accessors:
	Entity* GetFocus(void) const { return m_pFocus; }
	float GetWayPointX(void) const { return m_fWayPointX; }
	float GetWayPointY(void) const { return m_fWayPointY; }

	// Mutators:
	void SetFocus(Entity* focus);
	void SetWayPointX(float wayPointX) { m_fWayPointX = wayPointX; }
	void SetWayPointY(float wayPointY) { m_fWayPointY = wayPointY; }

private:
	//*******************************************************************
	// Members:
	Entity* m_pFocus = nullptr;
	float m_fWayPointX = 0.0f;
	float m_fWayPointY = 0.0f;
	float m_fSpeed = 0;
	float m_fAccelRate = 100;
	float m_fTurnRate = SGD::PI / 2;
	
};

