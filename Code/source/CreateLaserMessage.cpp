//***********************************************************************
//	File:		CreateLaserMessage.cpp
//	Author:		Brian McLaine
//	Course:		SGD
//	Purpose:	
//***********************************************************************

#include "CreateLaserMessage.h"
#include "MessageID.h"

CreateLaserMessage::CreateLaserMessage(float posX, float posY, float rotation, bool isPlayerShipLaser)
	: Message(MessageID::MSG_CREATE_LASER)
{
	// Store the parameters
	m_fPosX = posX;
	m_fPosY = posY;
	m_fRotation = rotation;
	m_bIsPlayerShipLaser = isPlayerShipLaser;
}