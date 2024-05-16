#include "Asteroid.h"


Asteroid::Asteroid(const BackBuffer* pBackBuffer)
{
	m_pSprite = new Sprite("data/Asteroid.bmp", RGB(0xff, 0x00, 0xff));
	m_pSprite->setBackBuffer(pBackBuffer);
	m_pSprite->mVelocity = Vec2(0.0, 100.0);

}

Asteroid::~Asteroid()
{
	delete m_pSprite;
}

void Asteroid::Update(float dt)
{
	m_pSprite->update(dt);
}

void Asteroid::Draw()
{

	m_pSprite->draw();
}

Vec2& Asteroid::Position()
{
	return m_pSprite->mPosition;
}

Vec2& Asteroid::Velocity()
{
	return m_pSprite->mVelocity;
}