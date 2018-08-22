#include"Primitive.h"
#include"Collision.h"

#define EDITOR_WIDTH 640
#define EDITOR_HEIGHT 510

/*Exterals*/
extern RECT ClipRect;
extern int Width,Height;
extern unsigned long *SelBuffer;

#define RGB16(r,g,b) ((b>>3)|((g>>2)<<5)|((r>>3)<<11))
#define RGB32(a,r,g,b) ((b)+((g)<<8)+((r)<<16)+((a)<<24))

#define BD_RED		255
#define BD_GREEN	0
#define BD_BLUE		0

#define TOOLBAR_HEIGHT 29

/*Defining Write_Pixel Macro*/
#define Sel_WritePixel(x,y,index)\
{\
	unsigned long *mem=SelBuffer + ((x)+Width*(y));\
	/*Write Pixel depending on it's index*/\
	*mem=index;\
}
/*Selection Sensitiity*/
#define Sel_Sen			2

#ifndef _POINT_FLOAT_
#define _POINT_FLOAT_
/*Point Float Structure*/
typedef struct Point_Float
{
	float x,y;
}Point_Float;
#endif

#ifndef _FLOAT_RECT_
#define _FLOAT_RECT_
/*Float Rectangle*/
struct Float_Rect
{
	float left,top,right,bottom;
};
#endif

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

/*Selection and Drawing Modes*/
enum Mode
{
	Mode_Draw,
	Mode_Select,
};

/*Selection Structure*/
struct Selection
{
	Primitive *p;
	Collision_Data *c;
};

/*Function Prototypes*/
void Create_Sel_Buffer(void);
void Reset_Sel_Buffer(void);
void Sel_Draw(Primitive *Primitives,Collision_Data *Collisions);
Selection Resolve_Selection(Primitive *Primitives,Collision_Data *Collisions,bool Mode,int x,int y);
void Draw_BoundingBox(Primitive *p,Collision_Data *c);
void SmoothTransition (HWND hwnd, RECT Old, RECT New,int Transition_Speed);

/*Primitive Selection*/
/*Primitive Rasterization Functions*/
long Ceil(float g);
void SEL_LINE(float x0,float y0,float x1,float y1,unsigned long index);
void SEL_CIRCLE(float xc,float yc, int R,unsigned long index);
void SEL_FILL_CIRCLE(float xc,float yc, int R,unsigned long index);
void SEL_TRIANGLE(Point_Float *PF,unsigned long index);
void SEL_FILL_TRIANGLE(Point_Float *triangleVtx,unsigned long index);