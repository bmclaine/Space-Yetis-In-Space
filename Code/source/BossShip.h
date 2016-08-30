//*********************************************************************//
//	File:		BossShip.h
//	Author:		Brian McLaine
//	Course:		SGD
//	Purpose:	BossShip class handles the Boss enemy ship entity
//*********************************************************************//

#pragma once

#include "Ship.h"

class BossShip : public Ship
{
public: BossShip(void) = default;
protected: virtual ~BossShip(void);

public:
	//*******************************************************************
	// IEntity Interface:
	virtual void Update(float elapsedTime) override;
	virtual void Render(void) override;

	virtual int GetType(void) const override { return ENT_ENEMY_SHIP; }
	virtual void HandleCollision(const IEntity* pOther)	override;

	void SetMoonA(Entity* moon) { m_pMoonA = moon; }
	void SetMoonB(Entity* moon) { m_pMoonB = moon; }

private:
	//*******************************************************************
	// Members:
	Entity* m_pMoonA = nullptr;
	Entity* m_pMoonB = nullptr;
	float m_fSpeed = 100.0f;
	float m_fAccelRate = 100;
	float m_fTurnRate = SGD::PI / 2;
	float m_fRotationTimer = 1.0f;
	float m_fAttackTimer = 4.0f;
	float m_fFireTimerA = 0.0f;
	float m_fFireTimerB = 0.0f;
	float m_fAttackCooldown = 4.0f;
	bool m_fAttackPhase = false;
	bool m_bRotateCC = false;
};
