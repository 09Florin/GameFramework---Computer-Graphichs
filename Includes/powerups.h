#include "Main.h"
#include "Sprite.h"


class powerups
{
public:

	powerups(const BackBuffer* pBackBuffer);
	virtual ~powerups();
	void					Update(float dt);
	void					Draw();
	Vec2& Position();
	Vec2& Velocity();
	void                    Shoot();
	Sprite* m_pSprite;



};
