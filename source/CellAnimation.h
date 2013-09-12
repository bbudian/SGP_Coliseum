/**************************************************
|	File:		CellAnimation.h
|	Author:		Benjamin Budian
|	Course:		SGP
|	Purpose:	Handles Animations for Entities
|
|	Note:		May need to be redesigned
**************************************************/
#pragma once

#define WIND32_LEAN_AND_MEAN
#include <Windows.h>

#include <string>
using std::string;

#include "../SGD Wrappers/SGD_String.h"

class CCellAnimation
{
public:
	CCellAnimation(void);
	~CCellAnimation(void);

	void Initialize(int nFrameWidth, int nFrameHeight, int nNumCols,int nStartingCell, int nEndCell, bool bLooping, TSTRING szFilePath);

	void Shutdown();

	void Update( float fElapsedTime );
	void Render( int nX, int nY, float fScale, DWORD dwColor );

	void Play( bool bLoop, float fSpeed );
	void Pause( bool bPause );

	void SetAnimation(int nStartingCell,int nNumCols, int nEndCell,bool bLooping);

	void SetResource(TSTRING tsPath);
	void SetWaitTime(float fWaitTime) { m_fSpeed = fWaitTime; }

	bool IsOver( void ) const;

	bool IsPlaying( void ) const { return m_bPlaying; }

	int GetCurrentCell() { return m_nCurrFrame; }
	float GetTimeWaited() { return m_fTimeWaited; }


private:

	RECT CellAlgorithm( int nID ) const;

	int m_nFrameWidth;
	int m_nFrameHeight;
	int m_nNumCols;
	int m_nStartingCell;

	int m_nImageID;

	int m_nCurrFrame;
	//int m_nNumFrames;
	int m_nEndCell;

	float m_fTimeWaited;
	float m_fTimePerFrame;
	float m_fSpeed;

	bool m_bPlaying;
	bool m_bLooping;


};

