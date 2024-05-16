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

I also took my liberty to personalize the game a little bit by adding sound effects and file saving.

!!![THE FRAMEWORK IS NOT MY PROPERTY! IT IS AN OPENSOURCE FRAMEWORK USED IN MY FACULTY WHICH WE USED FOR THE COMPUTER GRAPHICS LABORATORY]!!!

****The implementation is a little bit clanky since I had to draw my resources personally and some features won't be displayed as intended****


![image](https://github.com/09Florin/GameFramework---Computer-Graphichs/assets/92792343/fd41958b-396f-4625-9046-c87e0c88497e)

Tasks implementation:

- Showing the second sprite that explodes on Q

  Adding a new CPlayer class named CPlayer2 which contains the code from CPlayer but changing the starting point coordinates.
  Adding in CGameApp the controls for the second plane (CPlayer2)
  
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

  

 
