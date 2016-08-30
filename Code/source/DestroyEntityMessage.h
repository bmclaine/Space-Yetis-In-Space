//***********************************************************************
//	File:		DestroyEntityMessage.h
//	Author:		Brian McLaine
//	Course:		SGD
//	Purpose:	
//***********************************************************************

#pragma once

#include "../SGD Wrappers/SGD_Message.h"

class Entity;

class DestroyEntityMessage : public SGD::Message
{
public:
	DestroyEntityMessage(Entity* ptr);
	virtual ~DestroyEntityMessage();

	//*******************************************************************
	// Accessors:
	Entity* GetEntity(void) const { return m_pEntity; }

private:
	//*******************************************************************
	// Entity to remove from the Entity Manager
	Entity* m_pEntity = nullptr;
};

