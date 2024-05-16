#include "Main.h"
#include "Sprite.h"
class Score
{
public:

	Score(const BackBuffer* pBackBuffer);
	virtual ~Score();
	void					Update(float dt);
	void					Draw();
	Vec2& Position();
	Vec2& Velocity();
	Sprite* text;
};
