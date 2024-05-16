#include "Main.h"
#include "Sprite.h"


class Enemies
{
public:
	


	enum DIRECTION
	{
		DIR_FORWARD = 1,
		DIR_BACKWARD = 2,
		DIR_LEFT = 4,
		DIR_RIGHT = 8,
	};





	Enemies(const BackBuffer* pBackBuffer);
	virtual ~Enemies();
	void					Update(float dt);
	void					Draw();
	Vec2& Position();
	Vec2& Velocity();
	void Enemies::Move(ULONG ulDirection);
	void                    Shoot();
	Sprite* ship;



};
