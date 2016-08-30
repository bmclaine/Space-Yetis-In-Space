//*********************************************************************//
//	File:		MessageID.h
//	Author:		Brian McLaine
//	Course:		SGD
//	Purpose:	MessageID enum class defines the message types
//*********************************************************************//

#pragma once


//*********************************************************************//
// MessageID enum class
//	- unique identifiers for messages sent to Game::MessageProc
enum class MessageID	
{
	MSG_UNKNOWN = 0, MSG_CREATE_LASER = 1, MSG_DESTROY_ENTITY = 2,
};
