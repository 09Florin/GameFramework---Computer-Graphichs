#include "Score.h"


Score::Score(const BackBuffer* pBackBuffer)
{
	text = new Sprite("data/Score.bmp", RGB(0xff, 0x00, 0xff));
	text ->setBackBuffer(pBackBuffer);

}

Score::~Score()
{
	delete text;
}

void Score::Update(float dt)
{
	text->update(dt);
}

void Score::Draw()
{

	text->draw();
}

Vec2& Score::Position()
{
	return text->mPosition;
}

Vec2& Score::Velocity()
{
	return text->mVelocity;
}
