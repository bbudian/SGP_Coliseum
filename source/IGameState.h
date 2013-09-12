/**************************************************
|	File:		IGameState.h
|	Author:		Doug Monroe (dmonroe@fullsail.com)
|	Course:		SGD
|	Purpose:	IGameState class is the base INTERFACE
|				for all game states.
**************************************************/

#ifndef IGAMESTATE_H
#define IGAMESTATE_H


/*************************************************/
// IGameState class
//	- interface base class for game states
class IGameState
{
public:
	///////////////////////////////////////////////
	// Abstract Base Class
	virtual ~IGameState(void)	= 0
	{
	}
	
	///////////////////////////////////////////////
	// Interface:
	virtual void Enter	( void )				= 0;	// load resources
	virtual void Exit	( void )				= 0;	// unload resources

	virtual bool Input	( void )				= 0;	// handle user input
	virtual void Update	( float fElapsedTime )	= 0;	// update the state
	virtual void Render	( void )				= 0;	// render the state
};

#endif //IGAMESTATE_H
