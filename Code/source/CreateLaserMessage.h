//***********************************************************************
//	File:		CreateLaserMessage.h
//	Author:		Brian McLaine
//	Course:		SGD
//	Purpose:	
//***********************************************************************

#pragma once

#include "../SGD Wrappers/SGD_Message.h"

class CreateLaserMessage : public SGD::Message
{
public:
	CreateLaserMessage(float posX, float posY, float rotation, bool isShipLaser = true);
	virtual ~CreateLaserMessage() = default;

	//*******************************************************************
	// Accessors:
	float		GetPosX(void) const	{ return m_fPosX; }
	float		GetPosY(void) const	{ return m_fPosY; }
	float		GetRotation(void) const	{ return m_fRotation; }
	bool		IsPlayerShipLaser(void) const	{ return m_bIsPlayerShipLaser; }

private:
	//*******************************************************************
	// Members:
	float		m_fPosX = 0.0f;
	float		m_fPosY = 0.0f;
	float		m_fRotation = 0.0f;
	bool		m_bIsPlayerShipLaser = true;
};

