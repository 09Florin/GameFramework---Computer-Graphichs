#include "Main.h"
#include "Sprite.h"
#include <vector>


class EnemyBullet
{
public:

	EnemyBullet(const BackBuffer* pBackBuffer);
	virtual ~EnemyBullet();
	void					Update(float dt);
	void					Draw();
	Vec2& Position();
	Vec2& Velocity();
	void                    Shoot();
	Sprite* m_pSprite;
	float speed = 100;
	bool fire;
	void Upgrade();




};
