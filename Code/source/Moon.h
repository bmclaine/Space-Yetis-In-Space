//*********************************************************************//
//	File:		Moon.h
//	Author:		Brian McLaine
//	Course:		SGD
//	Purpose:	Moon class handles the moon entity
//*********************************************************************//

#pragma once

#include "Entity.h"

#include "../SGD Wrappers/SGD_IListener.h"
#include "../SGD Wrappers/SGD_Handle.h"

class CellAnimation;

class Moon : public Entity
{
public: Moon(void) = default;
protected: ~Moon(void);

public:
		   //*******************************************************************
		   // IEntity Interface:
		   virtual void Update(float elapsedTime) override;
		   virtual void Render(void) override;

		   virtual int GetType(void) const override { return ENT_MOON; }
		   virtual void HandleCollision(const IEntity* pOther)	override;

		   //*******************************************************************
		   // Accessors:
		   SGD::HAudio GetMoonExplosionSfx(void) const { return m_hMoonExplosionSfx; }
		   bool GetIsMoonA(void) const { return m_bIsMoonA; }
		   bool GetIsDead(void) const { return m_bIsDead; }

		   // Mutators:
		   void SetMoonExplosionSfx(SGD::HAudio sfx) { m_hMoonExplosionSfx = sfx; }
		   void SetIsMoonA(bool isMoonA) { m_bIsMoonA = isMoonA; }
		   void SetIsDead(bool isDead) { m_bIsDead = isDead; }


private:
	CellAnimation* m_pMoonAnim;
	CellAnimation* m_pMoonExplosionAnim;
	SGD::HAudio m_hMoonExplosionSfx = SGD::INVALID_HANDLE;
	int m_nHealth = 1000;
	float m_fSpeed = 100.0f;
	bool m_bIsMoonA = false;
	bool m_bIsDead = false;
};

