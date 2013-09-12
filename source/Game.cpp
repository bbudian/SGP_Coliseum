/**************************************************
|	File:		Game.cpp
|	Author:		Benjamin Budian
|	Course:		SGD
|	Purpose:	CGame class initializes SGD wrappers
|				& handles the game state machine
**************************************************/


#include "Game.h"

#include "../SGD Wrappers/CSGD_Direct3D.h"
#include "../SGD Wrappers/CSGD_DirectInput.h"
#include "../SGD Wrappers/CSGD_TextureManager.h"
#include "../SGD Wrappers/CSGD_XAudio2.h"

#include "IGameState.h"

#include <cassert>
#include <ctime>

#define FULLSCREEN_WIDTH 1024
#define FULLSCREEN_HEIGHT 1024
#define FULLSCREEN_HEIGHT_PROJECTOR 1024
#define FULLSCREEN_WIDTH_PROJECTOR 768

#define WINDOWED_WIDTH 1024
#define WINDOWED_HEIGHT 1024


/*************************************************/
// SINGLETON:

// Instantiate the static member
/*static*/ CGame* CGame::s_pInstance = nullptr;

// GetInstance
//	- returns the ONE singleton object
/*static*/ CGame* CGame::GetInstance( void )
{
	// Allocate the static pointer if necessary
	if( s_pInstance == nullptr )
		s_pInstance = new CGame;

	// Return the singleton
	return s_pInstance;
}

// DeleteInstance
//	- deallocate the singleton object
/*static*/ void CGame::DeleteInstance( void )
{
	delete s_pInstance;
	s_pInstance = nullptr;
}


/*************************************************/
// Constructor
//	- set all the data members to 'clean' values
CGame::CGame(void)
{
	direct_3D			= nullptr;
	direct_input			= nullptr;
	texture_manager			= nullptr;
	x_audio			= nullptr;

	font			= nullptr;


	screen_width	= 1;
	screen_height	= 1;
	is_windowed	= true;

	current_state	= nullptr;

	current_time	= 0;

	
	
	// seed the random number generator
	srand( (unsigned int)time( nullptr ) );
}

/*************************************************/
// Destructor
CGame::~CGame(void)
{
}


/*************************************************/
// Initialize
//	- setup the SGD wrappers
void CGame::Initialize( HWND hWnd, HINSTANCE hInstance,
					int nWidth, int nHeight,
					bool bIsWindowed )
{
	// Store the SGD Wrapper singletons
	direct_3D			= CSGD_Direct3D::GetInstance();
	direct_input			= CSGD_DirectInput::GetInstance();
	texture_manager			= CSGD_TextureManager::GetInstance();
	x_audio			= CSGD_XAudio2::GetInstance();

	// Store the parameters
	screen_width	= nWidth;
	screen_height	= nHeight;
	is_windowed	= bIsWindowed;


	// Initialize the wrappers
	direct_3D->Initialize( hWnd, screen_width, screen_height, is_windowed, false );
	direct_input->Initialize( hWnd, hInstance, DI_KEYBOARD /*| DI_MOUSE | DI_JOYSTICKS*/ );
	texture_manager->Initialize( direct_3D->GetDirect3DDevice(),	direct_3D->GetSprite() );
	x_audio->Initialize();


	// Initialize the Font
	font = new CBitmapFont;
	font->Initialize();

	// Start in the Main Menu state
	//ChangeState( CMainMenuState::GetInstance() );

	// Store the current time
	current_time	= GetTickCount();

		

}

/*************************************************/
// Main
//	- run one frame of the gameplay
bool CGame::Main( void )
{
	// Check input
	if( Input() == false )
		return false;

	// Update & render
	Update();
	Render();
	return true;
}

/*************************************************/
// Terminate
//	- shutdown the SGD wrappers (in REVERSE ORDER)
void CGame::Terminate( void )
{
	// Exit the current state
	ChangeState( nullptr );

	
	// Shutdown the font
	if( font != nullptr )
	{
		font->Shutdown();
		delete font;
		font = nullptr;
	}
		

	// Shutdown the wrappers
	x_audio->Terminate();
	texture_manager->Terminate();
	direct_input->Terminate();
	direct_3D->Terminate();

	
	// Reset the data members to 'clean' values
	direct_3D			= nullptr;
	direct_input			= nullptr;
	texture_manager			= nullptr;
	x_audio			= nullptr;
	
	screen_width	= 1;
	screen_height	= 1;
	is_windowed	= true;
}


/*************************************************/
// Input
//	- handle user input
bool CGame::Input( void )
{
	// Read the devices
	direct_input->ReadDevices();
	
	if( ((direct_input->KeyDown(DIK_LALT) == true ) || (direct_input->KeyDown(DIK_RALT) == true)) && direct_input->KeyPressed(DIK_RETURN) == true){
		is_windowed = !is_windowed;

		if(is_windowed == true)
			CSGD_Direct3D::GetInstance()->Resize(WINDOWED_WIDTH,WINDOWED_HEIGHT,is_windowed);
		else
			CSGD_Direct3D::GetInstance()->Resize(FULLSCREEN_WIDTH,FULLSCREEN_HEIGHT,is_windowed);

		return true;
	}

	return current_state->Input();	// current state handles input
}

/*************************************************/
// Update
//	- update the game
void CGame::Update( void )
{
	// Update the audio
	x_audio->Update();


	// Calculate the elapsed time between frames
	DWORD dwNow = GetTickCount();
	float elapsed_time = (dwNow - current_time) / 1000.0f;
	current_time = dwNow;


	// Cap the elapsed time to 1/8th
	if( elapsed_time > 0.125f )
		elapsed_time = 0.125f;



	// Current state updates
	current_state->Update( elapsed_time );
}

/*************************************************/
// Render
//	- render the game
void CGame::Render( void )
{
	// Clear the backbuffer
	direct_3D->Clear( D3DCOLOR_XRGB( 50, 50, 50 ) );	// Dark gray

	// Begin rendering
	direct_3D->DeviceBegin();
	direct_3D->SpriteBegin();
	{
		// Current state renders
		current_state->Render();
	}
	direct_3D->SpriteEnd();
	direct_3D->DeviceEnd();

	// Present the backbuffer to the screen
	direct_3D->Present();
}


/*************************************************/
// ChangeState
//	- switch between states
void CGame::ChangeState( IGameState* pState )
{
	// Exit the current state (if any)
	if( current_state != nullptr )
		current_state->Exit();


	// Store the new state
	current_state = pState;


	// Enter the new state (if any)
	if( current_state != nullptr )
		current_state->Enter();
}
