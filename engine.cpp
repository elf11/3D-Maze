#include <stdafx.h>
#include <stdio.h>
#include <stdarg.h>
#include <windows.h>
#include <MMSystem.h>
#include <math.h>
#include <time.h>
#include "Includes\glee\GLee.h"
#include <GL/glu.h>
#include "GL/glaux.h"
#include "glut.h"

#include "global.h"
#include "engine.h"
#include "camera.h"
#include "vector.h"
#include "Generator.h"
#include "Sky.h"
#include "Math.h"

class Sky;

#define M_PI  3.1415926535

int nFrames     = 0;
int nFPS        = 0;
DWORD dwLastFPS = 0;

int nWallList   = 0;
int nTopList    = 0;

Sky *sky;

PCCamera PCam;

unsigned int	arrTextures[2];
unsigned int walkway[1];

float LightAmbient[]  = { 0.1f, 0.1f, 0.1f, 0.0f}; 		
float LightDiffuse[]  = { 0.7f, 0.7f, 0.7f, 1.0f};      
float LightPosition[] = {-1.5f, 1.5f, 1.5f, 1.0f};      
const GLfloat SKY_SCALE= 6.0f;
const GLfloat SKY_DISTANCE= 45.0f;
const GLint BMP_HEADER_SIZE=54;
TVector2D charPos; 
float cRadius = 5.0f;

const float arrCubeVertices[36][3] = {
        1.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 1.0f,
        0.0f, 1.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 1.0f,
        0.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 1.0f,
        0.0f, 1.0f, 1.0f,
        0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f,
    };

int maxx;
int maxz;
static GLuint haze;
static GLuint rroad;
const GLfloat FOGCOLOR [4] = {0.0f, 0.0f, 0.0f, 1.0f};
GLfloat	rtri;
TVector2D startPos;
TVector2D portalPos;
AUX_RGBImageRec *road;

int **m;

#define MAZE_HEIGHT (20)
#define MAZE_WIDTH  (20)



int arrMazeData[MAZE_HEIGHT][MAZE_WIDTH];
TVector2D *freeCells;
int nrFreeCells = 0;

TVertex arrVertices[MAZE_WIDTH * MAZE_HEIGHT * 6 * 2 * 3];


int initOpenGL(GLvoid) {

	haze = maketex("up.bmp",512,512);
	rroad = maketex("crosshair1.bmp", 600, 438);

	maxx = MAZE_WIDTH;
	maxz = MAZE_HEIGHT;

	m = (int **)malloc(maxz * sizeof(int *));

	//Generator gen = Generator(maxx, maxz);
	//gen.mazeGen();

	FILE * f = fopen("maze.txt", "r");
	if (f == NULL)
	{
		printf("Eroare! Nu s-a putut gasi fisierul maze.txt\n");
		return -1;
	}
	char c;
	int i =0, j = 0;
	m[i] = (int *)malloc(maxx * sizeof(int));
	while((c = fgetc(f)) != EOF)
	{
		if (c == '\n')
		{
			i += 1;
			j = 0;
			m[i] = (int *)malloc(maxz * sizeof(int));
		} else {
			if (c == '0') m[i][j] = 0;
			else m[i][j] = 1;
			j += 1;
		}
	}

	fclose(f);

	for (int i = 0; i < MAZE_HEIGHT; i += 1)
		for (int j = 0; j < MAZE_WIDTH; j += 1)
			arrMazeData[i][j] = m[i][j];

	freeCells = (TVector2D *)malloc(maxx * maxz * sizeof(TVector2D));

	for (int i = 0; i < MAZE_HEIGHT; i += 1)
		for (int j = 0; j < MAZE_WIDTH; j += 1)
		{
			if (arrMazeData[i][j] == 0)
			{
				freeCells[nrFreeCells].fX = i;
				freeCells[nrFreeCells].fY = j;
				nrFreeCells += 1;
			}
		}

	srand(time(NULL));
	int x;
	x = rand() % nrFreeCells;
	startPos = freeCells[16];
	startPos.fX = startPos.fX + 2.1;
	startPos.fY = startPos.fY + 2.2;

	portalPos = freeCells[24];
			
    if (!loadGLTextures()) {
		return FALSE;                                    
	}

	sky = new Sky;
    
    // Setting up Z-Buffer
    glClearDepth(1.0);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    
    if (bUseLighting) {
        glEnable(GL_LIGHTING);
    } else {
        glDisable(GL_LIGHTING);
    }
    
    glClearColor(0.0f, 0.0f, 0.4f, 1.0f);                    
    
    //glLightfv(GL_LIGHT0, GL_AMBIENT, LightAmbient);				// Setup ambient light (LIGTH0)
    glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);				// Setup diffuse light (LIGHT1), will move with player
    glLightfv(GL_LIGHT1, GL_POSITION, LightPosition);			// Position The Light
    //glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
        
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);		
    
    glDisable(GL_CULL_FACE);
    glShadeModel(GL_SMOOTH);
    
	PCam = new CCamera(startPos.fX, 0.3, startPos.fY);
    
	

    initLists();
    
    return TRUE;                                                // Initialization went OK
}


GLvoid resizeGLScene(GLsizei nWidth, GLsizei nHeight) {			// Resize and initialize the GL window
	if (nHeight == 0) { 						                // Prevent a divide by zero by
	    nHeight = 1;            						        // making height equal one
	}

	glViewport(0, 0, nWidth, nHeight);          				// Reset the current viewport

    glMatrixMode(GL_PROJECTION);				            	// Select the projection matrix
	glLoadIdentity();	        					            // Reset the projection matrix

	gluPerspective(60.0f, (GLfloat)nWidth / (GLfloat)nHeight, 0.01f, 160.0f);

	glMatrixMode(GL_MODELVIEW);			            			// Select the modelview matrix
	glLoadIdentity();   						            	// Reset the modelview matrix
}

GLvoid destroyGLWindow(GLvoid)	{
    if (bFullscreen) {
        ChangeDisplaySettings(NULL, 0);
		ShowCursor(TRUE);
    }
    
    // Remove the rendering context
    if (hRC != NULL) {
        hRC = NULL;
    }
    
    // Remove the device context
    if ((hDC != NULL) && !ReleaseDC(hWnd, hDC)) {;
		hDC = NULL;
	}
	
	// Destroy the window
	if ((hWnd != NULL) && !DestroyWindow(hWnd)) {
		hWnd = NULL;
	}
	
	if (!UnregisterClass(szWindowClass, hInst)) {
		hInst = NULL;
	}
}

BOOL createGLWindow(TCHAR* szWndTitle, int nWidth, int nHeight, int iBits, bool bFullscreenFlag) {
    GLuint		PixelFormat;
    WNDCLASS	wc;					        		           
    DWORD		dwExStyle;
	DWORD		dwStyle;
	RECT WindowRect;						                	
	
	WindowRect.left   = (long)0;				            	
	WindowRect.right  = (long)nWidth;			            	
	WindowRect.top    = (long)0;			            		
	WindowRect.bottom = (long)nHeight;		            		
	
	bFullscreen = bFullscreenFlag;				            	
	
	hInst   		    = GetModuleHandle(NULL);		        
	wc.style		    = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	
	wc.lpfnWndProc	    = (WNDPROC) WndProc;
	wc.cbClsExtra	    = 0;
	wc.cbWndExtra	    = 0;
	wc.hInstance	    = hInst;
	wc.hIcon	    	= LoadIcon(NULL, IDI_WINLOGO);
	wc.hCursor  		= LoadCursor(NULL, IDC_CROSS);
	wc.hbrBackground    = NULL;						            
	wc.lpszMenuName		= NULL;
	wc.lpszClassName	= szWindowClass;
    
    if (!RegisterClass(&wc)) {   
		return FALSE;
	}

    if (bFullscreen) {								                
        dwExStyle = WS_EX_APPWINDOW;				               
		dwStyle = WS_POPUP;					                    	
		ShowCursor(FALSE);						                    
    } else {
        dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;	    		
		dwStyle = WS_OVERLAPPEDWINDOW;					            
    }
    
    AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);		
    
    if (!(hWnd = CreateWindowEx(dwExStyle,	            			
				                szWindowClass,			           
					            szWndTitle,					        
					            WS_CLIPSIBLINGS |			        
					            WS_CLIPCHILDREN |			        
					            dwStyle,				            
					            0, 0,			            		
					            WindowRect.right-WindowRect.left,	
					            WindowRect.bottom-WindowRect.top,	
			            		NULL,			            		
					            NULL,					           
					            hInst,  				            
				            	NULL)))					            
	{
	    destroyGLWindow();							                
		return FALSE;							                   
	}
	
    static	PIXELFORMATDESCRIPTOR pfd = {			            	
		sizeof(PIXELFORMATDESCRIPTOR),	            				
		1,							                            	
		PFD_DRAW_TO_WINDOW |					                    
		PFD_SUPPORT_OPENGL |						                
		PFD_DOUBLEBUFFER,					                    	
		PFD_TYPE_RGBA,							                    
		iBits,			                        					
		0, 0, 0, 0, 0, 0,						                    
		0,								                            // No Alpha Buffer
		0,							                            	
		0,						                            		
		0, 0, 0, 0,							                        
		16,								                           
		0,							                            	
		0,							                            	
		PFD_MAIN_PLANE,							                   
		0,						                            		
		0, 0, 0								                        
	};
    
    if (!(hDC = GetDC(hWnd))) {           							
		destroyGLWindow();				                			
		return FALSE;	                    						
	}
    
    if (!(PixelFormat = ChoosePixelFormat(hDC, &pfd))) {		    
		destroyGLWindow();						                	
		return FALSE;						                    	
	}
	
    if(!SetPixelFormat(hDC, PixelFormat, &pfd)) {	        		
		destroyGLWindow();			                			
		return FALSE;                   							
	}

    if (!(hRC = wglCreateContext(hDC))) {	            			
		destroyGLWindow();              							
		return FALSE;				                    			
	}
	
	if(!wglMakeCurrent(hDC, hRC)) {         						
		destroyGLWindow();              							
		return FALSE;	                    						
	}

    ShowWindow(hWnd, SW_SHOW);	                					
	SetForegroundWindow(hWnd);					                	
	SetFocus(hWnd);			                    					
	resizeGLScene(nWidth, nHeight);				            		

    if (!initOpenGL()) {                							
		destroyGLWindow();	                						
		return FALSE;	                    						
	}
    
    return TRUE;						                    		
}

int maketex(const char* tfile,GLint xSize,GLint ySize) //returns tex. no.
{
	 GLuint rmesh;
	 FILE * file;
	 unsigned char * texdata = (unsigned char*) malloc( xSize * ySize * 3 ); //3 is {R,G,B}

	 file = fopen(tfile, "rb" );
	 fseek(file,BMP_HEADER_SIZE,SEEK_CUR);
	 fread( texdata, xSize * ySize * 3, 1, file ); 
	 fclose( file );
	 glEnable( GL_TEXTURE_2D );

	 char* colorbits = new char[ xSize * ySize * 3]; 

	 for(GLint a=0; a<xSize * ySize * 3; ++a) colorbits[a]=0xFF; 

	 glGenTextures(1,&rmesh);
	 glBindTexture(GL_TEXTURE_2D,rmesh);

	 glTexImage2D(GL_TEXTURE_2D,0 ,3 , xSize,
	 ySize, 0 , GL_RGB, GL_UNSIGNED_BYTE, colorbits);

	 glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	 glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	 //Save viewport and set up new one
	 GLint viewport[4]; //4 is {X,Y,Width,Height}
	 glGetIntegerv(GL_VIEWPORT,(GLint*)viewport);

	 glViewport(0,0,xSize,ySize);

	 //Clear target and depth buffers
	 glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT); 

	 glPushMatrix(); //Duplicates MODELVIEW stack top
	 glLoadIdentity(); //Replace new top with {1}

	 glDrawPixels(xSize,ySize, GL_BGR_EXT, GL_UNSIGNED_BYTE,texdata);

	 glPopMatrix();
	 glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 0, 0, xSize, ySize, 0);
	 
	 //Restore viewport
	 glViewport(viewport[0],viewport[1],viewport[2],viewport[3]); //{X,Y,Width,Height}
	 delete[] colorbits;
	 free(texdata);
	 return rmesh;

}

AUX_RGBImageRec* LoadBMP(char* Filename) {
	FILE* File = NULL;
	
	if (Filename == NULL) {
		return NULL;
	}
	File = fopen(Filename, "r");
	
	if (File != NULL) {
		fclose(File);							
		return auxDIBImageLoadA(Filename);		
	}

    return NULL;
}	

int loadGLTextures(void) {
    int Status = FALSE;
    AUX_RGBImageRec* arrTextureImage[2];
    
    memset(arrTextureImage, 0, sizeof(void*) * 2);
    
    arrTextureImage[0] = LoadBMP("wall1.bmp");
    arrTextureImage[1] = LoadBMP("crosshair1.bmp");
    
    if (arrTextureImage[0] != NULL &&
        arrTextureImage[1] != NULL) {
		Status = TRUE;
		
		glGenTextures(1, &walkway[0]);
		
		glBindTexture(GL_TEXTURE_2D, walkway[0]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
		
	
        gluBuild2DMipmaps(GL_TEXTURE_2D, 3,
                          arrTextureImage[1]->sizeX,
                          arrTextureImage[1]->sizeY,
                          GL_RGB, GL_UNSIGNED_BYTE,
                          arrTextureImage[1]->data);
		

	    glGenTextures(2, &arrTextures[0]);
	    
        // Create MipMapped texture
		glBindTexture(GL_TEXTURE_2D, arrTextures[0]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
		
	
        gluBuild2DMipmaps(GL_TEXTURE_2D, 3,
                          arrTextureImage[0]->sizeX,
                          arrTextureImage[0]->sizeY,
                          GL_RGB, GL_UNSIGNED_BYTE,
                          arrTextureImage[0]->data);
                          
        glBindTexture(GL_TEXTURE_2D, arrTextures[1]);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	  
	    glTexImage2D(GL_TEXTURE_2D, 0, 3,
		             arrTextureImage[1]->sizeX,
		             arrTextureImage[1]->sizeY,
		             0, GL_RGB, GL_UNSIGNED_BYTE,
		             arrTextureImage[1]->data);
					 
	}
	
	if (arrTextureImage[0] != NULL) {
		if (arrTextureImage[0]->data != NULL) {
			free(arrTextureImage[0]->data);
		}
		free(arrTextureImage[0]);
	}
	
	if (arrTextureImage[1] != NULL) {
		if (arrTextureImage[1]->data != NULL) {
			free(arrTextureImage[1]->data);
		}
		free(arrTextureImage[1]);
	}
	
	return Status;	
}

// draw the ground plane
void draw_ground(int dimx, int dimy, int tessx, int tessy, float height){

	float sx = (float)dimx;
	float sy = (float)dimy;
	float tx = (float)tessx;
	float ty = (float)tessy;
	if(tx==0) tx=1;
	if(ty==0) ty=1;

	float cx = sx/tx; 
	float cy = sy/ty; 
	float dx = sx/cx;
	float dy = sy/cy;

	int sizex = (int)floor(cx);
	int sizey = (int)floor(cy);

	glPushMatrix();
	glTranslatef(-dimx/2.0f, height, -35);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, rroad);
	glBegin(GL_QUADS);
		for(int i=0;i<sizex;i++){
			for(int j=0;j<sizey;j++){
				glTexCoord2f(1.0f, 0.0f);
				glVertex3f( i*dx, 0, j*dy);
				glTexCoord2f(1.0f, 1.0f);
				glVertex3f( (i+1)*dx, 0, j*dy);
				glTexCoord2f(0.0f, 1.0f);
				glVertex3f( (i+1)*dx, 0, (j+1)*dy);
				glTexCoord2f(0.0f, 0.0f);
				glVertex3f( i*dx, 0, (j+1)*dy);
			}
		}
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}

// draw sky
void draw_Skybox(float x, float y, float z, float width, float height, float length)
{
	// Center the Skybox around the given x,y,z position
	x = x - width  / 2;
	y = y - height / 2;
	z = z - length / 2;
	
	glEnable(GL_TEXTURE_2D);

	Vector3 cameraPos = Vector3(width, height, length);
		//Vector3 cameraPos = Vector3(0,0,0);
	sky->Render(cameraPos);

	glDisable(GL_TEXTURE_2D);

}


void initLists() {
    nWallList   = genWalls();
}

int isWall(int x,int y) {
        return ((x >= 0) && (y >= 0) && (x < MAZE_WIDTH) && (y < MAZE_HEIGHT) && (arrMazeData[x][y] != 0));
}

void genWallMesh() {
    int x, y, j;
    TVertex CurVertex;
   
            
    g_nVertexCount = 0;
    glBegin(GL_TRIANGLES);
        for (x = 0; x < MAZE_WIDTH; ++x) {
            for (y = 0; y < MAZE_HEIGHT; ++y) {
                if (isWall(y, x)) {
                    for (j = 0; j < 36; j += 3) {
                            CurVertex.arr_vTriangle[0]   = toVector(arrCubeVertices[j][0] + x,
                                                                    arrCubeVertices[j][1],
                                                                    arrCubeVertices[j][2] + y);
                            CurVertex.arr_vTriangle[1]   = toVector(arrCubeVertices[j + 1][0] + x,
                                                                    arrCubeVertices[j + 1][1],
                                                                    arrCubeVertices[j + 1][2] + y);
                            CurVertex.arr_vTriangle[2]   = toVector(arrCubeVertices[j + 2][0] + x,
                                                                    arrCubeVertices[j + 2][1],
                                                                    arrCubeVertices[j + 2][2] + y);
                            if (j % 2 == 0) {
                                CurVertex.arr_vTexture[0]    = toVector(0.0f, 0.0f);
                                CurVertex.arr_vTexture[1]    = toVector(0.0f, 1.0f);
                                CurVertex.arr_vTexture[2]    = toVector(1.0f, 1.0f);
                            } else {
                                CurVertex.arr_vTexture[0]    = toVector(1.0f, 1.0f);
                                CurVertex.arr_vTexture[1]    = toVector(1.0f, 0.0f);
                                CurVertex.arr_vTexture[2]    = toVector(0.0f, 0.0f);
                            }
                            CurVertex.vNormal            = getPolygonNormal(CurVertex.arr_vTriangle[0],
                                                                            CurVertex.arr_vTriangle[1],
                                                                            CurVertex.arr_vTriangle[2]);
                            
                            glNormal3f(CurVertex.vNormal.fX,
                                       CurVertex.vNormal.fY,
                                       CurVertex.vNormal.fZ);
                                       
                            glTexCoord2f(CurVertex.arr_vTexture[0].fX,
                                         CurVertex.arr_vTexture[0].fY);
                            glVertex3f(CurVertex.arr_vTriangle[0].fX,
                                       CurVertex.arr_vTriangle[0].fY,
                                       CurVertex.arr_vTriangle[0].fZ);
                                       
                            glTexCoord2f(CurVertex.arr_vTexture[1].fX,
                                         CurVertex.arr_vTexture[1].fY);
                            glVertex3f(CurVertex.arr_vTriangle[1].fX,
                                       CurVertex.arr_vTriangle[1].fY,
                                       CurVertex.arr_vTriangle[1].fZ);
                            
                            glTexCoord2f(CurVertex.arr_vTexture[2].fX, 
                                         CurVertex.arr_vTexture[2].fY);
                            glVertex3f(CurVertex.arr_vTriangle[2].fX,
                                       CurVertex.arr_vTriangle[2].fY,
                                       CurVertex.arr_vTriangle[2].fZ);
                                       
                            arrVertices[g_nVertexCount] = CurVertex;
                            ++g_nVertexCount;
                    }
                }
            }
        }
    glEnd();
}

int genWalls() {
    int nDrawList;
    
    glNewList(nDrawList = glGenLists(1), GL_COMPILE);
        //glColor3f(1.0f, 1.0f, 1.0f);
		glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, arrTextures[0]);
        genWallMesh();
		glDisable(GL_TEXTURE_2D);
    glEndList();
    
    return (nDrawList);
}

void camForward(float fSpeed)
{
    PCam->moveForward(fSpeed);
}

void camStrafe(float fSpeed)
{
    PCam->moveStrafe(fSpeed);
}

void camElevate(float fSpeed) 
{
    PCam->moveUp(fSpeed);
}

void camRotate(float fRoll, float fPitch, float fYaw) 
{
    PCam->rotateRPY(fRoll, fPitch, fYaw);
}

void camAdjustSpeed(float fSpeedMult) 
{
    PCam->m_fSpeedMult = fSpeedMult;
}

void camReset() 
{
    PCam->reset();
}

void draw_Character(float x, float y, float z)
{
	//float newx = x; //* 0.1;
	//float newz = z;//* 0.1;
	//glRotatef(rtri,0.0f,1.0f,0.0f);

	glBegin(GL_QUADS);									// Draw A Quad
		//glColor3f(0.0f,1.0f,0.0f);						// Set The Color To Green
		glVertex3f(x + 0.1f, y+0.1f,z-0.1f);					// Top Right Of The Quad (Top)
		glVertex3f(x-0.1f, y+0.1f,z-0.1f);					// Top Left Of The Quad (Top)
		glVertex3f(x-0.1f, y+0.1f, z+0.1f);					// Bottom Left Of The Quad (Top)
		glVertex3f(x+ 0.1f, y+0.1f, z+0.1f);					// Bottom Right Of The Quad (Top)
		//glColor3f(1.0f,0.5f,0.0f);						// Set The Color To Orange
		glVertex3f(x+ 0.1f,y-0.1f, z+0.1f);					// Top Right Of The Quad (Bottom)
		glVertex3f(x-0.1f,y-0.1f, z+0.1f);					// Top Left Of The Quad (Bottom)
		glVertex3f(x-0.1f,y-0.1f,z-0.1f);					// Bottom Left Of The Quad (Bottom)
		glVertex3f(x+ 0.1f,y-0.1f,z-0.1f);					// Bottom Right Of The Quad (Bottom)
		//glColor3f(1.0f,0.0f,0.0f);						// Set The Color To Red
		glVertex3f(x+ 0.1f, y+0.1f, z+0.1f);					// Top Right Of The Quad (Front)
		glVertex3f(x-0.1f, y+0.1f, z+0.1f);					// Top Left Of The Quad (Front)
		glVertex3f(x-0.1f,y-0.1f, z+0.1f);					// Bottom Left Of The Quad (Front)
		glVertex3f(x+ 0.1f,y-0.1f, z+0.1f);					// Bottom Right Of The Quad (Front)
		//glColor3f(1.0f,1.0f,0.0f);						// Set The Color To Yellow
		glVertex3f(x+ 0.1f,y-0.1f,z-0.1f);					// Top Right Of The Quad (Back)
		glVertex3f(x-0.1f,y-0.1f,z-0.1f);					// Top Left Of The Quad (Back)
		glVertex3f(x-0.1f, y+0.1f,z-0.1f);					// Bottom Left Of The Quad (Back)
		glVertex3f(x+ 0.1f, y+0.1f,z-0.1f);					// Bottom Right Of The Quad (Back)
		//glColor3f(0.0f,0.0f,1.0f);						// Set The Color To Blue
		glVertex3f(x-0.1f, y+0.1f, z+0.1f);					// Top Right Of The Quad (Left)
		glVertex3f(x-0.1f, y+0.1f,z-0.1f);					// Top Left Of The Quad (Left)
		glVertex3f(x-0.1f,y-0.1f,z-0.1f);					// Bottom Left Of The Quad (Left)
		glVertex3f(x-0.1f,y-0.1f, z+0.1f);					// Bottom Right Of The Quad (Left)
		//glColor3f(1.0f,0.0f,1.0f);						// Set The Color To Violet
		glVertex3f(x+ 0.1f, y+0.1f,z-0.1f);					// Top Right Of The Quad (Right)
		glVertex3f(x+ 0.1f, y+0.1f, z+0.1f);					// Top Left Of The Quad (Right)
		glVertex3f( x+0.1f,y-0.1f, z+0.1f);					// Bottom Left Of The Quad (Right)
		glVertex3f( x+0.1f,y-0.1f,z-0.1f);					// Bottom Right Of The Quad (Right)
	glEnd();				
}

void redisplay()
{
	glPushMatrix();
	if (fog == TRUE)
		{
			glEnable(GL_FOG);
			glFogi(GL_FOG_MODE, GL_EXP2);
			glFogfv(GL_FOG_COLOR, FOGCOLOR);
			glFogf(GL_FOG_DENSITY, 0.3f);
			glHint(GL_FOG_HINT, GL_NICEST);
			glFogf(GL_FOG_START, 0.0f);
			glFogf(GL_FOG_END, 3.0f);
			
		}
	glPopMatrix();
}

int drawGLScene(GLvoid)	
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glPushMatrix();

        PCam->mouseLook();
        if (!bNoClipping) {
            PCam->checkCollision(arrVertices);
        }

	    glRotatef(PCam->m_fPitch, 1.0f, 0.0f, 0.0f);
	    glRotatef(PCam->m_fRoll,  0.0f, 0.0f, 1.0f);
	    glRotatef(PCam->m_fYaw,   0.0f, 1.0f, 0.0f);
	    glTranslatef(-PCam->m_fPosX,
	                 -PCam->m_fPosY,
	                 -PCam->m_fPosZ);
	
	    LightPosition[0] = PCam->m_fPosX;
	    LightPosition[1] = PCam->m_fPosY;
	    LightPosition[2] = PCam->m_fPosZ;
	    glLightfv(GL_LIGHT1, GL_POSITION, LightPosition);
		glEnable(GL_LIGHT1);

		

		glPushMatrix();
		glColor3f( 0.8f, 0.8f, 0.8f);
		
		//glTranslatef(portalPos.fX , 0.4f, portalPos.fY);
		//glutSolidTorus(0.1, 0.2, 15, 15);
		glPopMatrix();

		//PCam = new CCamera(startPos.fX, 0.3, startPos.fY);

		if(fsP == TRUE)
		{
			//charPos.fX = PCam->m_fPosX;
			//charPos.fY = PCam->m_fPosZ;
			//glTranslatef(0.0f, 0.0f, -0.5);
			int offset = 0.3;
			//draw_Character(PCam->m_fPosX - 0.3, 0.1f, PCam->m_fPosZ + 0.2); 
		}
		
		// Fog
		if (fog == TRUE)
		{
			glEnable(GL_FOG);
			glFogi(GL_FOG_MODE, GL_EXP2);
			glFogfv(GL_FOG_COLOR, FOGCOLOR);
			glFogf(GL_FOG_DENSITY, 0.3f);
			glHint(GL_FOG_HINT, GL_NICEST);
			glFogf(GL_FOG_START, 0.0f);
			glFogf(GL_FOG_END, 3.0f);	
		} else
			if (fog == FALSE)
			{
				glDisable(GL_FOG);
			}


		
		draw_Skybox(0, 0, 0, PCam->m_fPosX, PCam->m_fPosY, PCam->m_fPosZ);
		
		draw_ground(100,100,0,0,0.0);

		

        glCallList(nWallList);

    glPopMatrix();

	return TRUE;
}