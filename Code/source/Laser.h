//***********************************************************************
//	File:		Laser.h
//	Author:		Brian McLaine
//	Course:		SGD
//	Purpose:	Laser class handles laser entity
//***********************************************************************

#pragma once

#include "Entity.h"

class CellAnimation;

class Laser : public Entity
{
public:		Laser(void) = default;
protected:	virtual ~Laser(void);

public:
	//*******************************************************************
	// Interface:	
	virtual void Update(float elapsedTime) override;
	virtual void Render(void) override;

	virtual int GetType(void) const override { return ENT_LASER; }
	virtual void HandleCollision(const IEntity* pOther) override;

	SGD::HAudio GetLaserCollideSfx(void) { return m_hLaserCollideSfx; }
	void SetLaserCollideSfx(SGD::HAudio sfx) { m_hLaserCollideSfx = sfx; }

private:
	CellAnimation* m_pLaserExplosionAnim = nullptr;
	SGD::HAudio m_hLaserCollideSfx = SGD::INVALID_HANDLE;

	float m_fExplosionTimer = 1.0f;
};
