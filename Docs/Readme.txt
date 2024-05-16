# GameFramework---Computer-Graphichs
A Computer Graphics laboratory (second year of Computer Science) project where on the already built MFC GameFramework I had to implement the following tasks:

 Show a second sprite(plane) that explodes on Q.
• Block the possibility of the plane exiting the screen.
• The plane must be able to fire bullets
• Implement collision detection.
• Implement save and load.
• Implement ability to rotate plane.
• Implement lives and score.
• Add incoming crates (which give points and take lives).
• Collision detection plane-crate, bullet-crate.
• Implement moving enemies that randomly bombard the plane (similar to Space Invaders).
• Scrolling background.
• Ability to collect bonus lives/powerup that randomly appear on screen.

I also took my liberty to personalize the game a little bit by adding sound effects and popup notifications for the player state.

!!![THE FRAMEWORK IS NOT MY PROPERTY! IT IS AN OPENSOURCE FRAMEWORK USED IN MY FACULTY WHICH WE USED FOR THE COMPUTER GRAPHICS LABORATORY]!!!

****The implementation is a little bit clanky since I had to draw my resources personally and some features won't be displayed as intended****


![image](https://github.com/09Florin/GameFramework---Computer-Graphichs/assets/92792343/fd41958b-396f-4625-9046-c87e0c88497e)



Tasks implementation:

- Showing the second sprite that explodes on Q

  Adding a new CPlayer class named CPlayer2 which contains the code from CPlayer but changing the starting point coordinates.
  Adding in CGameApp the controls for the second plane (CPlayer2)
  Adding explosion function in the new player class
  
- Block the possibility of the plane exiting the screen

  Adding conditions for the situations when one of the sprites hits the window margin by half a plane:
  	if( ulDirection & CPlayer::DIR_RIGHT )
		m_pSprite->mVelocity.x += .1;
	if (m_pSprite->mPosition.x >= 800 - m_pSprite->width() / 2 - 15)
	{
		m_pSprite->mVelocity.x = 0;
		m_pSprite->mPosition.x--;
}

-  The plane must be able to fire bullets

  Adding a sprite and a class for the bullet.
  Adding controls in CGameApp class to fire the bullets

- Implement collision detection.

  Adding collision functions where I defined the hitbox for planes and bullets:

  bool Collision(Sprite* player1, Sprite* player2) {

	// Hitboxes

	RECT r1;
	r1.left = player2->mPosition.x - player2->width() / 2;
	r1.right = player2->mPosition.x + player2->width() / 2;
	r1.top = player2->mPosition.y - player2->width() / 2;
	r1.bottom = player2->mPosition.y + player2->width() / 2;


	RECT r2;
	r2.left = player1->mPosition.x - player1->width() / 2;
	r2.right = player1->mPosition.x + player1->width() / 2;
	r2.top = player1->mPosition.y - player1->width() / 2;
	r2.bottom = player1->mPosition.y + player1->width() / 2;


	if (r1.right > r2.left && r1.left < r2.right && r1.bottom > r2.top && r1.top < r2.bottom) {
		player1->mPosition.y = -100;
		return true;
	}

	if (r1.left > r2.right && r1.right < r2.left && r1.bottom > r2.top && r1.top < r2.bottom) {
		player1->mPosition.y = -100;
		return true;
	}

	return false;

}


- Implement save and load.
  
  Adding load and save functions for each player and add the save files in the project saving the coordinates of the planes

  
void CGameApp::SavePlayer1()
{
	std::ofstream File;
	File.open("SafeFiles.txt");
	File << m_pPlayer->Position().x << " " << m_pPlayer->Position().y << "\n";
	File.close();

}

void CGameApp::LoadPlayer1()
{
	std::ifstream File;
	File.open("SafeFiles.txt");
	double xcoordinate, ycoordinate;
	File >> xcoordinate >> ycoordinate;
	m_pPlayer->Position() = Vec2(xcoordinate, ycoordinate);
	File.close();


}

- Implement ability to rotate plane

  Adding rotate behavior by simply changing the direction of the sprite.
  Adding controls (key reading) in CGameApp

- Implement lives and score.

  Create sprites for lives and score.
  Create a class for each one. Create functions for dying for lives:
  
void Lives::Dying()
{
	death->mPosition = life->mPosition;
	m_bdeath = true;

}

  Set their constant position
  
- Add incoming crates (which give points and take lives).

  Adding the sprites for the new entities. Create the class for the crates (powerups.h/cpp)
  Make the crates being spawned randomly outside the window top margin by a big distance and move it vertically


-Collision detection plane-crate, bullet-crate

  Simply adding more collision functions in CGameApp


- Implement moving enemies that randomly bombard the plane (similar to Space Invaders).

  Adding the new sprite. Adding a new class. Create a starting position. Moving horizontally.
  Shooting randomly (just after the previous fired bullet already hit the plane or is out of the window bottom margin) by adding EnemyBullet class.

- Scrolling background.

  Adding a new background. Enlarge the image in paint. Move it on y axis. After reaching a certain level, reset it:

  	m_imgBackground.Paint(m_pBBuffer->getDC(), 0, y);
	y += 0.1;
	if (y >= 0)
		y = -300;

 
- Ability to collect bonus lives/powerup that randomly appear on screen.

  Implement collision with powerups and lives. Creating functions accordingly.


  ***The project was developed in 2022, I am sorry for not being able to describe it more accurately since I forgot the way I implemented some features. 
