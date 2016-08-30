//***********************************************************************
//	File:		DestroyEntityMessage.h
//	Author:		Brian McLaine
//	Course:		SGD
//	Purpose:	
//***********************************************************************

#include "DestroyEntityMessage.h"
#include "Entity.h"
#include "MessageID.h"
#include "../SGD Wrappers/SGD_Utilities.h"

DestroyEntityMessage::DestroyEntityMessage(Entity* ptr) : Message(MessageID::MSG_DESTROY_ENTITY)
{
	SGD_ASSERT(ptr != nullptr,
		"DestroyEntityMessage - parameter cannot be null");

	m_pEntity = ptr;
	m_pEntity->AddRef();
}

/*virtual*/ DestroyEntityMessage::~DestroyEntityMessage()
{
	m_pEntity->Release();
	m_pEntity = nullptr;
}
