#ifndef _global_h_
#define _global_h_

#define MAX_LOADSTRING	100
#define MAX_LEVELS		3
#define MAX_LENGTH		30

#define LEVEL_1		1
#define LEVEL_2		2
#define LEVEL_3		3

typedef struct {
	int level;
	int life;
	int guns;
} game_data;

extern TCHAR    szTitle[MAX_LOADSTRING];					
extern TCHAR    szWindowClass[MAX_LOADSTRING];	    		

extern HGLRC           hRC;			             			
extern HDC             hDC;					            	
extern HWND            hWnd;					            
extern HINSTANCE       hInst;		        		    	

extern BOOL	    bActive;						        	
extern BOOL	    bFullscreen;							   

extern BOOL	    bUseLighting;
extern BOOL	    bCullFaces;
extern BOOL	    bDrawNormals;
extern BOOL	    bNoClipping;

extern BOOL	    bArrKeys[256];								
extern BOOL	    lp;									        
extern BOOL	    xp;					        				
extern BOOL	    sp;					        				
extern BOOL	    np;					        				
extern BOOL		fog;
extern BOOL		fsP;

LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);

#endif