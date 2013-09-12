/**************************************************
|	File:		Game.h
|	Author:		Benjamin Budian
|	Course:		SGD
|	Purpose:	CGame class initializes SGD wrappers
|				& handles the game state machine
**************************************************/

#ifndef GAME_H
#define GAME_H


#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include "BitmapFont.h"


/*************************************************/
// Forward Class Declarations
//	- declares that the class name exists
//	- used only for pointers & references
class CSGD_Direct3D;
class CSGD_DirectInput;
class CSGD_TextureManager;
class CSGD_XAudio2;
class CSGD_MessageSystem;
class CSGD_EventSystem;
class IMessage;

class CEntity;
class CEntityManager;

class IGameState;


/*************************************************/
// CGame class
//	- SINGLETON:
//		- only ONE object is created
//		- prevents access to constructors
//		- object could be dynamically-allocated or global object
//		- class provides an access method for the object
//	- initializes & updates SGD Wrappers
//	- handles the game logic
class CGame
{
public:	
	///////////////////////////////////////////////
	// Singleton accessors:
	static CGame* GetInstance( void );
	static void DeleteInstance( void );

	
	///////////////////////////////////////////////
	// Setup, Run, Cleanup
	void Initialize( HWND hWnd, HINSTANCE hInstance,
					 int nWidth, int nHeight,
					 bool bIsWindowed );

	bool Main( void );

	void Terminate( void );
	
	
	///////////////////////////////////////////////
	// Window Accessors:
	int GetScreenWidth ( void ) const	{	return screen_width;	}
	int GetScreenHeight( void ) const	{	return screen_height;	}
	
	///////////////////////////////////////////////
	// Font Accessor:
	CBitmapFont* GetFont( void ) const	{	return font;			}


	///////////////////////////////////////////////
	// Game State Machine:
	void ChangeState( IGameState* pState );


private:	
	///////////////////////////////////////////////
	// Static pointer to the singleton object
	static CGame* s_pInstance;

	// Private constructor / destructor to prevent multiple objects
	CGame( void );						// default ctor
	~CGame( void );						// dtor
	CGame( const CGame& );				// copy ctor
	CGame& operator= ( const CGame& );	// assignment op

	
	///////////////////////////////////////////////
	// Helper Methods
	bool Input( void );
	void Update( void );
	void Render( void );


	
	///////////////////////////////////////////////
	// SGD Wrappers
	CSGD_Direct3D*			direct_3D;
	CSGD_DirectInput*		direct_input;
	CSGD_TextureManager*	texture_manager;
	CSGD_XAudio2*			x_audio;

	CBitmapFont*			font;
	
	///////////////////////////////////////////////
	// Window Attributes
	int						screen_width;
	int						screen_height;
	bool					is_windowed;
	
	///////////////////////////////////////////////
	// Current Game State
	IGameState*				current_state;
	
	///////////////////////////////////////////////
	// Current time
	DWORD					current_time;


};

#endif //GAME_H
