#include <stdafx.h>
#include <sstream>
#include <string>
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <MMSystem.h>

#include <gl\gl.h>
#include "glut.h"

#include "Save.h"
#include "main.h"
#include "engine.h"
#include "camera.h"

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "GLAUX.lib")

#ifdef _DEBUG
	#pragma comment(lib, "sfml-system-s-d.lib")
	#pragma comment(lib, "sfml-window-s-d.lib")
	#pragma comment(lib, "sfml-graphics-s-d.lib")
	#pragma comment(lib, "sfml-audio-s-d.lib")
#else
	#pragma comment(lib, "sfml-system-s.lib")
	#pragma comment(lib, "sfml-window-s.lib")
	#pragma comment(lib, "sfml-graphics-s.lib")
	#pragma comment(lib, "sfml-audio-s.lib")
#endif

#ifdef _DEBUG
#pragma comment( lib, "libtheoraplayer_d.lib" )
#else
#pragma comment( lib, "libtheoraplayer.lib" )
#endif

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")

#include "SFML/Window.hpp"
#include <assert.h>
#include <windows.h>
#include <GL/gl.h>
#include "TheoraVideoManager.h"
#include "TheoraVideoClip.h"
#include "TheoraPlayer.h"
#include "TheoraDataSource.h"
#include "OpenAL_AudioInterface.h"


#define MAX_NAME_LENGTH 15

BOOL startGame = FALSE, menu = FALSE;

TCHAR		szTitle[MAX_LOADSTRING];
TCHAR		szWindowClass[MAX_LOADSTRING];
int				user_id;
char			username[MAX_NAME_LENGTH];
std::string		user;
game_data		data;
int				addUser = 1;
int				index = 0;
int				menuIndex = 0;

HGLRC			hRC		= NULL;
HDC				hDC		= NULL;
HWND			hWnd	= NULL;
HINSTANCE		hInst	= NULL;

BOOL	bActive		= TRUE;
BOOL	bFullscreen = TRUE;
BOOL	fsP			= FALSE;
BOOL	fog			= FALSE;

BOOL	bArrKeys[256];
BOOL	lp = FALSE;
BOOL	xp = FALSE;
BOOL	sp = FALSE;
BOOL	np = FALSE;
BOOL	pp = FALSE;


BOOL	bUseLighting	= FALSE;
BOOL	bCullFaces		= FALSE;
BOOL	bDrawNormals	= FALSE;
BOOL	bNoClipping		= FALSE;

unsigned int createTexture(int w,int h,unsigned int format=GL_RGB)
{
	unsigned int tex_id;
	glGenTextures(1,&tex_id);
	glBindTexture(GL_TEXTURE_2D,tex_id);
	unsigned char* b=new unsigned char[w*h*4];
	memset(b,0,w*h*4);

	glTexImage2D(GL_TEXTURE_2D,0,(format == GL_RGB) ? 3 : 4,w,h,0,format,GL_UNSIGNED_BYTE,b);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	delete b;
	return tex_id;
}

int nextPow2(int x)
{
	int y;
	for (y=1;y<x;y*=2);
	return y;
}

void drawTexturedQuad(float x,float y,float w,float h,float sw,float sh)
{
	glBegin (GL_QUADS);
	glTexCoord2f(0,  0); glVertex3f(x,  y,  0.0f);
	glTexCoord2f(sw, 0); glVertex3f(x+w,y,  0.0f);
	glTexCoord2f(sw,sh); glVertex3f(x+w,y+h,0.0f);
	glTexCoord2f(0, sh); glVertex3f(x,  y+h,0.0f);
	glEnd();
}

void video()
{
	int i;
		// Create the Theora video manager and the OpenAL audio interface factory, then link them
	TheoraVideoManager* Manager = new TheoraVideoManager();
	OpenAL_AudioInterfaceFactory* OpenAL_AIF = new OpenAL_AudioInterfaceFactory();
	Manager->setAudioInterfaceFactory( OpenAL_AIF );

    // Create the main window
    sf::Window App(sf::VideoMode(320, 280, 32), "Cinematics playing");

    // Create a clock for measuring time elapsed
    sf::Clock Clock;
	float time_slice = 0.0f;
	float time_elapsed = Clock.GetElapsedTime();

    // Set color and depth clear value
    glClearDepth(1.f);
	glClearColor(0.8f, 0.8f, 0.8f, 0.5f);
	
	// Enable 2D textures
	glEnable(GL_TEXTURE_2D);

    // Enable Z-buffer read and write
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);

	//  Load a test clip in to be viewed
	TheoraVideoClip* test_clip = NULL;
	test_clip = Manager->createVideoClip( "Diablo_III_All_Cinematics.ogv" );
	//test_clip = Manager->createVideoClip( new TheoraMemoryFileDataSource( "media/bunny.ogg" ), TH_RGB );
	test_clip->setAutoRestart( true );

	unsigned int TextureID = createTexture(nextPow2(test_clip->getWidth()),nextPow2(test_clip->getHeight()));

	   // Start game loop
	while (App.IsOpened())
    {
        // Process events
        sf::Event Event;
        while (App.GetEvent(Event))
        {
            // Close window : exit
			if (Event.Type == sf::Event::Closed) {
				App.Close();
				bArrKeys['M'] = false;
			}
		}

		//  Update time_slice
		time_slice = Clock.GetElapsedTime() - time_elapsed;
		time_elapsed = Clock.GetElapsedTime();

        // Set the active window before using OpenGL commands
        // It's useless here because active window is always the same,
        // but don't forget it if you use multiple windows or controls
        App.SetActive();

		//UPDATE ANIMATION
		if ( test_clip && !test_clip->isPaused() && test_clip->getNumReadyFrames() > test_clip->getNumPrecachedFrames() * 0.5f )
		{
			Manager->update( time_slice );
		}
		//UPDATE ANIMATION

		/**/// 2D Rendering
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glViewport(0, 0, 320, 280);
		gluOrtho2D(0, 320, 280, 0);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glDisable( GL_DEPTH_TEST );
		glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );

		//RENDER ANIMATION
		glBindTexture( GL_TEXTURE_2D, TextureID );
		if ( TheoraVideoFrame* frame = test_clip->getNextFrame() )
		{
			glTexSubImage2D( GL_TEXTURE_2D, 0, 0, 0, test_clip->getWidth(), frame->getHeight(), GL_RGB, GL_UNSIGNED_BYTE, frame->getBuffer() );
			test_clip->popFrame();
		}
		float w=float(test_clip->getWidth()),h=float(test_clip->getHeight());
		float tw=float(nextPow2(int(w))),th=float(nextPow2(int(h)));
		glEnable(GL_TEXTURE_2D);
		drawTexturedQuad(0,0,float(test_clip->getWidth()), float(test_clip->getHeight()),w/tw,h/th);
		//RENDER ANIMATION
		/**/// 2D Rendering

        // Finally, display rendered frame on screen
        App.Display();
	}

	//  Delete the video manager and audio inferface factory
	delete Manager;
	delete OpenAL_AIF;
}



LRESULT CALLBACK WndProc(HWND	hWnd,
						 UINT	uMsg,
						 WPARAM	wParam,
						 LPARAM	lParam)
{
	switch (uMsg) {
		case WM_ACTIVATE:
			{
				if (!HIWORD(wParam)) {
				bActive = TRUE;
				} else {
					bActive = FALSE;
				}
			return 0;
			}
		case WM_SYSCOMMAND:
			{
				switch (wParam) {
					case SC_SCREENSAVE:
					case SC_MONITORPOWER:
						return 0;
				}
				break;
			}
		case WM_CLOSE:	
			{
				PostQuitMessage(0);
				return 0;
			}
		case WM_KEYDOWN:
			{
				bArrKeys[wParam] = TRUE;
				return 0;
			}
		case WM_KEYUP:
			{
				bArrKeys[wParam] = FALSE;
				return 0;
			}
		case WM_SIZE:
			{
				resizeGLScene(LOWORD(lParam), HIWORD(lParam));
				return 0;
			}
	}

	// Pass all unhandled messages to DefWindowProc
	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}

void dispatchKeys() {
	if (bArrKeys['L'] && !lp) {
		lp = TRUE;
		bUseLighting = !bUseLighting;
		if (!bUseLighting) {
			glDisable(GL_LIGHTING);
		} else {
			glEnable(GL_LIGHTING);
		}
	}
	if (!bArrKeys['L'])	{
		lp = FALSE;
	}
	if (bArrKeys[VK_PRIOR])	{
		camRotate(-0.9f, 0.0f, 0.0f);
	}
	if (bArrKeys[VK_NEXT])	{
		camRotate(0.9f, 0.0f, 0.0f);
	}
	if (bArrKeys['W'])	{
		camForward(-0.02f);
	}
	if (bArrKeys['S'])	{
		camForward(0.02f);
	}
	if (bArrKeys['A'])	{
		camStrafe(-0.02f);
	}
	if (bArrKeys['D'])	{
		camStrafe(0.02f);
	}
	if (bArrKeys['C']) {
		fsP = !fsP;
		redisplay();
	}
	if (bArrKeys['J']) {
		camRotate(0.0f, -0.9f, 0.0f);
	}
	if (bArrKeys['K']) {
		camRotate(0.0f, 0.9f, 0.0f);
	}
	if (bArrKeys['G'])	{
		camRotate(0.0f, 0.0f, -0.9f);
	}
	if (bArrKeys['H']) {
		camRotate(0.0f, 0.0f, 0.9f);
	}
	if (bArrKeys['R']) {
		camReset();
	}

	if (bArrKeys['M']) {
		//video_draw = TRUE;
		bArrKeys['M'] = false;
    }

	if (bArrKeys['P']) {
		menu = TRUE;
		bArrKeys['P'] = FALSE;
	}
	if (bArrKeys['F']) {
		if (fog == TRUE)
		{
			fog = FALSE;
			redisplay();
		}
		else
		{
			fog = TRUE;
			redisplay();
		}
	}

	if (bArrKeys['S']) {
		//write data in user's file
		save_user_data(data, user_id);
	}
}

void dispatchKeys_userinput(){
	if (bArrKeys[VK_RETURN]){
		startGame = TRUE;
		bArrKeys[VK_RETURN] = FALSE;
	}
	if (bArrKeys[8]){
		index--;
		bArrKeys[8] = FALSE;
	}
	
	for (int i = 0; i < 28; i++){
		if (bArrKeys['A' + i]){
			if (index < MAX_NAME_LENGTH){
				username[index++] = 'A' + i;
				bArrKeys['A' + i] = FALSE;
			}
		}
	}
}

void dispatchKeys_menu(){
	if (bArrKeys[VK_RETURN]){
		menu = FALSE;
		menuIndex = 0;
		bArrKeys[VK_RETURN] = FALSE;
	}
	if (bArrKeys[VK_DOWN]){
		menuIndex++;
		if (menuIndex >= 5)
			menuIndex = 0;
		bArrKeys[VK_DOWN] = FALSE;
	}
	if (bArrKeys[VK_UP]){
		menuIndex--;
		if (menuIndex < 0)
			menuIndex = 5;
		bArrKeys[VK_UP] = FALSE;
	}
}

int drawUserInputBox(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0, 0, 0, 1);
	glLoadIdentity();

	glDisable(GL_DEPTH_TEST);

	glScalef(8.0, 8.0, 8.0);
	glTranslatef(0, 0, -1);

	glColor3f(0.0f,1.0f,0.0f);

	glLineWidth(0.5); 
	glColor3f(1.0, 1.0, 1.0);

	glRasterPos2f(-0.4, 0.15);
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'E');
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'N');
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'T');
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'E');
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'R');
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, ' ');
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'U');
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'S');
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'E');
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'R');
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'N');
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'A');
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'M');
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'E');
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, ':');

	glBegin(GL_LINES);
	glVertex3f(-0.4, 0.1, 0.0);
	glVertex3f(0.4, 0.1, 0.0);
	glEnd();

	glBegin(GL_LINES);
	glVertex3f(-0.4, -0.1, 0.0);
	glVertex3f(0.4, -0.1, 0.0);
	glEnd();

	glBegin(GL_LINES);
	glVertex3f(-0.4, 0.1, 0.0);
	glVertex3f(-0.4, -0.1, 0.0);
	glEnd();

	glBegin(GL_LINES);
	glVertex3f(0.4, 0.1, 0.0);
	glVertex3f(0.4, -0.1, 0.0);
	glEnd();

	glRasterPos2f(-0.3, 0);
	for (int i = 0; i < index; i++){
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, username[i]);
	}

	glEnable(GL_DEPTH_TEST);

	return TRUE;
}

int drawMenu(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0, 0, 0, 1);
	glLoadIdentity();

	glDisable(GL_DEPTH_TEST);

	glScalef(8.0, 8.0, 8.0);
	glTranslatef(0, 0, -1);

	glColor3f(0.0f,1.0f,0.0f);

	glLineWidth(0.5); 
	glColor3f(1.0, 1.0, 1.0);

	glRasterPos2f(-0.1, 0.5);
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'M');
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'E');
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'N');
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'U');

	if (menuIndex != 0)
		glColor3f(0.5, 0.5, 0.5);
	else
		glColor3f(0.9, 0.9, 0.9);
	glBegin(GL_QUADS);
	glVertex3f(-0.2, 0.3, 0);
	glVertex3f(0.2, 0.3, 0);
	glVertex3f(0.2, 0.4, 0);
	glVertex3f(-0.2, 0.4, 0);
	glEnd();
	glColor3f(0, 0, 0);
	glRasterPos2f(-0.19, 0.35);
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'S');
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'T');
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'A');
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'R');
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'T');

	if (menuIndex != 1)
		glColor3f(0.5, 0.5, 0.5);
	else
		glColor3f(0.9, 0.9, 0.9);
	glBegin(GL_QUADS);
	glVertex3f(-0.2, 0.1, 0);
	glVertex3f(0.2, 0.1, 0);
	glVertex3f(0.2, 0.2, 0);
	glVertex3f(-0.2, 0.2, 0);
	glEnd();
	glColor3f(0, 0, 0);
	glRasterPos2f(-0.19, 0.15);
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'H');
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'E');
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'L');
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'P');

	if (menuIndex != 2)
		glColor3f(0.5, 0.5, 0.5);
	else
		glColor3f(0.9, 0.9, 0.9);
	glBegin(GL_QUADS);
	glVertex3f(-0.2, -0.1, 0);
	glVertex3f(0.2, -0.1, 0);
	glVertex3f(0.2, 0, 0);
	glVertex3f(-0.2, 0, 0);
	glEnd();
	glColor3f(0, 0, 0);
	glRasterPos2f(-0.19, -0.05);
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'L');
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'E');
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'V');
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'E');
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'L');
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, ' ');
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, '1');

	if (menuIndex != 3)
		glColor3f(0.5, 0.5, 0.5);
	else
		glColor3f(0.9, 0.9, 0.9);
	glBegin(GL_QUADS);
	glVertex3f(-0.2, -0.3, 0);
	glVertex3f(0.2, -0.3, 0);
	glVertex3f(0.2, -0.2, 0);
	glVertex3f(-0.2, -0.2, 0);
	glEnd();
	glColor3f(0, 0, 0);
	glRasterPos2f(-0.19, -0.25);
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'L');
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'E');
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'V');
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'E');
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'L');
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, ' ');
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, '2');

	if (menuIndex != 4)
		glColor3f(0.5, 0.5, 0.5);
	else
		glColor3f(0.9, 0.9, 0.9);
	glBegin(GL_QUADS);
	glVertex3f(-0.2, -0.5, 0);
	glVertex3f(0.2, -0.5, 0);
	glVertex3f(0.2, -0.4, 0);
	glVertex3f(-0.2, -0.4, 0);
	glEnd();
	glColor3f(0, 0, 0);
	glRasterPos2f(-0.19, -0.45);
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'L');
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'E');
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'V');
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'E');
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'L');
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, ' ');
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, '3');

	glEnable(GL_DEPTH_TEST);

	return TRUE;
}

int WINAPI WinMain(HINSTANCE	hInstance,
					HINSTANCE	hPrevInstance,
					LPSTR		lpCmdLine,
					int			nCmdShow)
{
	MSG msg;
	BOOL bDone = FALSE;

	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_GLMAZE3, szWindowClass, MAX_LOADSTRING);
	
	bFullscreen = FALSE;
    // Create Our OpenGL Window
	if (!createGLWindow(data, szTitle, 640, 480, 32, bFullscreen)) {
		return 0;
	}

	PlaySound(TEXT("lotr.wav"), NULL, SND_ASYNC|SND_FILENAME|SND_LOOP);

	drawGLScene(data.life);
	SwapBuffers(hDC);


	long int  melc = 0;
	while (!bDone) {
		
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT) {
				bDone = TRUE;
			} else {
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		} else {
			// Draw the scene.  Watch for ESC key and quit messages
			if (bActive) {
				if (bArrKeys[VK_ESCAPE]) {
					bDone = TRUE;
				} else {
					if (startGame){
						if (addUser == 1) {
							user = username;
							load_user(&data, user, &user_id);
							addUser = 0;
						}	

						if (!menu){
							melc++;
							if (melc== 400) {
									data.life --;
									melc = 0;
							}

							drawGLScene(data.life);
							SwapBuffers(hDC);
							dispatchKeys();
						}
						else{
							drawMenu();
							SwapBuffers(hDC);
							dispatchKeys_menu();
						}
					}
					else{
						drawUserInputBox();
						SwapBuffers(hDC);
						dispatchKeys_userinput();
					}
				}
			}
		}
	}

	// Shutdown
	destroyGLWindow();
	return (msg.wParam);
}