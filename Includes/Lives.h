#include "Main.h"
#include "Sprite.h"
class Lives
{
public:

	Lives(const BackBuffer* pBackBuffer);
	virtual ~Lives();
	void					Update(float dt);
	void					Draw();
	void                    Dying();
	Vec2& Position();
	Vec2& Velocity();
	Sprite* life;
	Sprite* extralife;
	Sprite* death;
	bool					m_bdeath;
	float speed = 100;

};
