#include <stdafx.h>
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <MMSystem.h>

#include <gl\gl.h>
#include "glut.h"

#include "main.h"
#include "engine.h"
#include "camera.h"

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "GLAUX.lib")

#define MAX_NAME_LENGTH 15

BOOL startGame = FALSE, menu = FALSE;

TCHAR		szTitle[MAX_LOADSTRING];
TCHAR		szWindowClass[MAX_LOADSTRING];
char		username[MAX_NAME_LENGTH];
int			index = 0;

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
		FILE* fp;
		
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
		bArrKeys[VK_RETURN] = FALSE;
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

	glRasterPos2f(-0.4, 0.15);
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'M');
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'E');
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'N');
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'U');

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
	if (!createGLWindow(szTitle, 640, 480, 32, bFullscreen)) {
		return 0;
	}

	PlaySound(TEXT("lotr.wav"), NULL, SND_ASYNC|SND_FILENAME|SND_LOOP);

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
						if (!menu){
							drawGLScene();
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