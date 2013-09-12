/**************************************************
|	File:		BitmapFont.cpp
|	Author:		Benjamin Budian
|	Course:		SGD
|	Purpose:	CBitmapFont class uses a texture of 
|				symbols to draw text to the screen.
**************************************************/


#include "BitmapFont.h"

#include "../SGD Wrappers/CSGD_TextureManager.h"
#include "../SGD Wrappers/SGD_String.h"
#include <cassert>


/*************************************************/
// Constructor
CBitmapFont::CBitmapFont(void)
{
	// Set the data members to 'clean' values
	m_nImageID		= -1;
	
	m_nCharWidth	= 0;
	m_nCharHeight	= 0;
	m_nNumCols		= 0;
	m_nNumRows		= 0; 
		
	m_cFirstChar	= 0;
	m_bOnlyUppercase= false;
}

/*************************************************/
// Destructor
CBitmapFont::~CBitmapFont(void)
{
}


/*************************************************/
// Initialize
//	- setup the font to use the SGD_Font_Glow.png
void CBitmapFont::Initialize( void )
{
	m_nImageID		= CSGD_TextureManager::GetInstance()->LoadTexture(_T("resource/graphics/bb_franklin_bitmap.png") );
	
	m_nCharWidth	= 32;
	m_nCharHeight	= 32;
	m_nNumCols		= 10;
	m_nNumRows		= 6;
		
	m_cFirstChar	= _T(' ');		// image starts with a space
	m_bOnlyUppercase= true;
}

/*************************************************/
// Shutdown
//	- unload resources
void CBitmapFont::Shutdown( void )
{
	CSGD_TextureManager::GetInstance()->UnloadTexture( m_nImageID );
	m_nImageID = -1;
}


/*************************************************/
// Draw
//	- draw the text one character at a time
void CBitmapFont::Draw( const TCHAR* strOutput, int nX, int nY,
						float fScale, DWORD dwColor ) const
{
	// Validate the image
	assert( m_nImageID != -1 && "CBitmapFont::Draw - image is not set" );


	// Store the starting x position
	int nColStart = nX;



	// Loop through each character in the string
	for( int i = 0; strOutput[ i ]; i++ )
	{
		// Get the character out of the string
		TCHAR ch = strOutput[ i ];


		// Check for whitespace
		if( ch == _T(' ') )
		{
			// Move to the next spot
			nX += (int)(m_nCharWidth * fScale);
			continue;
		}
		else if( ch == _T('\n') )
		{
			// Move to the next row, and reset the x
			nY += (int)(m_nCharHeight * fScale);
			nX =  nColStart;
			continue;
		}
		else if( ch == _T('\t') )
		{
			// Calculate how many characters are already printed
			int diff = (int)((nX - nColStart) / (m_nCharWidth * fScale));

			// Calculate the number of spaces to add to align at every 4 spaces
			int tab  = 4 - (diff % 4);

			nX += (int)(tab * m_nCharWidth * fScale);
			continue;
		}


		// Convert to uppercase?
		if( m_bOnlyUppercase == true )
			ch = TCTOUPPER( ch );


		// Calculate the cell ID for this character
		int id = ch - m_cFirstChar;
		RECT rCell = CellAlgorithm( id );

		// Draw the character
		CSGD_TextureManager::GetInstance()->Draw(
			m_nImageID, nX, nY, 
			fScale, fScale, &rCell,
			0.0f, 0.0f, 0.0f, dwColor );

		// Move the x for the next character
		nX += (int)(m_nCharWidth * fScale);
	}
}


/*************************************************/
// CellAlgorithm
//	- return the source rect in the image for the
//	  symbol with the given id (0-based index)
RECT CBitmapFont::CellAlgorithm( int id ) const
{
	RECT rChar = { };

	rChar.left		= (id % m_nNumCols) * m_nCharWidth;
	rChar.top		= (id / m_nNumCols) * m_nCharHeight;

	rChar.right		= rChar.left + m_nCharWidth;
	rChar.bottom	= rChar.top  + m_nCharHeight;

	return rChar;
}
