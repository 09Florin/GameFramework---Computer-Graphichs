#include "Bullet.h"


Bullet::Bullet(const BackBuffer* pBackBuffer)
{
	m_pSprite = new Sprite("data/Bullet.bmp", RGB(0xff, 0x00, 0xff));
	m_pSprite->setBackBuffer(pBackBuffer);
	m_pSprite->mVelocity = Vec2(0.0, -100.0);
	fire = false;
}

Bullet::~Bullet()
{
	delete m_pSprite;
}

void Bullet::Update(float dt)
{
	m_pSprite->update(dt);
}

void Bullet::Draw()
{
	
	m_pSprite->draw();
}

Vec2& Bullet::Position()
{
	return m_pSprite->mPosition;
}

Vec2& Bullet::Velocity()
{
	return m_pSprite->mVelocity;
}

// No use yet
void Bullet::Shoot()
{

	std::vector <int> bullets;
	bullets.push_back(/*m_pSprite*/5);

	PlaySound("data/Shoot sfx.wav", NULL, SND_FILENAME | SND_ASYNC);
	fire = true;


	
}


void Bullet::Upgrade() {

	m_pSprite->SetImage("data/Bulletupdate.bmp", RGB(0xff, 0x00, 0xff));
	
}


