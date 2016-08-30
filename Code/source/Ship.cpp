//*********************************************************************//
//	File:		Ship.cpp
//	Author:		Brian McLaine
//	Course:		SGD
//	Purpose:	Ship class handles the ship entity
//*********************************************************************//

#include "Ship.h"
#include "Game.h"
#include "GameplayState.h"
#include "CreateLaserMessage.h"
#include "BitmapFont.h"
#include "CellAnimation.h"

#include "../SGD Wrappers/SGD_AudioManager.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_InputManager.h"
#include "../SGD Wrappers/SGD_Utilities.h"
#include "../SGD Wrappers/SGD_String.h"

#define SHOT_DELAY 0.1f

Ship::Ship(void)
{
	IListener::RegisterForEvent("INCREASE_SCORE");
}

/*virtual*/ Ship::~Ship(void)
{
	if (m_pShipExplosionAnim != nullptr)
	{
		m_pShipExplosionAnim->Terminate();
		delete m_pShipExplosionAnim;
		m_pShipExplosionAnim = nullptr;
	}
}

/*virtual*/ void Ship::Update(float elapsedTime) /*override*/
{
	m_fTimePlayed += elapsedTime;

	if (m_pShipExplosionAnim != nullptr &&
		m_pShipExplosionAnim->IsPlaying())
	{
		m_pShipExplosionAnim->Update(elapsedTime);

		if (m_pShipExplosionAnim->IsFinished())
		{
			m_pShipExplosionAnim->Terminate();
			delete m_pShipExplosionAnim;
			m_pShipExplosionAnim = nullptr;
		}
	}
	else
	{
		SGD::InputManager* pInput = SGD::InputManager::GetInstance();

		if (m_nHealth <= 0)
		{
			if (pInput->IsKeyPressed(SGD::Key::Enter) == true && pInput->IsKeyDown(SGD::Key::Alt) == false && m_nLives > 0)
			{
				Respawn();
			}
			else
				return;
		}

		m_fFireTimer += elapsedTime;

		if (pInput->IsKeyDown(SGD::Key::Space) == true && m_fFireTimer >= SHOT_DELAY)
		{
			CreateLaserMessage* pMsg = new CreateLaserMessage(
				m_ptPosition.x + m_szSize.width / 2,
				m_ptPosition.y + m_szSize.height / 2,
				m_fRotation, true);

			pMsg->QueueMessage();
			pMsg = nullptr;

			m_fFireTimer = 0.0f;
		}

		if (pInput->IsKeyDown(SGD::Key::Right) == true || pInput->IsKeyDown(SGD::Key::D) == true)
		{
			m_fRotation += SGD::PI / 2 * elapsedTime;
		}
		else if (pInput->IsKeyDown(SGD::Key::Left) == true || pInput->IsKeyDown(SGD::Key::A) == true)
		{
			m_fRotation -= SGD::PI / 2 * elapsedTime;
		}

		if (pInput->IsKeyDown(SGD::Key::Up) == true || pInput->IsKeyDown(SGD::Key::W) == true)
		{

			SGD::Vector vAcceleration = { 0, -1 };

			vAcceleration.Rotate(m_fRotation);

			vAcceleration *= 120;

			m_vtVelocity += vAcceleration * elapsedTime;

			float speed = m_vtVelocity.ComputeLength();
			if (speed > 100)
				m_vtVelocity *= (100 / speed);
		}
		else if (pInput->IsKeyDown(SGD::Key::Down) == true || pInput->IsKeyDown(SGD::Key::S) == true)
		{

			SGD::Vector vAcceleration = { 0, -1 };

			vAcceleration.Rotate(m_fRotation);

			vAcceleration *= 120;

			m_vtVelocity -= vAcceleration * elapsedTime;

			float speed = m_vtVelocity.ComputeLength();
			if (speed > 100)
				m_vtVelocity *= (100 / speed);
		}

		Entity::Update(elapsedTime);

		SGD::Size screenSize = Game::GetInstance()->GetScreenSize();

		// Wrap around screen
		if (m_ptPosition.x < -m_szSize.width)
			m_ptPosition.x = screenSize.width;
		else if (m_ptPosition.x > screenSize.width)
			m_ptPosition.x = -m_szSize.width;

		if (m_ptPosition.y < -m_szSize.height)
			m_ptPosition.y = screenSize.height;
		else if (m_ptPosition.y > screenSize.height)
			m_ptPosition.y = -m_szSize.height;

		SGD::Event* pEvent = new SGD::Event("ASSESS_THREAT", nullptr, this);
		pEvent->QueueEvent();
	}
}

/*virtual*/ void Ship::Render(void) /*override*/
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

		// Draw the Ship Image
		pGraphics->DrawTexture(m_hImage, m_ptPosition, m_fRotation, m_szSize / 2);

		// Draw ship data
		BitmapFont* pFont = Game::GetInstance()->GetFont();

		SGD::Size screenSize = Game::GetInstance()->GetScreenSize();

		SGD::OStringStream health, lives, score;

		health << m_nHealth;
		lives << m_nLives;
		score << m_nScore;

		pFont->Draw("Health:", { 50, screenSize.height - 32 },
			0.5f, { 255, 255, 255, 255 });

		pFont->Draw(health.str().c_str(), { 180, screenSize.height - 32 },
			0.5f, { 255, 255, 255, 255 });

		pFont->Draw("Lives:", { 300, screenSize.height - 32 },
			0.5f, { 255, 255, 255, 255 });

		pFont->Draw(lives.str().c_str(), { 415, screenSize.height - 32 },
			0.5f, { 255, 255, 255, 255 });

		pFont->Draw("Score:", { 500, screenSize.height - 32 },
			0.5f, { 255, 255, 255, 255 });

		pFont->Draw(score.str().c_str(), { 615, screenSize.height - 32 },
			0.5f, { 255, 255, 255, 255 });

		if (m_nHealth <= 0)
		{
			SGD::Size screenSize = Game::GetInstance()->GetScreenSize();

			if (m_nLives <= 0)
			{
				SGD::AudioManager::GetInstance()->PlayAudio(m_hGameOverSfx);
				pFont->Draw("GAME OVER", { (screenSize.width / 2) - (4 * 32), screenSize.height / 2 }, 1.0f, { 255, 255, 127, 127 });
			}
			else
			{
				pFont->Draw("YOU DIED!!", { (screenSize.width / 2) - (5 * 32), screenSize.height / 2 }, 1.0f, { 255, 255, 127, 127 });
			}
			pFont->Draw("PRESS ENTER", { (screenSize.width / 2) - (5.5f * 32), (screenSize.height / 2) + 32 }, 1.0f, { 255, 255, 255, 255 });
		}
	}
}

/*virtual*/ void Ship::HandleCollision(const IEntity* pOther)	/*override*/
{
		if (m_nHealth <= 0)
			return;

		if (pOther->GetType() == ENT_LASER)
		{
			m_nHealth -= 25;
			m_nScoreMod = 1;
		}
		else if (pOther->GetType() == ENT_ENEMY_SHIP || pOther->GetType() == ENT_MOON)
		{
			m_nHealth = 0;
			m_nScoreMod = 1;
		}

		if (m_nHealth <= 0)
		{
			SGD::AudioManager::GetInstance()->PlayAudio(m_hShipExplosionSfx);

			m_szSize = { 0, 0 };

			if (m_pShipExplosionAnim == nullptr)
			{
				m_pShipExplosionAnim = new CellAnimation();
				m_pShipExplosionAnim->InitializeShipExplosion();
			}

			m_pShipExplosionAnim->Restart(false, 1.0f);

			SGD::Event* pEvent = new SGD::Event("TARGET_DESTROYED", nullptr, this);
			pEvent->QueueEvent();
			pEvent = nullptr;

			m_nLives--;
		}

		if (m_nLives <= 0)
		{
			GameplayState::GetInstance()->SetGameOver(true);
		}
}

void Ship::Respawn(void)
{
	if (m_nLives == 0)
	{
		m_nLives = 3;
		m_nScore = 0;
	}

	m_nHealth = 1000;
	m_nScoreMod = 1;

	SGD::Size screenSize = Game::GetInstance()->GetScreenSize();
	m_ptPosition = SGD::Point((screenSize.width - 64) / 2, (screenSize.height - 64) / 1.5f);

	m_szSize = {64, 64};

	m_vtVelocity = { 0, 0 };
	m_fRotation = 0.0f;
}


/*virtual*/ void Ship::HandleEvent(const SGD::Event* pEvent) /*override*/
{
	if (pEvent->GetEventID() == "INCREASE_SCORE")
	{
		int* points = reinterpret_cast<int*>(pEvent->GetData());

		if (points != nullptr && *points > 0)
			m_nScore += *points * m_nScoreMod;

		m_nScoreMod += 1;

		if (m_nScoreMod > 4)
			m_nScoreMod = 4;
	}
}
