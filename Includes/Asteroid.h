#include "Main.h"
#include "Sprite.h"


class Asteroid
{
public:

	Asteroid(const BackBuffer* pBackBuffer);
	virtual ~Asteroid();
	void					Update(float dt);
	void					Draw();
	Vec2& Position();
	Vec2& Velocity();
	void                    Shoot();
	Sprite* m_pSprite;



};
