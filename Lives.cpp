#include "Lives.h"



Lives::Lives(const BackBuffer* pBackBuffer)
{
	life = new Sprite("data/Heart.bmp", RGB(0xff, 0x00, 0xff));
	life->setBackBuffer(pBackBuffer);


	death = new Sprite("data/death.bmp", RGB(0xff, 0x00, 0xff));
	death->setBackBuffer(pBackBuffer);
	m_bdeath = false;

}

Lives::~Lives()
{
	delete life;
	delete death;

}

void Lives::Update(float dt)
{
	life->update(dt);

}

void Lives::Draw()
{
	if (!m_bdeath)
		life->draw();
	else
		death->draw();

}

Vec2& Lives::Position()
{
	return life->mPosition;


}

Vec2& Lives::Velocity()
{
	return life->mVelocity;


}


void Lives::Dying()
{
	death->mPosition = life->mPosition;
	m_bdeath = true;

}


