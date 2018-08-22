#include <windows.h>
#include "Graphics.h"
#include "Interface.h"

extern HINSTANCE GlobalInstance;

unsigned long *SelBuffer=NULL;

#define Absolute(x) (((x)<0)?-x:x)

/*Min/Max Macros*/
#define Min(a,b) ((a<b)?a:b)
#define Max(a,b) ((a>b)?a:b)

void Create_Sel_Buffer(void)
{
	/*Allocate Memory for VideBuffer*/
	SelBuffer = (unsigned long*)malloc(Width*Height*4);
	memset(SelBuffer,-1,Width*Height*4);
}

void Reset_Sel_Buffer(void)
{
	/* Clearing the surface */
	memset(SelBuffer,-1,Width*Height*4);
}

/*Draw Selection Primitives on Sel_Buffer*/
void Sel_Draw(Primitive *Primitives,Collision_Data *Collisions)
{
	/*Primitive indices will be even, Collision indices will be odd*/
	int Primitive_Index=0,Collision_Index=1;
	/*Draw All Character Primitives in List*/
	for(Primitive *tmp=Primitives;tmp;tmp=tmp->next,Primitive_Index+=2)
	{
		/*Draw Primitive According to it's type*/
		switch(tmp->Type)
		{
			case Type_Line:
							SEL_LINE(tmp->V[0].x,tmp->V[0].y,tmp->V[1].x,tmp->V[1].y,Primitive_Index);
							break;
			case Type_Circle:
							SEL_CIRCLE(tmp->V[0].x,tmp->V[0].y,(int)tmp->V[1].x,Primitive_Index);
							break;
			case Type_Filled_Circle:
							SEL_FILL_CIRCLE(tmp->V[0].x,tmp->V[0].y,(int)tmp->V[1].x,Primitive_Index);
							break;
			case Type_Triangle:
							{
								Point_Float p[3];
								for(int i=0;i<3;i++)
								{
									p[i].x=tmp->V[i].x;
									p[i].y=tmp->V[i].y;
								}
								SEL_TRIANGLE(p,Primitive_Index);
							}
							break;
			case Type_Filled_Triangle:
							{
								Point_Float p[3];
								for(int i=0;i<3;i++)
								{
									p[i].x=tmp->V[i].x;
									p[i].y=tmp->V[i].y;
								}
								SEL_FILL_TRIANGLE(p,Primitive_Index);
							}
							break;
		}
	}
	/*Draw All Collision Primitives in List*/
	for(Collision_Data *Ctmp=Collisions;Ctmp;Ctmp=Ctmp->next,Collision_Index+=2)
	{
		/*Draw Primitive According to it's type*/
		switch(Ctmp->Type)
		{
			case Type_Collision_Line:
							SEL_FILL_CIRCLE(Ctmp->V0.x,Ctmp->V0.y,2,Collision_Index);	//drawing the connections of the beginning of the edge
							SEL_LINE(Ctmp->V0.x,Ctmp->V0.y,Ctmp->V1.x,Ctmp->V1.y,Collision_Index);
							SEL_FILL_CIRCLE(Ctmp->V1.x,Ctmp->V1.y,2,Collision_Index);	//drawing the connections of the beginning of the edge
							break;
			case Type_Collision_Circle:
							SEL_CIRCLE(Ctmp->V0.x,Ctmp->V0.y,(int)Ctmp->V1.x,Collision_Index);
							break;
		}
	}
}

/*Function to resolve current selection*/
Selection Resolve_Selection(Primitive *Primitives,Collision_Data *Collisions,bool Mode,int x,int y)
{
	long index;
	int i,j;
	Selection Sel={0};
	for(i=x-Sel_Sen;i<=x+Sel_Sen;i++)
	{
		for(j=y-Sel_Sen;j<=y+Sel_Sen;j++)
		{
			if(Pixel_Clip(i,j))
			{
				index=SelBuffer[i+Width*j];
				if(index>=0)					//There is a selection
				{
					/*Check whether it's a primitive or collision selection*/
					if(!(index%2)&&!Mode)//Primitive Selection
					{
						/*Get Primitive Pointer*/
						index>>=1;
						for(Primitive *tmp=Primitives;index;tmp=tmp->next,index--);
						Sel.p=tmp;Sel.c=NULL;
						return Sel;
					}
					else if((index%2)&&Mode)	//Collision Selection
					{
						/*Get Collision Pointer*/
						index>>=1;
						for(Collision_Data *tmp=Collisions;index;tmp=tmp->next,index--);
						Sel.p=NULL;Sel.c=tmp;
						return Sel;
					}
				}
			}
		}
	}
	return Sel;
}

void Draw_BoundingBox(Primitive *p,Collision_Data *c)
{
	Float_Rect Box;
	Point_Float P1,P2;
	if(p)		//if supplied Primitive in argument is valid
	{
		switch(p->Type)
		{
			case Type_Line:
							/*Set Bounding Box of Line*/
							Box.left=(Min(p->V[0].x,p->V[1].x));
							Box.right=(Max(p->V[0].x,p->V[1].x));
							Box.top=(Min(p->V[0].y,p->V[1].y));
							Box.bottom=(Max(p->V[0].y,p->V[1].y));
							break;
			case Type_Circle:
			case Type_Filled_Circle:
							/*Set Bounding Box of Circle*/
							Box.left=(p->V[0].x-p->V[1].x);
							Box.right=(p->V[0].x+p->V[1].x);
							Box.top=(p->V[0].y-p->V[1].x);
							Box.bottom=(p->V[0].y+p->V[1].x);
							break;
			case Type_Triangle:
			case Type_Filled_Triangle:
							/*Set Bounding Box of Triangle*/
							Box.left=(Min(p->V[0].x,p->V[1].x)<Min(p->V[1].x,p->V[2].x)?Min(p->V[0].x,p->V[1].x):Min(p->V[1].x,p->V[2].x));
							Box.right=(Max(p->V[0].x,p->V[1].x)>Max(p->V[1].x,p->V[2].x)?Max(p->V[0].x,p->V[1].x):Max(p->V[1].x,p->V[2].x));
							Box.top=(Min(p->V[0].y,p->V[1].y)<Min(p->V[1].y,p->V[2].y)?Min(p->V[0].y,p->V[1].y):Min(p->V[1].y,p->V[2].y));
							Box.bottom=(Max(p->V[0].y,p->V[1].y)>Max(p->V[1].y,p->V[2].y)?Max(p->V[0].y,p->V[1].y):Max(p->V[1].y,p->V[2].y));
							break;
		}
		/*Draw Box after Clipping*/
		P1.x=Box.left;P1.y=Box.top;P2.x=Box.right;P2.y=Box.top;
		if(LB_Clipp(&P1,&P2))
			COLLISION_LINE(P1.x,P1.y,P2.x,P2.y,BD_RED,BD_GREEN,BD_BLUE);
		P1.x=Box.left;P1.y=Box.bottom;P2.x=Box.right;P2.y=Box.bottom;
		if(LB_Clipp(&P1,&P2))
			COLLISION_LINE(P1.x,P1.y,P2.x,P2.y,BD_RED,BD_GREEN,BD_BLUE);
		P1.x=Box.left;P1.y=Box.top;P2.x=Box.left;P2.y=Box.bottom;
		if(LB_Clipp(&P1,&P2))
			COLLISION_LINE(P1.x,P1.y,P2.x,P2.y,BD_RED,BD_GREEN,BD_BLUE);
		P1.x=Box.right;P1.y=Box.top;P2.x=Box.right;P2.y=Box.bottom;
		if(LB_Clipp(&P1,&P2))
			COLLISION_LINE(P1.x,P1.y,P2.x,P2.y,BD_RED,BD_GREEN,BD_BLUE);
	}
	else if(c)
	{
		switch(c->Type)
		{
			case Type_Collision_Line:
									/*Set Bounding Box of Line*/
									Box.left=(Min(c->V0.x,c->V1.x));
									Box.right=(Max(c->V0.x,c->V1.x));
									Box.top=(Min(c->V0.y,c->V1.y));
									Box.bottom=(Max(c->V0.y,c->V1.y));
									break;
			case Type_Collision_Circle:
									/*Set Bounding Box of Circle*/
									Box.left=(c->V0.x-c->V1.x);
									Box.right=(c->V0.x+c->V1.x);
									Box.top=(c->V0.y-c->V1.x);
									Box.bottom=(c->V0.y+c->V1.x);
									break;
		}
		/*Draw Box after Clipping*/
		P1.x=Box.left;P1.y=Box.top;P2.x=Box.right;P2.y=Box.top;
		if(LB_Clipp(&P1,&P2))
			COLLISION_LINE(P1.x,P1.y,P2.x,P2.y,BD_RED,BD_GREEN,BD_BLUE);
		P1.x=Box.left;P1.y=Box.bottom;P2.x=Box.right;P2.y=Box.bottom;
		if(LB_Clipp(&P1,&P2))
			COLLISION_LINE(P1.x,P1.y,P2.x,P2.y,BD_RED,BD_GREEN,BD_BLUE);
		P1.x=Box.left;P1.y=Box.top;P2.x=Box.left;P2.y=Box.bottom;
		if(LB_Clipp(&P1,&P2))
			COLLISION_LINE(P1.x,P1.y,P2.x,P2.y,BD_RED,BD_GREEN,BD_BLUE);
		P1.x=Box.right;P1.y=Box.top;P2.x=Box.right;P2.y=Box.bottom;
		if(LB_Clipp(&P1,&P2))
			COLLISION_LINE(P1.x,P1.y,P2.x,P2.y,BD_RED,BD_GREEN,BD_BLUE);
	}
}

void SmoothTransition (HWND hwnd, RECT Old, RECT New,int Transition_Speed)
{
	char Check=0;
	RECT RefreshRect=Old;

	/*Interpolation Factors*/
	int Interpolate_Left=New.left-Old.left;
	int Interpolate_Right=New.right-Old.right;
	int Interpolate_Top=New.top-Old.top;
	int Interpolate_Bottom=New.bottom-Old.bottom;
	/*Loop Counts*/
	int Loop_Left=abs(Interpolate_Left/Transition_Speed);
	int Loop_Right=abs(Interpolate_Right/Transition_Speed);
	int Loop_Top=abs(Interpolate_Top/Transition_Speed);
	int Loop_Bottom=abs(Interpolate_Bottom/Transition_Speed);
	/*Steps*/
	int Step_Left=(Interpolate_Left>0)?1:-1;
	int Step_Right=(Interpolate_Right>0)?1:-1;
	int Step_Top=(Interpolate_Top>0)?1:-1;
	int Step_Bottom=(Interpolate_Bottom>0)?1:-1;

	/*Resize window to old size and make sure it is shown*/
	MoveWindow(hwnd, Old.left,Old.top, Old.right-Old.left,Old.bottom-Old.top,1);
	ShowWindow(hwnd,SW_SHOW);
	while(Check<4)
	{
		if(Loop_Left)
		{
			Old.left+=Step_Left*Transition_Speed;
			Loop_Left--;
			Check=0;
		}
		else
			Check++;
		if(Loop_Right)
		{
			Old.right+=Step_Right*Transition_Speed;
			Loop_Right--;
			Check=0;
		}
		else
			Check++;
		if(Loop_Top)
		{
			Old.top+=Step_Top*Transition_Speed;
			Loop_Top--;
			Check=0;
		}
		else
			Check++;
		if(Loop_Bottom)
		{
			Old.bottom+=Step_Bottom*Transition_Speed;
			Loop_Bottom--;
			Check=0;
		}
		else
			Check++;
		/*Move to interpolated position*/
		MoveWindow(hwnd, Old.left,Old.top, Old.right-Old.left,Old.bottom-Old.top,true);
	}
	/*Move To Final Position*/
	MoveWindow(hwnd, New.left,New.top, New.right-New.left,New.bottom-New.top,true);
}


/*Primitive Selection Methods*/
/*Line Rasterizer*/
void SEL_LINE(float x0,float y0,float x1,float y1,unsigned long index)
{
	/*Declaring and defining all variables needed for algorithm*/
	int xStep,yStep,dx=(int)(x1+=0.5f)-(int)(x0+=0.5f),dy=(int)(y1+=0.5f)-(int)(y0+=0.5f),X0=(int)x0,Y0=(int)y0,X1=(int)x1,Y1=(int)y1;
	int PixelCount, PixelLeft;
	xStep=(dx>0)?1:-1;
	yStep=(dy>0)?1:-1;
	dx=(dx>0)?dx:-dx;
	dy=(dy>0)?dy:-dy;

	/*if both dx and dy are zero, return from function (nothing to draw)*/
	if(!dx&!dy)
		return;

	Sel_WritePixel(X0,Y0,index);
	Sel_WritePixel(X1,Y1,index);

	if(dx>=dy)
	{
		/*Declare and Define Decision variables*/
		int d0=2*dy-dx;
		int dA=2*(dy-dx);
		int dB=2*dy;

		PixelCount=(dx-1)/2;
		PixelLeft=(dx-1)%2;

		while(PixelCount--)
		{
			X0+=xStep;
			X1-=xStep;
			if(d0>=0)
			{
				d0+=dA;
				Y0+=yStep;
				Y1-=yStep;
			}
			else
				d0+=dB;
			Sel_WritePixel(X0,Y0,index);
			Sel_WritePixel(X1,Y1,index);
		}
		if(PixelLeft)
		{
			X0+=xStep;
			if(d0>=0)
				Y0+=yStep;
			Sel_WritePixel(X0,Y0,index);
		}
	}
	else
	{
		/*Declare and Define Decision variables*/
		int d0=2*dx-dy;
		int dA=2*(dx-dy);
		int dB=2*dx;

		PixelCount=(dy-1)/2;
		PixelLeft=(dy-1)%2;

		while(PixelCount--)
		{
			Y0+=yStep;
			Y1-=yStep;
			if(d0>=0)
			{
				d0+=dA;
				X0+=xStep;
				X1-=xStep;
			}
			else
				d0+=dB;
			Sel_WritePixel(X0,Y0,index);
			Sel_WritePixel(X1,Y1,index);
		}
		if(PixelLeft)
		{
			Y0+=yStep;
			if(d0>=0)
				X0+=xStep;
			Sel_WritePixel(X0,Y0,index);
		}
	}
}

/*Circle Rasterizer*/
void SEL_CIRCLE(float xc,float yc, int R,unsigned long index)
{
	/*Defining all needed variables*/
	int Xc=(int)(xc+0.5f),Yc=(int)(yc+0.5f);
	int X=0,Y=R,d=1-R;

	/*Draw first 8 points on circle (8 way symmetric)*/
	/*
		Note that if coordinates are greater than the buffersize or less than zero, point will not be added to avoid run-time error
		Instead point (Xc+R,Y) which is already on circle is added
	*/

	if(Pixel_Clip(X+Xc,Y+Yc))
		Sel_WritePixel(X+Xc,Y+Yc,index);

	if(Pixel_Clip(-X+Xc,Y+Yc))
		Sel_WritePixel(-X+Xc,Y+Yc,index);

	if(Pixel_Clip(X+Xc,-Y+Yc))
		Sel_WritePixel(X+Xc,-Y+Yc,index);

	if(Pixel_Clip(-X+Xc,-Y+Yc))
		Sel_WritePixel(-X+Xc,-Y+Yc,index);

	if(Pixel_Clip(Y+Xc,X+Yc))
		Sel_WritePixel(Y+Xc,X+Yc,index);

	if(Pixel_Clip(-Y+Xc,X+Yc))
		Sel_WritePixel(-Y+Xc,X+Yc,index);

	if(Pixel_Clip(Y+Xc,-X+Yc))
		Sel_WritePixel(Y+Xc,-X+Yc,index);

	if(Pixel_Clip(-Y+Xc,-X+Yc))
		Sel_WritePixel(-Y+Xc,-X+Yc,index);


	while(X<Y)							//Loop while X is less than Y ==> dx>dy
	{
		if(d<0)							//if the decision variable is less than zero
			d+=2*X+3;					//Update decision variable by 2*X+3
		else							//else
		{
			d+=2*(X-Y)+5;				//Update decision variable by 2*(X-Y)+5
			Y--;						//Decrement Y by 1
		}
		X++;							//Increment X by 1

		/*Draw all 8 symmetric points*/
		/*
			Note that if coordinates are greater than the buffersize or less than zero, point will not be added to avoid run-time error
			Instead point (Xc+R,Y) which is already on circle is added
		*/
		if(Pixel_Clip(X+Xc,Y+Yc))
			Sel_WritePixel(X+Xc,Y+Yc,index);

		if(Pixel_Clip(-X+Xc,Y+Yc))
			Sel_WritePixel(-X+Xc,Y+Yc,index);

		if(Pixel_Clip(X+Xc,-Y+Yc))
			Sel_WritePixel(X+Xc,-Y+Yc,index);

		if(Pixel_Clip(-X+Xc,-Y+Yc))
			Sel_WritePixel(-X+Xc,-Y+Yc,index);

		if(Pixel_Clip(Y+Xc,X+Yc))
			Sel_WritePixel(Y+Xc,X+Yc,index);

		if(Pixel_Clip(-Y+Xc,X+Yc))
			Sel_WritePixel(-Y+Xc,X+Yc,index);

		if(Pixel_Clip(Y+Xc,-X+Yc))
			Sel_WritePixel(Y+Xc,-X+Yc,index);

		if(Pixel_Clip(-Y+Xc,-X+Yc))
			Sel_WritePixel(-Y+Xc,-X+Yc,index);
	}
}

/*Filled Circle Rasterizer*/
void SEL_FILL_CIRCLE(float xc,float yc, int R,unsigned long index)
{
	/*Defining all needed variables*/
	int X=0,Y=R,Xc=(int)(xc+0.5f),Yc=(int)(yc+0.5f),d=1-R,de=3,dse=-2*R+5;
	int x,xEnd,y;

	/*Draw first horizontal diameter*/
	if((y=X+Yc)>=ClipRect.top && y<ClipRect.bottom)
	{
		x=-Y+Xc;if(x<ClipRect.left){x=ClipRect.left;};
		xEnd=Y+Xc;if(xEnd>ClipRect.right-1){xEnd=ClipRect.right-1;};

		for(;x<=xEnd;x++)
			Sel_WritePixel(x,y,index);
	}

	while(X<Y)
	{
		if(d<0)
		{
			d+=de;
			de+=2;
			dse+=2;
		}
		else
		{
			d+=dse;
			de+=2;
			dse+=4;
			Y--;
		}
		X++;

		/*Draw all 4 lines*/
		if((y=Y+Yc)>=ClipRect.top && y<ClipRect.bottom)
		{
			x=-X+Xc;if(x<ClipRect.left){x=ClipRect.left;};
			xEnd=X+Xc;if(xEnd>ClipRect.right-1){xEnd=ClipRect.right-1;};

			for(;x<=xEnd;x++)
				Sel_WritePixel(x,y,index);
		}

		if((y=X+Yc)>=ClipRect.top && y<ClipRect.bottom)
		{
			x=-Y+Xc;if(x<ClipRect.left){x=ClipRect.left;};
			xEnd=Y+Xc;if(xEnd>ClipRect.right-1){xEnd=ClipRect.right-1;};

			for(;x<=xEnd;x++)
				Sel_WritePixel(x,y,index);
		}

		if((y=-Y+Yc)>=ClipRect.top && y<ClipRect.bottom)
		{
			x=-X+Xc;if(x<ClipRect.left){x=ClipRect.left;};
			xEnd=X+Xc;if(xEnd>ClipRect.right-1){xEnd=ClipRect.right-1;};

			for(;x<=xEnd;x++)
				Sel_WritePixel(x,y,index);
		}

		if((y=-X+Yc)>=ClipRect.top && y<ClipRect.bottom)
		{
			x=-Y+Xc;if(x<ClipRect.left){x=ClipRect.left;};
			xEnd=Y+Xc;if(xEnd>ClipRect.right-1){xEnd=ClipRect.right-1;};

			for(;x<=xEnd;x++)
				Sel_WritePixel(x,y,index);
		}
	}
}

void SEL_TRIANGLE(Point_Float *PF, unsigned long index)
{
	int i, j;
	for(i=0, j=1; i<3; i++, j++)
	{
		if(j==3)
			j=0;
		SEL_LINE(PF[i].x, PF[i].y, PF[j].x, PF[j].y,index);
	}
}


void SEL_FILL_TRIANGLE(Point_Float *triangleVtx,unsigned long index)
{
	char topVtx,midVtx,botVtx,MidIsLeft,leftEdge,rightEdge;
	int y,yEnd,x,xEnd;
	float InvSlope[3],xL,xR;

	/*
		Determining Positions of the three vertices and updating 
		topVtx, midVtx, botVtx and MidIsLeft variables accordingly
	*/

	///////////////////////////////////////////////
	if(triangleVtx[0].y<triangleVtx[1].y)
	{
		if(triangleVtx[2].y<triangleVtx[0].y)
		{
			topVtx=2;
			midVtx=0;
			botVtx=1;
			MidIsLeft=1;
		}
		else
		{
			topVtx=0;
			if(triangleVtx[1].y<triangleVtx[2].y)
			{
				midVtx=1;
				botVtx=2;
				MidIsLeft=1;
			}
			else
			{
				midVtx=2;
				botVtx=1;
				MidIsLeft=0;
			}
		}
	}
	else
	{
		if(triangleVtx[2].y<triangleVtx[1].y)
		{
			topVtx=2;
			midVtx=1;
			botVtx=0;
			MidIsLeft=0;
		}
		else
		{
			topVtx=1;
			if(triangleVtx[0].y<triangleVtx[2].y)
			{
				midVtx=0;
				botVtx=2;
				MidIsLeft=0;
			}
			else
			{
				midVtx=2;
				botVtx=0;
				MidIsLeft=1;
			}
		}
	}
	///////////////////////////////////////////////
	/*Updating Inverse Slope Values*/
	InvSlope[0]=(triangleVtx[botVtx].x-triangleVtx[topVtx].x)/(triangleVtx[botVtx].y-triangleVtx[topVtx].y);
	InvSlope[1]=(triangleVtx[midVtx].x-triangleVtx[topVtx].x)/(triangleVtx[midVtx].y-triangleVtx[topVtx].y);
	InvSlope[2]=(triangleVtx[botVtx].x-triangleVtx[midVtx].x)/(triangleVtx[botVtx].y-triangleVtx[midVtx].y);

	/*Updating the leftEdge and rightEdge Variables*/
	leftEdge=MidIsLeft;
	rightEdge=!MidIsLeft;

	/*Getting starting and ending y values*/
	y=Ceil(triangleVtx[topVtx].y);
	yEnd=Ceil(triangleVtx[midVtx].y)-1;

	/*Getting Initial x values*/
	xL=xR=triangleVtx[topVtx].x;

	/*Readjusting x values for maximum precision*/
	xL+=InvSlope[leftEdge]*(y-triangleVtx[topVtx].y);
	xR+=InvSlope[rightEdge]*(y-triangleVtx[topVtx].y);

	/*Clipping First half of triangle by interpolating x and y*/
	if(y<=ClipRect.top)
	{
		xL+=InvSlope[leftEdge]*(ClipRect.top-y);
		xR+=InvSlope[rightEdge]*(ClipRect.top-y);
		y=ClipRect.top;
	}
	if(yEnd>=ClipRect.bottom-1)
		yEnd=ClipRect.bottom-1;
	/*Filling the triangle from Top to Mid vertex*/
	for(;y<=yEnd;y++)
	{
		x=Ceil(xL);if(x<ClipRect.left){x=ClipRect.left;};
		xEnd=Ceil(xR)-1;if(xEnd>ClipRect.right-1){xEnd=ClipRect.right-1;};
		for(;x<=xEnd;x++)
			Sel_WritePixel(x,y,index);

		xL+=InvSlope[leftEdge];
		xR+=InvSlope[rightEdge];
	}

	/*Updating the leftEdge and rightEdge Variables*/
	leftEdge=MidIsLeft<<1;
	rightEdge=(!MidIsLeft)<<1;

	/*Updating starting and ending y Values*/
	y=Ceil(triangleVtx[midVtx].y);
	yEnd=Ceil(triangleVtx[botVtx].y)-1;

	/*Clipping Second half of triangle by interpolating x and y*/
	if(MidIsLeft)
	{
		xL=triangleVtx[midVtx].x;
		if(y<=ClipRect.top)
		{
			xL=triangleVtx[midVtx].x+InvSlope[2]*(ClipRect.top-y);
			xR=triangleVtx[topVtx].x+InvSlope[0]*(ClipRect.top-Ceil(triangleVtx[topVtx].y));
			y=ClipRect.top;
		}
		if(yEnd>=ClipRect.bottom-1)
			yEnd=ClipRect.bottom-1;
	}
	else
	{
		xR=triangleVtx[midVtx].x;
		if(y<=ClipRect.top)
		{
			xR=triangleVtx[midVtx].x+InvSlope[2]*(ClipRect.top-y);
			xL=triangleVtx[topVtx].x+InvSlope[0]*(ClipRect.top-Ceil(triangleVtx[topVtx].y));
			y=ClipRect.top;
		}
		if(yEnd>=ClipRect.bottom-1)
			yEnd=ClipRect.bottom-1;
	}

	/*Filling the triangle from Mid to Bot vertex*/
	for(;y<=yEnd;y++)
	{
		x=Ceil(xL);if(x<ClipRect.left){x=ClipRect.left;};
		xEnd=Ceil(xR)-1;if(xEnd>ClipRect.right-1){xEnd=ClipRect.right-1;};
		for(;x<=xEnd;x++)
			Sel_WritePixel(x,y,index);

		xL+=InvSlope[leftEdge];
		xR+=InvSlope[rightEdge];
	}
}