//*********************************************************************//
//	File:		Moon.cpp
//	Author:		Brian McLaine
//	Course:		SGD
//	Purpose:	Moon class handles the moon entity
//*********************************************************************//

#include "Moon.h"

#include "Game.h"
#include "CellAnimation.h"
#include "DestroyEntityMessage.h"

#include "../SGD Wrappers/SGD_AudioManager.h"
#include "../SGD Wrappers/SGD_Event.h"

Moon::~Moon()
{
	if (m_pMoonAnim != nullptr)
	{
		m_pMoonAnim->Terminate();
		delete m_pMoonAnim;
		m_pMoonAnim = nullptr;
	}
	if (m_pMoonExplosionAnim != nullptr)
	{
		m_pMoonExplosionAnim->Terminate();
		delete m_pMoonExplosionAnim;
		m_pMoonExplosionAnim = nullptr;
	}
}

/*virtual*/ void Moon::Update(float elapsedTime) /*override*/
{
	if (m_pMoonAnim == nullptr && m_bIsDead == false)
	{
		m_pMoonAnim = new CellAnimation;
		m_pMoonAnim->InitializeMoon();
		m_pMoonAnim->Restart(true, 0.5f);
	}

	if (m_pMoonAnim != nullptr &&
		m_pMoonAnim->IsPlaying())
	{
		m_pMoonAnim->Update(elapsedTime);
	}
	else if (m_pMoonExplosionAnim != nullptr &&
		m_pMoonExplosionAnim->IsPlaying())
	{
		m_pMoonExplosionAnim->Update(elapsedTime);

		if (m_pMoonExplosionAnim->IsFinished())
		{
			DestroyEntityMessage* pDestroyMsg = new DestroyEntityMessage{ this };

			pDestroyMsg->QueueMessage();
			pDestroyMsg = nullptr;
		}
	}

	SGD::Size screenSize = Game::GetInstance()->GetScreenSize();
	SGD::Vector toWaypoint = {};

	if (m_bIsMoonA == true)
		toWaypoint = SGD::Vector(64, 128) - SGD::Vector(m_ptPosition.x, m_ptPosition.y);
	else
		toWaypoint = SGD::Vector(screenSize.width - 128, 128) - SGD::Vector(m_ptPosition.x, m_ptPosition.y);


		if (toWaypoint.ComputeLength() <= 32)
			m_fSpeed = 0;

	SGD::Vector velocity = { 0, -1 };
	velocity *= m_fSpeed;
	velocity.Rotate(m_fRotation);
	SetVelocity(velocity);

	Entity::Update(elapsedTime);
}

/*virtual*/ void Moon::Render(void) /*override*/
{
	if (m_pMoonAnim != nullptr &&
		m_pMoonAnim->IsPlaying())
	{
		m_pMoonAnim->Render(m_ptPosition);
	}
	else if (m_pMoonExplosionAnim != nullptr &&
		m_pMoonExplosionAnim->IsPlaying())
	{
		m_pMoonExplosionAnim->Render(m_ptPosition);
	}
}

/*virtual*/ void Moon::HandleCollision(const IEntity* pOther)	/*override*/
{
	if (pOther->GetType() == ENT_LASER)
	{
		m_nHealth -= 50;

		int points = 10;
		SGD::Event event = { "INCREASE_SCORE", &points, this };
		event.SendEventNow();
	}

	if (pOther->GetType() == ENT_PLAYER_SHIP)
	{
		m_nHealth -= 250;
	}

	if (m_nHealth <= 0)
	{
		SGD::AudioManager::GetInstance()->PlayAudio(m_hMoonExplosionSfx);

		m_szSize = { 0, 0 };

		m_bIsDead = true;

		if (m_pMoonAnim->IsPlaying())
		{
			m_pMoonAnim->Terminate();
			delete m_pMoonAnim;
			m_pMoonAnim = nullptr;
		}

		if (m_pMoonExplosionAnim == nullptr)
		{
			m_pMoonExplosionAnim = new CellAnimation;
			m_pMoonExplosionAnim->InitializeMoonExplosion();
		}

		m_pMoonExplosionAnim->Restart(false, 1.0f);

		int points = 100;
		SGD::Event event = { "INCREASE_SCORE", &points, this };
		event.SendEventNow();
	}
}
