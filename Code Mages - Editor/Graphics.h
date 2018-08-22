#ifndef GRAPHICS_HEADER_
#define GRAPHICS_HEADER_

#include"Windowsx.h"
#include"ddraw.h"
#include"Primitive.h"
#include"Collision.h"

extern DWORD bitCount;
extern char isWindowed;
extern int Width,Height;
extern RECT ClipRect;
extern unsigned char *VideoBuffer;

/*Defining Write_Pixel Macro*/
#define Graphics_WritePixel(x,y,r,g,b)\
{\
	unsigned char *mem=VideoBuffer + ((x)+Width*(y))*bitCount;\
	/*Write Pixel depending on the bitCount*/\
	switch(bitCount)\
	{\
		case 2:\
			{\
				WORD color=RGB16(r,g,b);\
				*mem=(unsigned char)color;\
				*++mem=(unsigned char)(color>>8);\
				break;\
			}\
		case 3:\
				*mem=b;\
				*++mem=g;\
				*++mem=r;\
				break;\
		case 4:\
				*mem=b;\
				*++mem=g;\
				*++mem=r;\
				*++mem=0;\
				break;\
	}\
}

Color_RGB GetPixelColor(int x, int y);

/*Macros Definitions*/
#define SAFE_RELEASE(p) {if(p){(p)->Release();(p)=NULL;}}

#define RGB16(r,g,b) ((b>>3)|((g>>2)<<5)|((r>>3)<<11))
#define RGB32(a,r,g,b) ((b)+((g)<<8)+((r)<<16)+((a)<<24))

#ifndef _POINT_FLOAT_
#define _POINT_FLOAT_
/*Point Float Structure*/
typedef struct Point_Float
{
	float x,y;
}Point_Float;
#endif

/*Function Definitions*/
void Graphics_Flip();
void Graphics_CreateBuffer();
void Graphics_FreeDirectDraw();
void Graphics_TextOut(int x,int y,const char *str);
void Graphics_Draw(void (*)());
HRESULT Graphics_CreateFullScreenDisplay(HWND hwnd,WORD bdp);
HRESULT Graphics_CreateWindowedDisplay(HWND hwnd,DWORD width,DWORD height);
void Grid(unsigned int square_width, unsigned int square_height, unsigned int dash_spacing, unsigned char r, unsigned char g, unsigned char b, unsigned char axis_r, unsigned char axis_g, unsigned char axis_b);
void Reset_Surface (void);
void Graphics_Refresh(void);
void DrawRect (RECT rect, unsigned char r, unsigned char g, unsigned char b);
void Preview_Color(RECT r,COLORREF Color);

#ifndef _PIXEL_CLIP_
#define _PIXEL_CLIP_
/*Inline Pixel Clipping Function*/ 			
__inline BOOL Pixel_Clip(int x,int y) 			
{ 			
	if(x>ClipRect.left&&x<ClipRect.right&&y>ClipRect.top&&y<ClipRect.bottom) 			
		return TRUE; 			
	return FALSE; 			
}
#endif

/*Primitive Rasterization Functions*/
BOOL LB_Clipp(Point_Float *p1,Point_Float *p2);
void LINE(float x0,float y0,float x1,float y1,unsigned int r,unsigned int g,unsigned int b);
void CIRCLE(float xc,float yc, int R,unsigned char r,unsigned char g,unsigned char b);
void FILL_CIRCLE(float xc,float yc, int R,unsigned char r,unsigned char g,unsigned char b);
void TRIANGLE(Point_Float *PF, unsigned char r,unsigned char g,unsigned char b);
void FILL_TRIANGLE(Point_Float *triangleVtx,unsigned char r,unsigned char g,unsigned char b);
void SortVertices(Point_Float *triangleVtx);
/*Collision*/
void COLLISION_LINE(float x0,float y0,float x1,float y1,unsigned int r,unsigned int g,unsigned int b);
void COLLISION_CIRCLE(float xc,float yc, int R,unsigned char r,unsigned char g,unsigned char b);

#endif