//-----------------------------------------------------------------------------
// File: CGameApp.cpp
//
// Desc: Game Application class, this is the central hub for all app processing
//
// Original design by Adam Hoult & Gary Simmons. Modified by Mihai Popescu.
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// CGameApp Specific Includes
//-----------------------------------------------------------------------------
#include "CGameApp.h"

extern HINSTANCE g_hInst;

//-----------------------------------------------------------------------------
// CGameApp Member Functions
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// Name : CGameApp () (Constructor)
// Desc : CGameApp Class Constructor
//-----------------------------------------------------------------------------
CGameApp::CGameApp()
{
	// Reset / Clear all required values
	m_hWnd			= NULL;
	m_hIcon			= NULL;
	m_hMenu			= NULL;
	m_pBBuffer		= NULL;
	m_pPlayer		= NULL;
	m_pPlayer2      = NULL;
	bullet          = NULL;
	heart11         = NULL; // Last life of player 1
	heart12         = NULL;
	heart13         = NULL;
	heart21         = NULL; // Last life of player 2
	heart22         = NULL;
	heart23         = NULL;
	score1          = NULL;
	score2          = NULL;
	asteroid        = NULL;
	battleship      = NULL;
	powerup         = NULL;
	ebullet         = NULL;
	m_LastFrameRate = 0;
}

//-----------------------------------------------------------------------------
// Name : ~CGameApp () (Destructor)
// Desc : CGameApp Class Destructor
//-----------------------------------------------------------------------------
CGameApp::~CGameApp()
{
	// Shut the engine down
	ShutDown();
}

//-----------------------------------------------------------------------------
// Name : InitInstance ()
// Desc : Initialises the entire Engine here.
//-----------------------------------------------------------------------------
bool CGameApp::InitInstance( LPCTSTR lpCmdLine, int iCmdShow )
{
	// Create the primary display device
	if (!CreateDisplay()) { ShutDown(); return false; }

	// Build Objects
	if (!BuildObjects()) 
	{ 
		MessageBox( 0, _T("Failed to initialize properly. Reinstalling the application may solve this problem.\nIf the problem persists, please contact technical support."), _T("Fatal Error"), MB_OK | MB_ICONSTOP);
		ShutDown(); 
		return false; 
	}

	// Set up all required game states
	SetupGameState();

	// Success!
	return true;
}

//-----------------------------------------------------------------------------
// Name : CreateDisplay ()
// Desc : Create the display windows, devices etc, ready for rendering.
//-----------------------------------------------------------------------------
bool CGameApp::CreateDisplay()
{
	LPTSTR			WindowTitle		= _T("GameFramework");
	LPCSTR			WindowClass		= _T("GameFramework_Class");
	USHORT			Width			= 800;
	USHORT			Height			= 600;
	RECT			rc;
	WNDCLASSEX		wcex;


	wcex.cbSize			= sizeof(WNDCLASSEX);
	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= CGameApp::StaticWndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= g_hInst;
	wcex.hIcon			= LoadIcon(g_hInst, MAKEINTRESOURCE(IDI_ICON));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= 0;
	wcex.lpszClassName	= WindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_ICON));

	if(RegisterClassEx(&wcex)==0)
		return false;

	// Retrieve the final client size of the window
	::GetClientRect( m_hWnd, &rc );
	m_nViewX		= rc.left;
	m_nViewY		= rc.top;
	m_nViewWidth	= rc.right - rc.left;
	m_nViewHeight	= rc.bottom - rc.top;

	m_hWnd = CreateWindow(WindowClass, WindowTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, Width, Height, NULL, NULL, g_hInst, this);

	if (!m_hWnd)
		return false;

	// Show the window
	ShowWindow(m_hWnd, SW_SHOW);

	// Success!!
	return true;
}

//-----------------------------------------------------------------------------
// Name : BeginGame ()
// Desc : Signals the beginning of the physical post-initialisation stage.
//		From here on, the game engine has control over processing.
//-----------------------------------------------------------------------------
int CGameApp::BeginGame()
{
	MSG		msg;

	// Start main loop
	while(true) 
	{
		// Did we recieve a message, or are we idling ?
		if ( PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) ) 
		{
			if (msg.message == WM_QUIT) break;
			TranslateMessage( &msg );
			DispatchMessage ( &msg );
		} 
		else 
		{
			// Advance Game Frame.
			FrameAdvance();

		} // End If messages waiting
	
	} // Until quit message is receieved

	return 0;
}

//-----------------------------------------------------------------------------
// Name : ShutDown ()
// Desc : Shuts down the game engine, and frees up all resources.
//-----------------------------------------------------------------------------
bool CGameApp::ShutDown()
{
	// Release any previously built objects
	ReleaseObjects ( );
	
	// Destroy menu, it may not be attached
	if ( m_hMenu ) DestroyMenu( m_hMenu );
	m_hMenu		 = NULL;

	// Destroy the render window
	SetMenu( m_hWnd, NULL );
	if ( m_hWnd ) DestroyWindow( m_hWnd );
	m_hWnd		  = NULL;
	
	// Shutdown Success
	return true;
}

//-----------------------------------------------------------------------------
// Name : StaticWndProc () (Static Callback)
// Desc : This is the main messge pump for ALL display devices, it captures
//		the appropriate messages, and routes them through to the application
//		class for which it was intended, therefore giving full class access.
// Note : It is VITALLY important that you should pass your 'this' pointer to
//		the lpParam parameter of the CreateWindow function if you wish to be
//		able to pass messages back to that app object.
//-----------------------------------------------------------------------------
LRESULT CALLBACK CGameApp::StaticWndProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	// If this is a create message, trap the 'this' pointer passed in and store it within the window.
	if ( Message == WM_CREATE ) SetWindowLong( hWnd, GWL_USERDATA, (LONG)((CREATESTRUCT FAR *)lParam)->lpCreateParams);

	// Obtain the correct destination for this message
	CGameApp *Destination = (CGameApp*)GetWindowLong( hWnd, GWL_USERDATA );
	
	// If the hWnd has a related class, pass it through
	if (Destination) return Destination->DisplayWndProc( hWnd, Message, wParam, lParam );
	
	// No destination found, defer to system...
	return DefWindowProc( hWnd, Message, wParam, lParam );
}

//-----------------------------------------------------------------------------
// Name : DisplayWndProc ()
// Desc : The display devices internal WndProc function. All messages being
//		passed to this function are relative to the window it owns.
//-----------------------------------------------------------------------------
LRESULT CGameApp::DisplayWndProc( HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam )
{
	static UINT			fTimer;	
	int nr1;
	int nr2;

	// Determine message type
	switch (Message)
	{
		case WM_CREATE:
			break;
		
		case WM_CLOSE:
			PostQuitMessage(0);
			break;
		
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		
		case WM_SIZE:
			if ( wParam == SIZE_MINIMIZED )
			{
				// App is inactive
				m_bActive = false;
			
			} // App has been minimized
			else
			{
				// App is active
				m_bActive = true;

				// Store new viewport sizes
				m_nViewWidth  = LOWORD( lParam );
				m_nViewHeight = HIWORD( lParam );
		
			
			} // End if !Minimized

			break;

		case WM_LBUTTONDOWN:
			// Capture the mouse
			SetCapture( m_hWnd );
			GetCursorPos( &m_OldCursorPos );
			break;

		case WM_LBUTTONUP:
			// Release the mouse
			ReleaseCapture( );
			break;

		case WM_KEYDOWN:
			switch(wParam)
			{
			case VK_ESCAPE:
				PostQuitMessage(0);
				break;
			case VK_RETURN:
				fTimer = SetTimer(m_hWnd, 1, 250, NULL);
				nr1 = m_pPlayer->Nr();
				m_pPlayer->Explode();
				if (nr1 == 3) {
					heart13->Dying();
				}
				else if (nr1 == 2) {
					heart12->Dying();
				}
				else if (nr1 == 1) {
					heart11->Dying();
				}
				else if (nr1 <= 0) {
					PlaySound("data/Game Over - Sound Effect [HD].wav", NULL, SND_FILENAME | SND_ASYNC);
					MessageBox(hWnd, "Player 1 has been eliminated!", "Game Over!", MB_OK);
				}
				break;
			case 0x51: //second case//Q
				fTimer = SetTimer(m_hWnd, 2, 250, NULL);
				nr2 = m_pPlayer2->Nr();
				m_pPlayer2->Explode();
				if (nr2 == 3) {
					heart23->Dying();
				}
				else if (nr2 == 2) {
					heart22->Dying();
				}
				else if (nr2 == 1) {
					heart21->Dying();
				}
				else if (nr2 <= 0) {
					PlaySound("data/Game Over - Sound Effect [HD].wav", NULL, SND_FILENAME | SND_ASYNC);
					MessageBox(hWnd, "Player 2 has been eliminated!", "Game Over!", MB_OK);
				}
				break;
			case VK_OEM_MINUS: // Player 1
				PlaySound("data/GTASaveGameSFX.wav", NULL, SND_FILENAME | SND_ASYNC);
				MessageBox(hWnd, "The game has been saved for Player 1", "Save Game", MB_OK);
				SavePlayer1();
				break;
			case VK_OEM_PLUS: // Player 1
				PlaySound("data/LoadGame.wav", NULL, SND_FILENAME | SND_ASYNC);
				MessageBox(hWnd, "There you go again, Player 1", "Load Game", MB_OK);
				LoadPlayer1();
				PlaySound("data/GTALoadGameSFX.wav", NULL, SND_FILENAME | SND_ASYNC);
				break;
			case VK_F1: // Player 2
				PlaySound("data/GTASaveGameSFX.wav", NULL, SND_FILENAME | SND_ASYNC);
				MessageBox(hWnd, "The game has been saved for Player 2", "Save Game", MB_OK);
				SavePlayer2();
				break;
			case VK_F5: // Player 2
				PlaySound("data/LoadGame.wav", NULL, SND_FILENAME | SND_ASYNC);
				MessageBox(hWnd, "There you go again, Player 2", "Load Game", MB_OK);
				LoadPlayer2();
				PlaySound("data/GTALoadGameSFX.wav", NULL, SND_FILENAME | SND_ASYNC);
				break;

			case VK_NUMPAD4: // Player 1

				if (m_pPlayer->GetDir() == 0)
					m_pPlayer->RotatePlaneRight();

				else if (m_pPlayer->GetDir() == 1)
					m_pPlayer->RotatePlaneDown();

				else if (m_pPlayer->GetDir() == 2)
					m_pPlayer->RotatePlaneLeft();

				else if (m_pPlayer->GetDir() == 3)
					m_pPlayer->RotatePlaneUp();
				else
					break;
			case 0x5A: 
				//m_imgBackground.
				
				break;

			case VK_SPACE:
				bullet->Position() = Vec2(m_pPlayer2->Position().x, m_pPlayer2->Position().y-90);
				bullet->Update(m_Timer.GetTimeElapsed());
				break;


			case VK_TAB: // Player 2

				if (m_pPlayer2->GetDir() == 0)
					m_pPlayer2->RotatePlaneRight();

				else if (m_pPlayer2->GetDir() == 1)
					m_pPlayer2->RotatePlaneDown();

				else if (m_pPlayer2->GetDir() == 2)
					m_pPlayer2->RotatePlaneLeft();

				else if (m_pPlayer2->GetDir() == 3)
					m_pPlayer2->RotatePlaneUp();
				else
					break;
				break;
			}
			break;
			

		case WM_TIMER:
			switch(wParam)
			{
			case 1:
				if(!m_pPlayer->AdvanceExplosion())
					KillTimer(m_hWnd, 1);
				break;

			case 2:
				if (!m_pPlayer2->AdvanceExplosion())
					KillTimer(m_hWnd, 2);
				break;


			}
			break;

		case WM_COMMAND:
			break;

		default:
			return DefWindowProc(hWnd, Message, wParam, lParam);

	} // End Message Switch
	
	return 0;
}

//-----------------------------------------------------------------------------
// Name : BuildObjects ()
// Desc : Build our demonstration meshes, and the objects that instance them
//-----------------------------------------------------------------------------
bool CGameApp::BuildObjects()
{
	m_pBBuffer = new BackBuffer(m_hWnd, m_nViewWidth, m_nViewHeight);
	m_pPlayer = new CPlayer(m_pBBuffer);

	
	m_pPlayer2 = new CPlayer2(m_pBBuffer);

	bullet = new Bullet(m_pBBuffer);

	heart11 = new Lives(m_pBBuffer);
	heart12 = new Lives(m_pBBuffer);
	heart13 = new Lives(m_pBBuffer);

	heart21 = new Lives(m_pBBuffer);
	heart22 = new Lives(m_pBBuffer);
	heart23 = new Lives(m_pBBuffer);

	score1 = new Score(m_pBBuffer);
	score2 = new Score(m_pBBuffer);

	asteroid = new Asteroid(m_pBBuffer); 
	battleship = new Enemies(m_pBBuffer);

	powerup = new powerups(m_pBBuffer);

	ebullet = new EnemyBullet(m_pBBuffer);



	if(!m_imgBackground.LoadBitmapFromFile("data/Background.bmp", GetDC(m_hWnd)))
		return false;

	// Success!
	return true;
}

//-----------------------------------------------------------------------------
// Name : SetupGameState ()
// Desc : Sets up all the initial states required by the game.
//-----------------------------------------------------------------------------
void CGameApp::SetupGameState()
{
	m_pPlayer->Position() = Vec2(730, 400);
	m_pPlayer2->Position() = Vec2(0, 400);

	bullet->Position() = Vec2(m_pPlayer2->Position().x+50, m_pPlayer2->Position().y);

	heart11->Position() = Vec2(770, 20);
	heart12->Position() = Vec2(745, 20);
	heart13->Position() = Vec2(720, 20);

	heart21->Position() = Vec2(13, 20);
	heart22->Position() = Vec2(38, 20);
	heart23->Position() = Vec2(63, 20);

	score1->Position() = Vec2(720, 55);
	score2->Position() = Vec2(45, 55);

	srand(991*time(NULL));
	j = rand() % 601;
    asteroid->Position() = Vec2(j, -300);
	battleship->Position() = Vec2(120, 140);
	powerup->Position() = Vec2(j,-920);
	ebullet->Position() = Vec2(battleship->Position().x, battleship->Position().y + 50);

}

//-----------------------------------------------------------------------------
// Name : ReleaseObjects ()
// Desc : Releases our objects and their associated memory so that we can
//		rebuild them, if required, during our applications life-time.
//-----------------------------------------------------------------------------
void CGameApp::ReleaseObjects( )
{
	if(m_pPlayer != NULL)
	{
		delete m_pPlayer;
		m_pPlayer = NULL;
	}

	if (m_pPlayer2 != NULL)
	{
		delete m_pPlayer2;
		m_pPlayer2 = NULL;
	}

	if(m_pBBuffer != NULL)
	{
		delete m_pBBuffer;
		m_pBBuffer = NULL;
	}

	if (bullet != NULL)
	{
		delete bullet;
		bullet = NULL;
	}

	if (heart11 != NULL)
	{
		delete heart11;
		heart11 = NULL;
	}

	if (heart12 != NULL)
	{
		delete heart12;
		heart12 = NULL;
	}

	if (heart13 != NULL)
	{
		delete heart13;
		heart13 = NULL;
	}

	if (heart21 != NULL)
	{
		delete heart21;
		heart21 = NULL;
	}

	if (heart22 != NULL)
	{
		delete heart22;
		heart22 = NULL;
	}

	if (heart23 != NULL)
	{
		delete heart23;
		heart23 = NULL;
	}


	if (score1 != NULL)
	{
		delete score1;
		score1 = NULL;
	}

	if (score2 != NULL)
	{
		delete score2;
		score2 = NULL;
	}

	
	if (asteroid != NULL)
	{
		delete asteroid;
		asteroid = NULL;
	}

	if (asteroid != NULL)
	{
		delete asteroid;
		asteroid = NULL;
	}


	if (battleship != NULL)
	{
		delete battleship;
		battleship = NULL;
	}


	if (powerup != NULL)
	{
		delete powerup;
		powerup = NULL;
	}

	if (ebullet != NULL)
	{
		delete ebullet;
		ebullet = NULL;
	}

}

//-----------------------------------------------------------------------------
// Name : FrameAdvance () (Private)
// Desc : Called to signal that we are now rendering the next frame.
//-----------------------------------------------------------------------------
void CGameApp::FrameAdvance()
{
	static TCHAR FrameRate[ 50 ];
	static TCHAR TitleBuffer[ 255 ];

	// Advance the timer
	m_Timer.Tick( );

	// Skip if app is inactive
	if ( !m_bActive ) return;
	
	// Get / Display the framerate
	if ( m_LastFrameRate != m_Timer.GetFrameRate() )
	{
		m_LastFrameRate = m_Timer.GetFrameRate( FrameRate, 50 );
		sprintf_s( TitleBuffer, _T("Game : %s"), FrameRate );
		SetWindowText( m_hWnd, TitleBuffer );

	} // End if Frame Rate Altered

	// Poll & Process input devices
	ProcessInput();

	// Animate the game objects
	AnimateObjects();

	// Drawing the game objects
	DrawObjects();
}

//-----------------------------------------------------------------------------
// Name : ProcessInput () (Private)
// Desc : Simply polls the input devices and performs basic input operations
//-----------------------------------------------------------------------------
void CGameApp::ProcessInput( )
{
	static UCHAR pKeyBuffer[ 256 ];
	ULONG		Direction = 0;
	ULONG		Direction2= 0;
	ULONG		DirectionRight = 8;
	ULONG		DirectionLeft = 4;
	POINT		CursorPos;
	float		X = 0.0f, Y = 0.0f;

	// Retrieve keyboard state
	if ( !GetKeyboardState( pKeyBuffer ) ) return;

	// Check the relevant keys
	if ( pKeyBuffer[ VK_UP	  ] & 0xF0 ) Direction |= CPlayer::DIR_FORWARD;
	if ( pKeyBuffer[ VK_DOWN  ] & 0xF0 ) Direction |= CPlayer::DIR_BACKWARD;
	if ( pKeyBuffer[ VK_LEFT  ] & 0xF0 ) Direction |= CPlayer::DIR_LEFT;
	if ( pKeyBuffer[ VK_RIGHT ] & 0xF0 ) Direction |= CPlayer::DIR_RIGHT;

	m_pPlayer->Move(Direction);

	if (pKeyBuffer[0x57] & 0xF0) Direction2|= CPlayer2::DIR_FORWARD2;
	if (pKeyBuffer[0x53] & 0xF0) Direction2|= CPlayer2::DIR_BACKWARD2;
	if (pKeyBuffer[0x41] & 0xF0) Direction2|= CPlayer2::DIR_LEFT2;
	if (pKeyBuffer[0x44] & 0xF0) Direction2|= CPlayer2::DIR_RIGHT2;

	m_pPlayer2->Move(Direction2);
	
	if (asteroid->Position().y > 620) {
		srand(1777 * time(NULL));
		j = rand() % 601;
		asteroid->Position() = Vec2(j, -300);
	}

	if (powerup->Position().y > 620) {
		srand(1777 * time(NULL));
		j = rand() % 601;
		powerup->Position() = Vec2(j, -1500);
		bullet->Upgrade();
		bullet->Update(m_Timer.GetTimeElapsed());

	}

	if (ebullet->Position().y > 620) {
		ebullet->Position() = Vec2(battleship->Position().x, battleship->Position().y + 50);
		ebullet->Update(m_Timer.GetTimeElapsed());
	}
	

	if (m_pPlayer->Nr() > 0) {
		battleship->Move(DirectionRight);
	}


	// Now process the mouse (if the button is pressed)
	if ( GetCapture() == m_hWnd )
	{
		// Hide the mouse pointer
		SetCursor( NULL );

		// Retrieve the cursor position
		GetCursorPos( &CursorPos );

		// Reset our cursor position so we can keep going forever :)
		SetCursorPos( m_OldCursorPos.x, m_OldCursorPos.y );

	} // End if Captured
}

//-----------------------------------------------------------------------------
// Name : AnimateObjects () (Private)
// Desc : Animates the objects we currently have loaded.
//-----------------------------------------------------------------------------
void CGameApp::AnimateObjects()
{
	m_pPlayer->Update(m_Timer.GetTimeElapsed());
	m_pPlayer2->Update(m_Timer.GetTimeElapsed());

	bullet->Update(m_Timer.GetTimeElapsed());

	heart11->Update(m_Timer.GetTimeElapsed());
	heart12->Update(m_Timer.GetTimeElapsed());
	heart13->Update(m_Timer.GetTimeElapsed());

	heart21->Update(m_Timer.GetTimeElapsed());
	heart22->Update(m_Timer.GetTimeElapsed());
	heart23->Update(m_Timer.GetTimeElapsed());

	score1->Update(m_Timer.GetTimeElapsed());
	score2->Update(m_Timer.GetTimeElapsed());

	asteroid->Update(m_Timer.GetTimeElapsed());

	battleship->Update(m_Timer.GetTimeElapsed());

	powerup->Update(m_Timer.GetTimeElapsed());
	ebullet->Update(m_Timer.GetTimeElapsed());

}

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



bool ObjCollision(Sprite* player2, Sprite* object) {

	// Hitboxes

	RECT r1;
	r1.left = player2->mPosition.x - player2->width() / 2;
	r1.right = player2->mPosition.x + player2->width() / 2;
	r1.top = player2->mPosition.y - player2->width() / 2;
	r1.bottom = player2->mPosition.y + player2->width() / 2;


	RECT r2;
	r2.left = object->mPosition.x - object->width() / 2;
	r2.right = object->mPosition.x + object->width() / 2;
	r2.top = object->mPosition.y - object->width() / 2;
	r2.bottom = object->mPosition.y + object->width() / 2;



	if (/*r1.left > r2.right && r1.right < r2.left && r1.bottom > r2.top && r1.top < r2.bottom*/r2.bottom > r1.top && r2.top < r1.bottom && r2.right > r1.left && r2.left < r1.right){
		player2->mPosition.y = -100;
		return true;
	}

	if (/*r1.left > r2.right && r1.right < r2.left && r1.bottom > r2.top && r1.top < r2.bottom*/r2.bottom > r1.top && r2.top < r1.bottom && r2.right > r1.left && r2.left < r1.right) {
		player2->mPosition.y = -100;
		return true;
	}

	return false;

}



bool powerupCollision(Sprite* player2, Sprite* object) {

	// Hitboxes

	RECT r1;
	r1.left = player2->mPosition.x - player2->width() / 2;
	r1.right = player2->mPosition.x + player2->width() / 2;
	r1.top = player2->mPosition.y - player2->width() / 2;
	r1.bottom = player2->mPosition.y + player2->width() / 2;


	RECT r2;
	r2.left = object->mPosition.x - object->width() / 2;
	r2.right = object->mPosition.x + object->width() / 2;
	r2.top = object->mPosition.y - object->width() / 2;
	r2.bottom = object->mPosition.y + object->width() / 2;



	if (/*r1.left > r2.right && r1.right < r2.left && r1.bottom > r2.top && r1.top < r2.bottom*/r2.bottom > r1.top && r2.top < r1.bottom && r2.right > r1.left && r2.left < r1.right) {
		return true;
	}

	if (/*r1.left > r2.right && r1.right < r2.left && r1.bottom > r2.top && r1.top < r2.bottom*/r2.bottom > r1.top && r2.top < r1.bottom && r2.right > r1.left && r2.left < r1.right) {
		return true;
	}

	return false;

}

//-----------------------------------------------------------------------------
// Name : DrawObjects () (Private)
// Desc : Draws the game objects
//-----------------------------------------------------------------------------
void CGameApp::DrawObjects( )
{
	m_pBBuffer->reset();


	m_imgBackground.Paint(m_pBBuffer->getDC(), 0, y);
	y += 0.1;
	if (y >= 0)
		y = -300;

	m_pPlayer->Draw();
	m_pPlayer2->Draw();

	bullet->Draw();

	heart11->Draw();
	heart12->Draw();
	heart13->Draw();

	heart21->Draw();
	heart22->Draw();
	heart23->Draw();

	score1->Draw();
	score2->Draw();
	
	asteroid->Draw();
	battleship->Draw();
	powerup->Draw();
	ebullet->Draw();

	if (Collision(m_pPlayer->m_pSprite, m_pPlayer2->m_pSprite2)) {
		SetTimer(m_hWnd, 1, 250, NULL);
		m_pPlayer->Explode();
		m_pPlayer->Position() = Vec2(GetSystemMetrics(SM_CXSCREEN) / 2 + 300, GetSystemMetrics(SM_CYSCREEN) + 200);
		m_pPlayer->Update(m_Timer.GetTimeElapsed());

		SetTimer(m_hWnd, 2, 250, NULL);
		m_pPlayer2->Explode();
		m_pPlayer2->Position() = Vec2(GetSystemMetrics(SM_CXSCREEN) / 2 - 1000, GetSystemMetrics(SM_CYSCREEN) + 200);
		m_pPlayer2->Update(m_Timer.GetTimeElapsed());

	}


	if (ObjCollision(m_pPlayer2->m_pSprite2, asteroid->m_pSprite)) {
		SetTimer(m_hWnd, 2, 250, NULL);
		m_pPlayer2->Explode();
		m_pPlayer2->Position() = Vec2(GetSystemMetrics(SM_CXSCREEN) / 2 - 1000, GetSystemMetrics(SM_CYSCREEN) + 200);
		m_pPlayer2->Update(m_Timer.GetTimeElapsed());


		asteroid->Position() = Vec2(GetSystemMetrics(SM_CXSCREEN) / 2 + 300, GetSystemMetrics(SM_CYSCREEN) + 200);
		srand(991 * time(NULL));
		j = rand() % 601;
		asteroid->Position() = Vec2(j, -300);
		asteroid->Update(m_Timer.GetTimeElapsed());


	}


	if (ObjCollision(bullet->m_pSprite, asteroid->m_pSprite)) {
		bullet->Position() = Vec2(m_pPlayer2->Position().x + 850, m_pPlayer2->Position().y);
		bullet->Update(m_Timer.GetTimeElapsed());


		asteroid->Position() = Vec2(GetSystemMetrics(SM_CXSCREEN) / 2 + 300, GetSystemMetrics(SM_CYSCREEN) + 200);
		srand(991 * time(NULL));
		j = rand() % 601;
		asteroid->Position() = Vec2(j, -300);
		asteroid->Update(m_Timer.GetTimeElapsed());


	}


	if (powerupCollision(m_pPlayer2->m_pSprite2, powerup->m_pSprite)) {
		PlaySound("data/powerupSFX.wav", NULL, SND_FILENAME | SND_ASYNC);
		powerup->Position() = Vec2(GetSystemMetrics(SM_CXSCREEN) / 2 + 300, GetSystemMetrics(SM_CYSCREEN) + 200);
		srand(991 * time(NULL));
		j = rand() % 601;
		powerup->Position() = Vec2(j, -4000);
		powerup->Update(m_Timer.GetTimeElapsed());


	}



	if (ObjCollision(m_pPlayer2->m_pSprite2, ebullet->m_pSprite)) {
		SetTimer(m_hWnd, 2, 250, NULL);
		m_pPlayer2->Explode();
		m_pPlayer2->Position() = Vec2(GetSystemMetrics(SM_CXSCREEN) / 2 - 1000, GetSystemMetrics(SM_CYSCREEN) + 200);
		m_pPlayer2->Update(m_Timer.GetTimeElapsed());


		ebullet->Position() = Vec2(GetSystemMetrics(SM_CXSCREEN) / 2 + 300, GetSystemMetrics(SM_CYSCREEN) + 200); 
		ebullet->Position() = Vec2(battleship->Position().x, battleship->Position().y + 50);
		ebullet->Update(m_Timer.GetTimeElapsed());


	}



	m_pBBuffer->present();

}



//-----------------------------------------------------------------------------
// Name : Save and Load
// Desc : Saving positions of Player 1 and 2 and Loading their last saved positions
//-----------------------------------------------------------------------------

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

void CGameApp::SavePlayer2()
{
	std::ofstream File;
	File.open("SafeFiles2.txt");
	File << m_pPlayer2->Position().x << " " << m_pPlayer2->Position().y << "\n";
	File.close();

}

void CGameApp::LoadPlayer2()
{
	std::ifstream File;
	File.open("SafeFiles2.txt");
	double xcoordinate2, ycoordinate2;
	File >> xcoordinate2 >> ycoordinate2;
	m_pPlayer2->Position() = Vec2(xcoordinate2, ycoordinate2);
	File.close();


}

