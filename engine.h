#ifndef _engine_h_
#define _engine_h_

extern int nFrames;
extern int nFPS;
extern DWORD dwLastFPS;

int initOpenGL(void);

int drawGLScene(void);
void draw_ground(int dimx, int dimy, int tessx, int tessy, float height);
void draw_Skybox(float x, float y, float z, float width, float height, float length);
void draw_Character(float x, float y, float z);
int maketex(const char* tfile,GLint xSize,GLint ySize); 

void initLists();
int genWalls();
int loadGLTextures(void);
void redisplay();

void destroyGLWindow(void);
BOOL createGLWindow(TCHAR* szWndTitle, int nWidth, int nHeight, int iBits, bool bFullscreen);
void resizeGLScene(int nWidth, int nHeight);

void camForward(float fSpeed);
void camStrafe(float fSpeed);
void camElevate(float fSpeed);
void camRotate(float fRoll, float fPitch, float fYaw);
void camAdjustSpeed(float fSpeedMult);
void camReset();

#endif