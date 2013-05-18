#include <stdafx.h>
#include <windows.h>
#include <MMSystem.h>

#include <gl\gl.h>
#include "glut.h"

#include "main.h"
#include "engine.h"
#include "camera.h"

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "GLAUX.lib")

TCHAR szTitle[MAX_LOADSTRING];
TCHAR szWindowClass[MAX_LOADSTRING];

HGLRC           hRC     = NULL;
HDC             hDC     = NULL;
HWND            hWnd    = NULL;
HINSTANCE       hInst   = NULL;

BOOL	bActive     = TRUE;
BOOL	bFullscreen = TRUE;
BOOL	fsP			= FALSE;
BOOL	fog			= FALSE;

BOOL	bArrKeys[256];
BOOL	lp = FALSE;
BOOL    xp = FALSE;
BOOL    sp = FALSE;
BOOL    np = FALSE;
BOOL    pp = FALSE;


BOOL	bUseLighting    = TRUE;
BOOL    bCullFaces      = FALSE;
BOOL    bDrawNormals    = FALSE;
BOOL    bNoClipping     = FALSE;

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
        camRotate(0.0f, 0.0f, 0.9f);
    }
    if (bArrKeys['H']) {
        camRotate(0.0f, 0.0f, -0.9f);
    }
    if (bArrKeys['M'] && !sp) {
        camAdjustSpeed(1.8f);
    }
    if (!bArrKeys['M']) {
        sp = FALSE;
        camAdjustSpeed(1.0f);
    }
    if (bArrKeys['R']) {
        camReset();
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
}

int WINAPI WinMain(HINSTANCE	hInstance,
		           HINSTANCE	hPrevInstance,
			       LPSTR		lpCmdLine,
			       int	    	nCmdShow)
{
 	MSG msg;
 	BOOL bDone = FALSE;
	
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_GLMAZE3, szWindowClass, MAX_LOADSTRING);
	
	bFullscreen = FALSE;
	bool startGame = FALSE;

	startGame = TRUE;

	if (startGame){
		// Create Our OpenGL Window
		if (!createGLWindow(szTitle, 640, 480, 32, bFullscreen)) {
			return 0;
		}

		PlaySound(TEXT("ObstacleCourse.wav"), NULL, SND_ASYNC|SND_FILENAME|SND_LOOP);
	    
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
						drawGLScene();
						SwapBuffers(hDC);
						dispatchKeys();
					}
				}
			}
		}
	}

	// Shutdown
	destroyGLWindow();
	
	return (msg.wParam);
}