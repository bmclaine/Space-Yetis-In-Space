//***********************************************************************
//	File:		Laser.cpp
//	Author:		Brian McLaine
//	Course:		SGD
//	Purpose:	Laser class handles laser entity
//***********************************************************************

#include "Laser.h"

#include "Game.h"
#include "DestroyEntityMessage.h"
#include "CellAnimation.h"

#include "../SGD Wrappers/SGD_AudioManager.h"

/*virtual*/ Laser::~Laser(void)
{
	if (m_pLaserExplosionAnim != nullptr)
	{
		m_pLaserExplosionAnim->Terminate();
		delete m_pLaserExplosionAnim;
		m_pLaserExplosionAnim = nullptr;
	}
}

/*virtual*/ void Laser::Update(float elapsedTime)		/*override*/
{
	if (m_pLaserExplosionAnim != nullptr &&
		m_pLaserExplosionAnim->IsPlaying())
	{
		m_pLaserExplosionAnim->Update(elapsedTime);

		if (m_pLaserExplosionAnim->IsFinished())
		{
			DestroyEntityMessage* pDestroyMsg = new DestroyEntityMessage{ this };

			pDestroyMsg->QueueMessage();
			pDestroyMsg = nullptr;
		}
	}
	else
	{
		Entity::Update(elapsedTime);

		SGD::Rectangle rSelf = this->GetRect();
		SGD::Rectangle rScreen = { SGD::Point{ 0, 0 }, Game::GetInstance()->GetScreenSize() };

		if (rSelf.IsIntersecting(rScreen) == false)
		{
			DestroyEntityMessage* pDestroyMsg = new DestroyEntityMessage{ this };

			pDestroyMsg->QueueMessage();
			pDestroyMsg = nullptr;
		}
	}
}

/*virtual*/ void Laser::Render(void) /*override*/
{
	if (m_pLaserExplosionAnim != nullptr &&
		m_pLaserExplosionAnim->IsPlaying())
	{
		m_pLaserExplosionAnim->Render({ m_ptPosition.x - 2, m_ptPosition.y }, 2.0f);
	}
	else
	{
		Entity::Render();
	}
}

/*virtual*/ void Laser::HandleCollision(const IEntity* pOther)	/*override*/
{
	if (pOther->GetType() == ENT_PLAYER_SHIP
		|| pOther->GetType() == ENT_ENEMY_SHIP
		|| pOther->GetType() == ENT_LASER
		|| pOther->GetType() == ENT_MOON)
	{
		// Play sfx
		SGD::AudioManager::GetInstance()->PlayAudio(m_hLaserCollideSfx);

		m_szSize = { 0, 0 };

		if (m_pLaserExplosionAnim == nullptr)
		{
			m_pLaserExplosionAnim = new CellAnimation;
			m_pLaserExplosionAnim->InitializeLaserExplosion();
		}

		m_pLaserExplosionAnim->Restart(false, 1.0f);
	}
}