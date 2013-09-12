/**************************************************
|	File:		BitmapFont.h
|	Author:		Benjamin Budian
|	Course:		SGD
|	Purpose:	CBitmapFont class uses a texture of
|				symbols to draw text to the screen.
**************************************************/

#ifndef BITMAPFONT_H
#define BITMAPFONT_H


#define WIN32_LEAN_AND_MEAN
#include <Windows.h>


/*************************************************/
// CBitmapFont class
//	- uses CellAlgorithm to calculate the source rect of each character
//	- RESTRICTIONS:
//		- image size MUST be a power of 2: 2,4,8,16,32,etc
//		- characters MUST have a fixed size (clean rows & columns)
//		- MUST be in ASCII order
//		- could be missing symbols from the front or end
//		- symbols missing in the middle MUST have a space reserved
class CBitmapFont
{
public:
	///////////////////////////////////////////////
	CBitmapFont	(void);
	~CBitmapFont(void);
	
	///////////////////////////////////////////////
	// Init & Shutdown
	void Initialize	( void );	// should have parameters
	void Shutdown	( void );
	
	///////////////////////////////////////////////
	// Draw Text
	void Draw(	const TCHAR* strOutput, int nX, int nY,
				float fScale, DWORD dwColor ) const;

private:
	///////////////////////////////////////////////
	// Cell Algorithm helper method
	RECT CellAlgorithm( int id ) const;

	
	///////////////////////////////////////////////
	// image
	int			m_nImageID;

	// cell algorithm
	int			m_nCharWidth;
	int			m_nCharHeight;
	int			m_nNumCols;
	int			m_nNumRows;

	// font info
	TCHAR		m_cFirstChar;
	bool		m_bOnlyUppercase;
};

#endif //BITMAPFONT_H
