//*********************************************************************//
//	File:		BossShip.cpp
//	Author:		Brian McLaine
//	Course:		SGD
//	Purpose:	BossShip class handles the boss ship entity
//*********************************************************************//

#include "BossShip.h"

#include "Game.h"
#include "Moon.h"
#include "CellAnimation.h"
#include "CreateLaserMessage.h"
#include "DestroyEntityMessage.h"

#include "../SGD Wrappers/SGD_AudioManager.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_Utilities.h"

#include <cstdlib>

#define BOSS_SHOT_DELAYA 0.03f
#define BOSS_SHOT_DELAYB 0.07f

/*virtual*/ BossShip::~BossShip(void)
{
	if (m_pShipExplosionAnim != nullptr)
	{
		m_pShipExplosionAnim->Terminate();
		delete m_pShipExplosionAnim;
		m_pShipExplosionAnim = nullptr;
	}
}

/*virtual*/ void BossShip::Update(float elapsedTime) /*override*/
{
	if (m_pShipExplosionAnim != nullptr &&
		m_pShipExplosionAnim->IsPlaying())
	{
		m_pShipExplosionAnim->Update(elapsedTime);

		if (m_pShipExplosionAnim->IsFinished())
		{
			DestroyEntityMessage* pDestroyMsg = new DestroyEntityMessage{ this };
			pDestroyMsg->QueueMessage();
			pDestroyMsg = nullptr;
		}
	}
	else
	{
		SGD::Size screenSize = Game::GetInstance()->GetScreenSize();
		SGD::Vector toWaypoint = SGD::Vector((screenSize.width / 2) - 64, 64) - SGD::Vector(m_ptPosition.x, m_ptPosition.y);


		if (toWaypoint.ComputeLength() <= 16)
		{
			m_fSpeed = 0;
			m_fAttackPhase = true;
		}

		SGD::Vector velocity = { 0, -1 };
		velocity *= m_fSpeed;
		velocity.Rotate(m_fRotation);
		SetVelocity(velocity);

		Entity::Update(elapsedTime);

		if (m_fAttackPhase == true && m_fAttackTimer > 0)
		{
			m_fRotationTimer -= elapsedTime;
			m_fAttackTimer -= elapsedTime;

			if (m_fRotationTimer <= 0)
			{
				m_fRotationTimer = 2.0f;

				if (m_bRotateCC == true)
					m_bRotateCC = false;
				else
					m_bRotateCC = true;
			}

			if (m_bRotateCC == true)
				m_fRotation -= m_fTurnRate * elapsedTime;
			else
				m_fRotation += m_fTurnRate * elapsedTime;

			m_fFireTimerA += elapsedTime;
			m_fFireTimerB += elapsedTime;

			if (m_fFireTimerA >= BOSS_SHOT_DELAYA)
			{
				m_fFireTimerA = 0.0f;

				CreateLaserMessage* pMsg = new CreateLaserMessage(
					m_ptPosition.x + m_szSize.width / 2,
					m_ptPosition.y + m_szSize.height / 2,
					m_fRotation, false);

				pMsg->QueueMessage();
				pMsg = nullptr;

				if (m_fFireTimerB >= BOSS_SHOT_DELAYB)
				{
					m_fFireTimerB = 0.0f;

					CreateLaserMessage* pMsg = new CreateLaserMessage(
						m_ptPosition.x + m_szSize.width / 2,
						m_ptPosition.y + m_szSize.height / 2,
						m_fRotation, false);

					pMsg->QueueMessage();
					pMsg = nullptr;
				}
			}
		}

		if (m_fAttackTimer <= 0)
			m_fAttackCooldown -= elapsedTime;

		if (m_fAttackCooldown <= 0)
		{
			m_fAttackCooldown = 4.0f;
			m_fAttackTimer = 4.0f;
			m_fRotationTimer = 1.0f;
		}
	}
}

/*virtual*/ void BossShip::Render(void) /*override*/
{
	if (m_pShipExplosionAnim != nullptr &&
		m_pShipExplosionAnim->IsPlaying())
	{
		m_pShipExplosionAnim->Render(m_ptPosition);
	}
	else
	{
		SGD_ASSERT(m_hImage != SGD::INVALID_HANDLE,
			"Ship::Render - ship image was not set!");

		SGD::GraphicsManager* pGraphics = SGD::GraphicsManager::GetInstance();

		// Draw the AIShip Image
		pGraphics->DrawTexture(m_hImage, m_ptPosition, m_fRotation, m_szSize / 2);
	}
}

/*virtual*/ void BossShip::HandleCollision(const IEntity* pOther)	/*override*/
{
	Moon* pMoonA = dynamic_cast<Moon*>(m_pMoonA);
	Moon* pMoonB = dynamic_cast<Moon*>(m_pMoonB);

	if (pOther->GetType() == ENT_LASER)
	{
		if (pMoonA->GetIsDead() == true && pMoonB->GetIsDead() == true)
		{
			m_nHealth -= 50;

			int points = 10;
			SGD::Event event = { "INCREASE_SCORE", &points, this };
			event.SendEventNow();
		}

	}

	if (pOther->GetType() == ENT_PLAYER_SHIP)
	{
		if (pMoonA->GetIsDead() == true && pMoonB->GetIsDead() == true)
		{
			m_nHealth -= 250;
		}
	}

	if (m_nHealth <= 0)
	{
		SGD::AudioManager::GetInstance()->PlayAudio(m_hShipExplosionSfx);

		m_szSize = { 0, 0 };

		if (m_pShipExplosionAnim == nullptr)
		{
			m_pShipExplosionAnim = new CellAnimation;
			m_pShipExplosionAnim->InitializeBossShipExplosion();
		}

		m_pShipExplosionAnim->Restart(false, 1.0f);
	}
}
