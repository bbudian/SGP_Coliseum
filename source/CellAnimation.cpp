/**************************************************
|	File:		CellAnimation.cpp
|	Author:		Benjamin Budian
|	Course:		SGD
|	Purpose:	Handles Animations for Entities
**************************************************/
#include "CellAnimation.h"

#include "../SGD Wrappers/CSGD_TextureManager.h"
#include "../SGD Wrappers/CSGD_Direct3D.h"
#include "../SGD Wrappers/SGD_String.h"
#include "../SGD Wrappers/CSGD_XAudio2.h"

#include <cassert>

CCellAnimation::CCellAnimation(void)
{
	m_nFrameWidth		 = 0;
	m_nFrameHeight		 = 0;
	m_nNumCols			 = 0;
	m_nStartingCell		 = 0;

	m_nImageID			 = -1;
	m_nCurrFrame		 = 0;

	m_fTimeWaited		 = 0.0f;
	m_fTimePerFrame		 = 0.0f;
	m_fSpeed			 = 0.0f;

	m_bPlaying			 = false;
	m_bLooping			 = false;
}

CCellAnimation::~CCellAnimation(void){}

void CCellAnimation::Initialize( int nFrameWidth, int nFrameHeight, int nNumCols,int nStartingCell,int nEndCell, bool bLooping, TSTRING tsFilePath)
{
	m_nFrameWidth	= nFrameWidth;	
	m_nFrameHeight	= nFrameHeight;	
	m_nNumCols		= nNumCols;		
	m_nStartingCell	= nStartingCell;	
	m_nEndCell		= nStartingCell + nEndCell;

	m_nImageID	= CSGD_TextureManager::GetInstance()->LoadTexture( tsFilePath.c_str() );	

	assert( m_nImageID != -1 && "CCellAnimation::Initialize - image path invalid" );

	m_nCurrFrame	= nStartingCell;

	m_fTimeWaited	= 0.0f;
	m_fTimePerFrame	= 0.06f;
	m_fSpeed		= 1.0f;


	m_bPlaying	= false;
	m_bLooping	= bLooping;	

}

void CCellAnimation::Shutdown(){
	if( m_nImageID != -1){
		CSGD_TextureManager::GetInstance()->UnloadTexture( m_nImageID );
		m_nImageID = -1;
	}
}

void CCellAnimation::Update( float fElapsedTime ){
	if( m_bPlaying == false )
		return;

	m_fTimeWaited += fElapsedTime * m_fSpeed;

	if( m_fTimeWaited >= m_fTimePerFrame ){
		m_fTimeWaited = 0.0f;
		++m_nCurrFrame;
	}

	if( m_nCurrFrame == m_nEndCell){
		if( m_bLooping == true )
			m_nCurrFrame = m_nStartingCell; 
		else{	
			--m_nCurrFrame;
			m_bPlaying = false;
		}
	}
}

void CCellAnimation::Render( int nX, int nY, float fScale, DWORD dwColor ){
	assert( m_nImageID != -1 && "CCellAnimation::Render - Image was not set" );

	RECT rFrame = CellAlgorithm( m_nCurrFrame );
	CSGD_TextureManager::GetInstance()->Draw(m_nImageID, nX, nY, fScale, fScale, &rFrame, 0.0f, 0.0f, 0.0f, dwColor);
}

void CCellAnimation::Play( bool bLoop, float fSpeed ){
	m_bLooping = bLoop;
	m_fSpeed = fSpeed;

	m_nCurrFrame = m_nStartingCell;
	m_fTimeWaited = 0.0f;
	m_bPlaying = true;
}

void CCellAnimation::Pause( bool bPause ){
	m_bPlaying = !bPause;
	m_nCurrFrame = m_nStartingCell;
}

bool CCellAnimation::IsOver( void ) const {
	return (m_bPlaying == false && m_nCurrFrame == m_nEndCell && m_fTimeWaited == 0.0f);
}

RECT CCellAnimation::CellAlgorithm( int id ) const{
	RECT rCell = {};

	rCell.left = (id % m_nNumCols) * m_nFrameWidth;
	rCell.top = (id / m_nNumCols) * m_nFrameHeight;

	rCell.right = rCell.left + m_nFrameWidth;
	rCell.bottom = rCell.top + m_nFrameHeight;

	return rCell;
}

void CCellAnimation::SetAnimation(int nStartingCell,int nNumCols, int nEndCell,bool bLooping){
	m_nStartingCell = nStartingCell;
	m_nCurrFrame = nStartingCell;
	m_nNumCols = nNumCols;
	m_nEndCell = nStartingCell + nEndCell;
	m_bLooping = bLooping;
}

void CCellAnimation::SetResource(TSTRING tsPath){
	Shutdown();

	m_nImageID	= CSGD_TextureManager::GetInstance()->LoadTexture( tsPath.c_str() );		
}






