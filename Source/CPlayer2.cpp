//---------------------------------------------------------------------------- -
// File: CPlayer.cpp
//
// Desc: This file stores the player object class. This class performs tasks
//       such as player movement, some minor physics as well as rendering.
//
// Original design by Adam Hoult & Gary Simmons. Modified by Mihai Popescu.
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// CPlayer Specific Includes
//-----------------------------------------------------------------------------
#include "CPlayer2.h"

//-----------------------------------------------------------------------------
// Name : CPlayer () (Constructor)
// Desc : CPlayer Class Constructor
//-----------------------------------------------------------------------------
CPlayer2::CPlayer2(const BackBuffer * pBackBuffer)
{
	//m_pSprite = new Sprite("data/planeimg.bmp", "data/planemask.bmp");
	m_pSprite2 = new Sprite("data/planeimgandmask.bmp", RGB(0xff, 0x00, 0xff));
	m_pSprite2->setBackBuffer(pBackBuffer);
	m_eSpeedState = SPEED_STOP;
	m_fTimer = 0;



	// Animation frame crop rectangle
	RECT r;
	r.left = 0;
	r.top = 0;
	r.right = 128;
	r.bottom = 128;

	m_pExplosionSprite = new AnimatedSprite("data/explosion.bmp", "data/explosionmask.bmp", r, 4);
	m_pExplosionSprite->setBackBuffer(pBackBuffer);
	m_bExplosion = false;
	m_iExplosionFrame = 0;


}

//-----------------------------------------------------------------------------
// Name : ~CPlayer () (Destructor)
// Desc : CPlayer Class Destructor
//-----------------------------------------------------------------------------
CPlayer2::~CPlayer2()
{
	delete m_pSprite2;
	delete m_pExplosionSprite;
}

void CPlayer2::Update(float dt)
{
	// Update sprite
	m_pSprite2->update(dt);


	// Get velocity
	double v = m_pSprite2->mVelocity.Magnitude();

	// NOTE: for each async sound played Windows creates a thread for you
	// but only one, so you cannot play multiple sounds at once.
	// This creation/destruction of threads also leads to bad performance
	// so this method is not recommanded to be used in complex projects.

	// update internal time counter used in sound handling (not to overlap sounds)
	m_fTimer += dt;

	// A FSM is used for sound manager 
	switch (m_eSpeedState)
	{
	case SPEED_STOP:
		if (v > 35.0f)
		{
			m_eSpeedState = SPEED_START;
			PlaySound("data/jet-start.wav", NULL, SND_FILENAME | SND_ASYNC);
			m_fTimer = 0;
		}
		break;
	case SPEED_START:
		if (v < 25.0f)
		{
			m_eSpeedState = SPEED_STOP;
			PlaySound("data/jet-stop.wav", NULL, SND_FILENAME | SND_ASYNC);
			m_fTimer = 0;
		}
		else
			if (m_fTimer > 1.f)
			{
				PlaySound("data/jet-cabin.wav", NULL, SND_FILENAME | SND_ASYNC);
				m_fTimer = 0;
			}
		break;
	}

	// NOTE: For sound you also can use MIDI but it's Win32 API it is a bit hard
	// see msdn reference: http://msdn.microsoft.com/en-us/library/ms711640.aspx
	// In this case you can use a C++ wrapper for it. See the following article:
	// http://www.codeproject.com/KB/audio-video/midiwrapper.aspx (with code also)
}

void CPlayer2::Draw()
{
	if (!m_bExplosion)
		m_pSprite2->draw();
	else
		m_pExplosionSprite->draw();

}

void CPlayer2::Move(ULONG ulDirection)
{

	if (ulDirection & CPlayer2::DIR_LEFT2)
		m_pSprite2->mVelocity.x -= .1;
	if (m_pSprite2->mPosition.x <= 0 + m_pSprite2->width() / 2)
	{
		m_pSprite2->mVelocity.x = 0;
		m_pSprite2->mPosition.x++;
	}


	if (ulDirection & CPlayer2::DIR_RIGHT2)
		m_pSprite2->mVelocity.x += .1;
	if (m_pSprite2->mPosition.x >= 800 - m_pSprite2->width() / 2 - 15)
	{
		m_pSprite2->mVelocity.x = 0;
		m_pSprite2->mPosition.x--;
	}

	if (ulDirection & CPlayer2::DIR_FORWARD2)
		m_pSprite2->mVelocity.y -= .1;
	if (m_pSprite2->mPosition.y <= 0 + m_pSprite2->height() / 2)
	{
		m_pSprite2->mVelocity.y = 0;
		m_pSprite2->mPosition.y++;
	}

	if (ulDirection & CPlayer2::DIR_BACKWARD2)
		m_pSprite2->mVelocity.y += .1;
	if (m_pSprite2->mPosition.y >= 600 - m_pSprite2->height() / 2 - 35)
	{
		m_pSprite2->mVelocity.y = 0;
		m_pSprite2->mPosition.y--;
	}

}




Vec2& CPlayer2::Position()
{
	return m_pSprite2->mPosition;

}

Vec2& CPlayer2::Velocity()
{
	return m_pSprite2->mVelocity;

}

void CPlayer2::Explode()
{
	m_pExplosionSprite->mPosition = m_pSprite2->mPosition;
	m_pExplosionSprite->SetFrame(0);
	PlaySound("data/explosion.wav", NULL, SND_FILENAME | SND_ASYNC);
	m_bExplosion = true;
	nr--;
}


bool CPlayer2::AdvanceExplosion()
{
	if (m_bExplosion)
	{
		m_pExplosionSprite->SetFrame(m_iExplosionFrame++);
		if (m_iExplosionFrame == m_pExplosionSprite->GetFrameCount())
		{
			m_bExplosion = false;
			m_iExplosionFrame = 0;
			m_pSprite2->mVelocity = Vec2(0, 0);
			m_eSpeedState = SPEED_STOP;
			return false;
		}
	}
	return true;
}

int CPlayer2::Nr() {
	return nr;
}



void CPlayer2::SetDir(int dir)
{
	this->dir = dir;
}

int CPlayer2::GetDir()
{
	return this->dir;
}


void CPlayer2::RotatePlaneRight()
{

	m_pSprite2->SetImage("data/PlaneImgAndMaskRight.bmp", RGB(0xff, 0x00, 0xff));
	dir = 1;
}
void CPlayer2::RotatePlaneDown()
{

	m_pSprite2->SetImage("data/PlaneImgAndMaskDown.bmp", RGB(0xff, 0x00, 0xff));
	dir = 2;
}
void CPlayer2::RotatePlaneLeft()
{
	m_pSprite2->SetImage("data/PlaneImgAndMaskLeft.bmp", RGB(0xff, 0x00, 0xff));

	dir = 3;

}
void CPlayer2::RotatePlaneUp()
{
	m_pSprite2->SetImage("data/PlaneImgAndMask.bmp", RGB(0xff, 0x00, 0xff));

	dir = 0;
}