#include "Enemies.h"


Enemies::Enemies(const BackBuffer* pBackBuffer)
{
	ship = new Sprite("data/ship.bmp", RGB(0xff, 0x00, 0xff));
	ship->setBackBuffer(pBackBuffer);


}

Enemies::~Enemies()
{
	delete ship;
}

void Enemies::Update(float dt)
{
	ship->update(dt);
}

void Enemies::Draw()
{

	ship->draw();
}

Vec2& Enemies::Position()
{
	return ship->mPosition;
}

Vec2& Enemies::Velocity()
{
	return ship->mVelocity;
}



	void Enemies::Move(ULONG ulDirection)
	{
		if (ulDirection & Enemies::DIR_LEFT)
			ship->mVelocity.x -= .1;
		if (ship->mPosition.x <= 0 + ship->width() / 2)
		{
			ship->mVelocity.x = 0.0;

		}

		if (ulDirection & Enemies::DIR_RIGHT)
			ship->mVelocity.x += .1;
		if (ship->mPosition.x >= 800 - ship->width() / 2 - 15)
		{
			ship->mVelocity.x = 0.0;
			
		}

		if (ulDirection & Enemies::DIR_FORWARD)
			ship->mVelocity.y -= .1;
		if (ship->mPosition.y <= 0 + ship->height() / 2)
		{
			ship->mVelocity.y = 0;
			ship->mPosition.y++;
		}

		if (ulDirection & Enemies::DIR_BACKWARD)
			ship->mVelocity.y += .1;
		if (ship->mPosition.y >= 600 - ship->height() / 2 - 35)
		{
			ship->mVelocity.y = 0;
			ship->mPosition.y--;
		}


	}