#include "Clipping.h"

BOOL LB_Clipp(Point_Float *p1,Point_Float *p2) 			
{ 			
	int i; 			
	float ta=0,tb=1,pk[4],qk[4]; 			
 			
	/*Getting pk and qk for all the edges*/ 			
	pk[0]=p1->x-p2->x; 			
	pk[1]=-pk[0]; 			
	pk[2]=p1->y-p2->y; 			
	pk[3]=-pk[2]; 			
 			
	qk[0]=p1->x-ClipRect.left; 			
	qk[1]=ClipRect.right-1-p1->x; 			
	qk[2]=p1->y-ClipRect.top; 			
	qk[3]=ClipRect.bottom-1-p1->y; 			
 			
	/*Checking for trivial rejection of line*/ 			
	for(i=0;i<4;i++) 			
	{ 			
		if(pk[i]==0&&qk[i]<0) 			
		return FALSE; 			
	} 			
 			
	/*Getting new ta and tb values to clip line boundaries*/ 			
	for(i=0;i<4;i++) 			
	{ 			
		if(pk[i]<0 && ta<qk[i]/pk[i]) 			
			ta=qk[i]/pk[i]; 			
		else if(pk[i]>0 && tb>qk[i]/pk[i]) 			
			tb=qk[i]/pk[i]; 			
 			
	/*Reject line if necessary*/ 			
	if(ta>tb) 			
		return FALSE; 			
	} 			
 			
	/*Getting new starting and ending point values*/ 			
	p2->x=p1->x+pk[1]*tb; 			
	p2->y=p1->y+pk[3]*tb; 			
				
	p1->x+=pk[1]*ta; 			
	p1->y+=pk[3]*ta; 			
 			
	return TRUE; 			
}