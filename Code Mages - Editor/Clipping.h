#include "Graphics.h" 			
 			
/*External Clipping Rectangle*/ 			
extern RECT ClipRect; 			
 			
/*Function Prototypes*/ 			
BOOL LB_Clipp(Point_Float *p1,Point_Float *p2); 			

/*Inline Pixel Clipping Function*/ 			
__inline BOOL Pixel_Clip(int x,int y) 			
{ 			
	if(x>ClipRect.left&&x<ClipRect.right&&y>ClipRect.top&&y<ClipRect.bottom) 			
		return TRUE; 			
	return FALSE; 			
}