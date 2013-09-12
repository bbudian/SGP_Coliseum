/**************************************************
|	File:		Message.h
|	Author:		Benjamin Budian
|	Course:		SGD
|	Purpose:	CMessage class is the concrete base 
|				class for messages sent to the 
|				Message System.
**************************************************/

#ifndef MESSAGE_H
#define MESSAGE_H


#include "../SGD Wrappers/IMessage.h"


/*************************************************/
// Enumeration of Message Types
enum MessageType {MSG_GAME_OVER, MSG_DESTROY_ENTITY, MSG_CREATE_PROJECTILE };


/*************************************************/
// CMessage class
//	- concrete base class for messages
//	- stores the message ID
class CMessage : public IMessage
{
public:
	///////////////////////////////////////////////
	// Constructor REQUIRES the message type
	CMessage( MessageType type )				{	m_eMsgType = type; 	}
	virtual ~CMessage( void )					{	}
	
	///////////////////////////////////////////////
	// Accessor
	virtual MSGID GetMessageID( void ) final	{	return m_eMsgType;	}

private:
	///////////////////////////////////////////////
	// Message ID
	MessageType		m_eMsgType;
};

#endif //MESSAGE_H
