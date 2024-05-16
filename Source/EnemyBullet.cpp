#include "EnemyBullet.h"


EnemyBullet::EnemyBullet(const BackBuffer* pBackBuffer)
{
	m_pSprite = new Sprite("data/EnemyBullet.bmp", RGB(0xff, 0x00, 0xff));
	m_pSprite->setBackBuffer(pBackBuffer);
	m_pSprite->mVelocity = Vec2(0.0, 100.0);
	fire = false;
}

EnemyBullet::~EnemyBullet()
{
	delete m_pSprite;
}

void EnemyBullet::Update(float dt)
{
	m_pSprite->update(dt);
}

void EnemyBullet::Draw()
{

	m_pSprite->draw();
}

Vec2& EnemyBullet::Position()
{
	return m_pSprite->mPosition;
}

Vec2& EnemyBullet::Velocity()
{
	return m_pSprite->mVelocity;
}
