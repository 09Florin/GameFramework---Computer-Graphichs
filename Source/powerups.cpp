#include "powerups.h"


powerups::powerups(const BackBuffer* pBackBuffer)
{
	m_pSprite = new Sprite("data/powerup.bmp", RGB(0xff, 0x00, 0xff));
	m_pSprite->setBackBuffer(pBackBuffer);
	m_pSprite->mVelocity = Vec2(0.0, 100.0);

}

powerups::~powerups()
{
	delete m_pSprite;
}

void powerups::Update(float dt)
{
	m_pSprite->update(dt);
}

void powerups::Draw()
{

	m_pSprite->draw();
}

Vec2& powerups::Position()
{
	return m_pSprite->mPosition;
}

Vec2& powerups::Velocity()
{
	return m_pSprite->mVelocity;
}