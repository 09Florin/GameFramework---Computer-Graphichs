//-----------------------------------------------------------------------------
// File: CPlayer.cpp
//
// Desc: This file stores the player object class. This class performs tasks
//	   such as player movement, some minor physics as well as rendering.
//
// Original design by Adam Hoult & Gary Simmons. Modified by Mihai Popescu.
//-----------------------------------------------------------------------------

#ifndef _CPLAYER2_H_
#define _CPLAYER2_H_

//-----------------------------------------------------------------------------
// CPlayer Specific Includes
//-----------------------------------------------------------------------------
#include "Main.h"
#include "Sprite.h"
#include <chrono>
#include <thread>


//-----------------------------------------------------------------------------
// Main Class Definitions
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// Name : CPlayer (Class)
// Desc : Player class handles all player manipulation, update and management.
//-----------------------------------------------------------------------------
class CPlayer2
{
public:
	//-------------------------------------------------------------------------
	// Enumerators
	//-------------------------------------------------------------------------
	enum DIRECTION
	{
		DIR_FORWARD2 = 1,
		DIR_BACKWARD2 = 2,
		DIR_LEFT2 = 4,
		DIR_RIGHT2 = 8,
	};


	enum ESpeedStates
	{
		SPEED_START,
		SPEED_STOP
	};

	//-------------------------------------------------------------------------
	// Constructors & Destructors for This Class.
	//-------------------------------------------------------------------------
	CPlayer2(const BackBuffer* pBackBuffer);
	virtual ~CPlayer2();

	//-------------------------------------------------------------------------
	// Public Functions for This Class.
	//-------------------------------------------------------------------------
	void					Update(float dt);
	void					Draw();
	void					Move(ULONG ulDirection);
	Vec2& Position();
	Vec2& Velocity();
	void					Explode();
	bool					AdvanceExplosion();
	void                    RotatePlaneLeft();
	void                    RotatePlaneRight();
	void                    RotatePlaneUp();
	void                    RotatePlaneDown();
	void                    SetImage(const BackBuffer* pBackBuffer);
	int                     Nr();
	void SetDir(int dir);
	int GetDir();
	Sprite* m_pSprite2;

private:
	//-------------------------------------------------------------------------
	// Private Variables for This Class.
	//-------------------------------------------------------------------------
	
	ESpeedStates			m_eSpeedState;
	float					m_fTimer;
	bool					m_bExplosion;
	AnimatedSprite* m_pExplosionSprite;
	int						m_iExplosionFrame;
	int nr = 3;
	int dir = 0;



};

#endif // _CPLAYER2_H_