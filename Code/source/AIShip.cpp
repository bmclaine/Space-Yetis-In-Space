//*********************************************************************//
//	File:		AIShip.cpp
//	Author:		Brian McLaine
//	Course:		SGD
//	Purpose:	AIShip class handles the enemy ship entity
//*********************************************************************//

#include "AIShip.h"

#include "Game.h"
#include "CellAnimation.h"
#include "CreateLaserMessage.h"
#include "DestroyEntityMessage.h"

#include "../SGD Wrappers/SGD_AudioManager.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_Utilities.h"

#include <cstdlib>

#define AI_SHOT_DELAY 0.2f

AIShip::AIShip(void)
{
	IListener::RegisterForEvent("GAME_OVER");
	IListener::RegisterForEvent("TARGET_DESTROYED");
	IListener::RegisterForEvent("ASSESS_THREAT");
}

/*virtual*/ AIShip::~AIShip(void)
{
	if (m_pFocus != nullptr)
	{
		m_pFocus->Release();
		m_pFocus = nullptr;
	}

	if (m_pShipExplosionAnim != nullptr)
	{
		m_pShipExplosionAnim->Terminate();
		delete m_pShipExplosionAnim;
		m_pShipExplosionAnim = nullptr;
	}
}

/*virtual*/ void AIShip::Update(float elapsedTime) /*override*/
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
		SGD::Vector toWaypoint = {};

		if (m_pFocus == nullptr)
		{
			toWaypoint = SGD::Vector(m_fWayPointX, m_fWayPointY) - SGD::Vector(m_ptPosition.x, m_ptPosition.y);

			if (toWaypoint.ComputeLength() < 100.0f)
			{
				RandomizeWayPoint();
			}
			else
			{
				if (m_fSpeed < 50.0f)
					m_fSpeed += m_fAccelRate * elapsedTime;
			}
		}
		else
		{
			toWaypoint = SGD::Vector(m_pFocus->GetPosition().x, m_pFocus->GetPosition().y) - SGD::Vector(m_ptPosition.x, m_ptPosition.y);
			if (toWaypoint.ComputeLength() > 600.0f)
			{
				RandomizeWayPoint();
				m_pFocus = nullptr;
			}
			else if (toWaypoint.ComputeLength() > 300.0f && m_fSpeed < 50)
				m_fSpeed += m_fAccelRate * elapsedTime;
			else if (toWaypoint.ComputeLength() < 300.0f && m_fSpeed > 0.0f)
				m_fSpeed -= m_fAccelRate * elapsedTime;
		}

		toWaypoint.Normalize();

		SGD::Vector orientation = { 0, -1 };
		orientation.Rotate(m_fRotation);

		if (toWaypoint.ComputeDotProduct(orientation) < 0.999f)
		{
			if (orientation.ComputeSteering(toWaypoint) < 0)
				m_fRotation -= m_fTurnRate * elapsedTime;
			else
				m_fRotation += m_fTurnRate * elapsedTime;
		}

		SGD::Vector velocity = { 0, -1 };
		velocity *= m_fSpeed;
		velocity.Rotate(m_fRotation);
		SetVelocity(velocity);

		Entity::Update(elapsedTime);

		m_fFireTimer += elapsedTime;

		if (m_fFireTimer >= AI_SHOT_DELAY && m_pFocus != nullptr && toWaypoint.ComputeLength() <= 300.0f)
		{
			m_fFireTimer = 0.0f;

			CreateLaserMessage* pMsg = new CreateLaserMessage(
				m_ptPosition.x + m_szSize.width / 2,
				m_ptPosition.y + m_szSize.height / 2,
				m_fRotation, false);

			pMsg->QueueMessage();
			pMsg = nullptr;
		}
	}
}

/*virtual*/ void AIShip::Render(void) /*override*/
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

/*virtual*/ void AIShip::HandleCollision(const IEntity* pOther)	/*override*/
{
	if (pOther->GetType() == ENT_LASER)
	{
		m_nHealth -= 50;

		int points = 10;
		SGD::Event event = { "INCREASE_SCORE", &points, this };
		event.SendEventNow();
	}
	
	if (pOther->GetType() == ENT_PLAYER_SHIP || m_nHealth <= 0)
	{
		SGD::AudioManager::GetInstance()->PlayAudio(m_hShipExplosionSfx);

		m_szSize = { 0, 0 };

		if (m_pShipExplosionAnim == nullptr)
		{
			m_pShipExplosionAnim = new CellAnimation;
			m_pShipExplosionAnim->InitializeAIShipExplosion();
		}

		m_pShipExplosionAnim->Restart(false, 1.0f);

		int points = 100;
		SGD::Event event = { "INCREASE_SCORE", &points, this };
		event.SendEventNow();
	}
}

/*virtual*/ void AIShip::HandleEvent(const SGD::Event* pEvent) /*override*/
{
	if (pEvent->GetEventID() == "GAME_OVER")
		SetFocus(nullptr);
	else if (pEvent->GetEventID() == "TARGET_DESTROYED")
	{
		if (pEvent->GetSender() == m_pFocus)
		{
			SetFocus(nullptr);
			RandomizeWayPoint();
		}
	}
	else if (pEvent->GetEventID() == "ASSESS_THREAT")
	{
		if (m_pFocus == nullptr)
		{
			Ship* pShip = reinterpret_cast<Ship*>(pEvent->GetSender());

			SGD::Vector toFocus = SGD::Vector(pShip->GetPosition().x, pShip->GetPosition().y) - SGD::Vector(m_ptPosition.x, m_ptPosition.y);

			if (toFocus.ComputeLength() < 500.0f)
			{
				toFocus.Normalize();

				SGD::Vector orientation = { 0, -1 };
				orientation.Rotate(m_fRotation);

				if (toFocus.ComputeDotProduct(orientation) > 0.25f)
					SetFocus(pShip);
			}
		}
	}
}

void AIShip::RandomizeWayPoint(void)
{
	m_fWayPointX = float(rand() % int(Game::GetInstance()->GetScreenSize().width - 128.0f) + 64.0f);
	m_fWayPointY = float(rand() % int(Game::GetInstance()->GetScreenSize().height - 128.0f) + 64.0f);
}

void AIShip::SetFocus(Entity* focus)
{
	if (m_pFocus != nullptr)
		m_pFocus->Release();

	m_pFocus = focus;

	if (m_pFocus != nullptr)
		m_pFocus->AddRef();
}


