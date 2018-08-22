#include"Graphics.h"
#include"Interface.h"

extern HWND GlobalHwnd;

#define COLLISION_SPACING 3

DWORD bitCount=0;
char isWindowed=1;
int Width,Height;
unsigned char *VideoBuffer=NULL;
RECT ClipRect={0,0,640,480};
extern bool ShowRects;
extern RECT SourceRect, DestinationRect;

LPDIRECTDRAW lpdd=NULL;					//Direct Draw object
LPDIRECTDRAWSURFACE lpdds_Primary=NULL;	//Primary Surface
LPDIRECTDRAWSURFACE lpdds_Secondary=NULL;//Secondary Surface
LPDIRECTDRAWCLIPPER lpclipper=NULL;		//Clipper Object

HRESULT Graphics_CreateFullScreenDisplay(HWND hwnd,WORD bdp)
{
	DDSURFACEDESC ddsd;

	/*Get System Resolution*/
	Width=GetSystemMetrics(SM_CXSCREEN);
	Height=GetSystemMetrics(SM_CYSCREEN);

	/*Creating Direct Draw Object*/
	if(FAILED(DirectDrawCreate(NULL,&lpdd,NULL)))
		/*ERROR*/return E_FAIL;

	/*Setting cooperative level to full screen mode*/
	if(FAILED(lpdd->SetCooperativeLevel(hwnd,DDSCL_EXCLUSIVE|DDSCL_FULLSCREEN)))
		/*ERROR*/return E_FAIL;

	/*Setting the Display Mode*/
	if(FAILED(lpdd->SetDisplayMode(Width,Height,bdp)))
		/*ERROR*/return E_FAIL;

	/*Creating the surface and back-buffer*/

	/*Initializing the Direct Draw surface description*/
	ZeroMemory(&ddsd,sizeof(ddsd));
	ddsd.dwSize=sizeof(DDSURFACEDESC);
	ddsd.dwFlags=DDSD_CAPS|DDSD_BACKBUFFERCOUNT;
	ddsd.dwBackBufferCount=1;
	ddsd.ddsCaps.dwCaps=DDSCAPS_PRIMARYSURFACE|DDSCAPS_COMPLEX|DDSCAPS_FLIP;

	/*Creating the primary surface*/
	if(FAILED(lpdd->CreateSurface(&ddsd,&lpdds_Primary,NULL)))
		/*ERROR*/return E_FAIL;

	/*Creating the Back-Buffer surface*/
	ddsd.ddsCaps.dwCaps=DDSCAPS_BACKBUFFER;
	if(FAILED(lpdds_Primary->GetAttachedSurface(&ddsd.ddsCaps,&lpdds_Secondary)))
		/*ERROR*/return E_FAIL;

	Graphics_CreateBuffer();
	return S_OK;
}

HRESULT Graphics_CreateWindowedDisplay(HWND hwnd,DWORD width,DWORD height)
{
	DDSURFACEDESC ddsd;
	int x=(GetSystemMetrics(SM_CXSCREEN)-width)/2,y=(GetSystemMetrics(SM_CYSCREEN)-height)/2;
	RECT r;

	/*Setting the Width and Height*/
	Width=width;
	Height=height;

	/*Creating Direct Draw Object*/
	if(FAILED(DirectDrawCreate(NULL,&lpdd,NULL)))
		/*ERROR*/return E_FAIL;

	/*Setting cooperative level to window mode*/
	if(FAILED(lpdd->SetCooperativeLevel(hwnd,DDSCL_NORMAL)))
		/*ERROR*/return E_FAIL;

	ZeroMemory(&ddsd,sizeof(ddsd));
	ddsd.dwSize=sizeof(ddsd);
	ddsd.dwFlags=DDSD_CAPS;
	ddsd.ddsCaps.dwCaps=DDSCAPS_PRIMARYSURFACE;

	/*Creating the primary surface*/
	if(FAILED(lpdd->CreateSurface(&ddsd,&lpdds_Primary,NULL)))
		/*ERROR*/return E_FAIL;

	/*Creating the Off-Screen surface*/
	ZeroMemory(&ddsd,sizeof(ddsd));
	ddsd.dwSize=sizeof(ddsd);
	ddsd.dwFlags=DDSD_CAPS|DDSD_WIDTH|DDSD_HEIGHT;
	ddsd.dwWidth=width;
	ddsd.dwHeight=height;
	ddsd.ddsCaps.dwCaps=DDSCAPS_OFFSCREENPLAIN|DDSCAPS_VIDEOMEMORY;

	if(FAILED(lpdd->CreateSurface(&ddsd,&lpdds_Secondary,NULL)))
		return E_FAIL;

	/*Setting Window Rectangle*/
	SetRect(&r,x,y,x+width,y+height);

	/*Adjusting Client Area rectangle*/
	AdjustWindowRectEx(&r,GetWindowStyle(hwnd),GetMenu(hwnd)!=NULL,GetWindowExStyle(hwnd));
	MoveWindow(hwnd,r.left,r.top,r.right-r.left,r.bottom-r.top,FALSE);

	/*Creating the clipper and associating it to the window*/
	if(FAILED(lpdd->CreateClipper(0,&lpclipper,NULL)))
		return E_FAIL;
	if(FAILED(lpclipper->SetHWnd(0,hwnd)))
	{
		lpclipper->Release();
		return E_FAIL;
	}
	if(FAILED(lpdds_Primary->SetClipper(lpclipper)))
	{
		lpclipper->Release();
		return E_FAIL;
	}

	Graphics_CreateBuffer();
	return S_OK;
}

void Graphics_CreateBuffer()
{
	DDSURFACEDESC ddsd;

	/*Getting Secondary Surface Description*/
	ZeroMemory(&ddsd,sizeof(ddsd));
	ddsd.dwSize=sizeof(ddsd);

	lpdds_Secondary->GetSurfaceDesc(&ddsd);
	/*Pixel Depth*/
	bitCount=ddsd.ddpfPixelFormat.dwRGBBitCount>>3;

	/*Allocate Memory for VideBuffer*/
	VideoBuffer = (unsigned char*)malloc(Width*Height*bitCount);
	memset(VideoBuffer,192,Width*Height*bitCount);
}

void Graphics_FreeDirectDraw()
{
	/*Check if pointer is valid*/
	if(lpdd)
	{
		/*Restore normal window and default resolution and depth*/
		lpdd->SetCooperativeLevel(GlobalHwnd,DDSCL_NORMAL);
		lpdd->RestoreDisplayMode();
	}

	/*Releasing the clipper (if it exists)*/
	SAFE_RELEASE(lpclipper);

	/*Releasing the Secondary Surface (if it exists)*/
	SAFE_RELEASE(lpdds_Secondary);

	/*Releasing the Primary Surface (if it exists)*/
	SAFE_RELEASE(lpdds_Primary);

	/*Free Direct Draw object*/
	SAFE_RELEASE(lpdd);

	/*Free VideoBuffer if it's pointer is valid*/
	if(VideoBuffer){free(VideoBuffer);VideoBuffer=NULL;}
}

void Graphics_Flip()
{
	if(lpdds_Primary&&lpdds_Secondary)
		if(isWindowed)
		{
			/*If in Windowed mode, we need to Blit*/
			RECT client_rect;
			GetClientRect(GlobalHwnd,&client_rect);
			/* Modifying the client rect because of the toolbar */
			client_rect.top+=TOOLBAR_HEIGHT;
			client_rect.bottom+=TOOLBAR_HEIGHT;
			ClientToScreen(GlobalHwnd,(POINT*)&client_rect);
			ClientToScreen(GlobalHwnd,(POINT*)&client_rect+1);
			lpdds_Primary->Blt(&client_rect,lpdds_Secondary,NULL,DDBLT_WAIT,NULL);
		}
		else
			/*If in Full Screen mode, we only need to Flip*/
			lpdds_Primary->Flip(NULL,DDFLIP_WAIT);
}

void Graphics_TextOut(int x,int y,const char *str)
{
	HDC hdc;

	if(lpdds_Secondary)
	{
		/*Get Handle to Device Context from secondary surface*/
		lpdds_Secondary->GetDC(&hdc);

		/*Set text background and color*/
		SetBkMode(hdc,TRANSPARENT);
		SetTextColor(hdc,RGB(0,0,220));

		/*Output text to surface*/
		TextOut(hdc,x,y,str,(int)strlen(str));

		/*Release Handle to Device Context*/
		lpdds_Secondary->ReleaseDC(hdc);
	}
}

void Graphics_Draw(void(*drawingFn)())
{
	int i;
	int lPitch;
	unsigned char *buffer=NULL;
	unsigned char *tempBuffer=VideoBuffer;

	if(lpdds_Secondary)
	{
		DDSURFACEDESC ddsd;
		ZeroMemory(&ddsd,sizeof(ddsd));
		ddsd.dwSize=sizeof(ddsd);

		/*Lock the Secondary Surface*/
		lpdds_Secondary->Lock(NULL,&ddsd,DDLOCK_SURFACEMEMORYPTR|DDLOCK_WAIT,NULL);

		/*Set the Surface memory pointer and the Pitch*/
		buffer=(unsigned char*)ddsd.lpSurface;
		lPitch=ddsd.lPitch;

		if(drawingFn)
			drawingFn();

		if(lPitch==bitCount*Width)
			/*Block Copy*/
			memcpy((void*)buffer,(void*)tempBuffer,Width*Height*bitCount);
		else
		{
			for(i=0;i<Height;i++)
			{
				/*Line by Line Copy*/
				memcpy((void*)buffer,(void*)tempBuffer,Width*bitCount);
				buffer+=lPitch;
				tempBuffer+=bitCount*Width;
			}
		}
		/*Unlock Surface*/
		lpdds_Secondary->Unlock(NULL);
	}
}

void Grid(unsigned int square_width, unsigned int square_height, unsigned int dash_spacing, unsigned char r, unsigned char g, unsigned char b, unsigned char axis_r, unsigned char axis_g, unsigned char axis_b)
{
	unsigned int i=0;
	int x0=0, y0=0, x1=Width-1, y1=Height-1;

	for(i=0; i<Height/square_height; i++)			//drawing horizontally
	{
		for(x0=0; x0<x1; x0+=dash_spacing)
			Graphics_WritePixel(x0, y0, r, g, b);
		y0+=square_height;
	}
	x0=0;
	for(i=0; i<Width/square_width; i++)			//drawing vertically
	{
		for(y0=0; y0<y1; y0+=dash_spacing)
			Graphics_WritePixel(x0, y0, r, g, b);
		x0+=square_width;
	}
	for(x0=0; x0<x1; x0++)					//draw the horizontal x-axis
		Graphics_WritePixel(x0, Height/2, axis_r, axis_g, axis_b);
	for(y0=0; y0<y1; y0++)					//draw the vertical y-axis
		Graphics_WritePixel(Width/2, y0, axis_r, axis_g, axis_b);
}

void Reset_Surface(void)
{
	/* Clearing the surface */
	memset(VideoBuffer,192,Width*Height*bitCount);
	/* re-drawing the grid */
	Grid(20,20,5,128,128,128,128,128,128);
	/* re-draw the rects */
	if(ShowRects)
	{
		/* Drawing the rectangles */
		DrawRect(SourceRect, 255, 0, 0);
		DrawRect(DestinationRect, 0, 0, 255);
	}
}

void Graphics_Refresh(void)
{
	/*Refresh*/
	Graphics_Draw(NULL);
	Graphics_Flip();
}

Color_RGB GetPixelColor(int x, int y)
{
	unsigned char *mem = VideoBuffer + (x+y*Width)*bitCount;
	Color_RGB color={0};

	switch(bitCount)
	{
		//case 2:
		//	return (*mem + ((*++mem)<<8));

		case 3:
		case 4:
			color.b = *mem;
			color.g = *++mem;
			color.r = *++mem;
			return color;
	}
	return color;
}

void DrawRect (RECT rect, unsigned char r, unsigned char g, unsigned char b)
{
	/* upper edge */
	COLLISION_LINE((float)rect.left, (float)rect.top, (float)rect.right, (float)rect.top, r, g, b);
	/* left edge */
	COLLISION_LINE((float)rect.left, (float)rect.top, (float)rect.left, (float)rect.bottom, r, g, b);
	/* bottom edge */
	COLLISION_LINE((float)rect.left, (float)rect.bottom, (float)rect.right, (float)rect.bottom, r, g, b);
	/* right edge */
	COLLISION_LINE((float)rect.right, (float)rect.bottom, (float)rect.right, (float)rect.top, r, g, b);
}

void Preview_Color(RECT r,COLORREF Color)
{
	int i,j;
	/*Get Device Context Of Entire Screen*/
	HDC hdc=GetWindowDC(NULL);
	/*Draw Bounding Rectangle*/
	Rectangle(hdc,r.left-1,r.top-1,r.right+1,r.bottom+1);
	for(i=r.left;i<r.right;i++)
		for(j=r.top;j<r.bottom;j++)
			SetPixel(hdc,i,j,Color);

	/*Release Device Context*/
	ReleaseDC(NULL,hdc);
}
/*Primitive Rasterization functions*/
/*Line Clipper*/
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
/*Line Rasterizer*/
void LINE(float x0,float y0,float x1,float y1,unsigned int r,unsigned int g,unsigned int b)
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

	Graphics_WritePixel(X0,Y0,r,g,b);
	Graphics_WritePixel(X1,Y1,r,g,b);

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
			Graphics_WritePixel(X0,Y0,r,g,b);
			Graphics_WritePixel(X1,Y1,r,g,b);
		}
		if(PixelLeft)
		{
			X0+=xStep;
			if(d0>=0)
				Y0+=yStep;
			Graphics_WritePixel(X0,Y0,r,g,b);
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
			Graphics_WritePixel(X0,Y0,r,g,b);
			Graphics_WritePixel(X1,Y1,r,g,b);
		}
		if(PixelLeft)
		{
			Y0+=yStep;
			if(d0>=0)
				X0+=xStep;
			Graphics_WritePixel(X0,Y0,r,g,b);
		}
	}
}

/*Circle Rasterizer*/
void CIRCLE(float xc,float yc, int R,unsigned char r,unsigned char g,unsigned char b)
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
		Graphics_WritePixel(X+Xc,Y+Yc,r,g,b);

	if(Pixel_Clip(-X+Xc,Y+Yc))
		Graphics_WritePixel(-X+Xc,Y+Yc,r,g,b);

	if(Pixel_Clip(X+Xc,-Y+Yc))
		Graphics_WritePixel(X+Xc,-Y+Yc,r,g,b);

	if(Pixel_Clip(-X+Xc,-Y+Yc))
		Graphics_WritePixel(-X+Xc,-Y+Yc,r,g,b);

	if(Pixel_Clip(Y+Xc,X+Yc))
		Graphics_WritePixel(Y+Xc,X+Yc,r,g,b);

	if(Pixel_Clip(-Y+Xc,X+Yc))
		Graphics_WritePixel(-Y+Xc,X+Yc,r,g,b);

	if(Pixel_Clip(Y+Xc,-X+Yc))
		Graphics_WritePixel(Y+Xc,-X+Yc,r,g,b);

	if(Pixel_Clip(-Y+Xc,-X+Yc))
		Graphics_WritePixel(-Y+Xc,-X+Yc,r,g,b);


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
			Graphics_WritePixel(X+Xc,Y+Yc,r,g,b);

		if(Pixel_Clip(-X+Xc,Y+Yc))
			Graphics_WritePixel(-X+Xc,Y+Yc,r,g,b);

		if(Pixel_Clip(X+Xc,-Y+Yc))
			Graphics_WritePixel(X+Xc,-Y+Yc,r,g,b);

		if(Pixel_Clip(-X+Xc,-Y+Yc))
			Graphics_WritePixel(-X+Xc,-Y+Yc,r,g,b);

		if(Pixel_Clip(Y+Xc,X+Yc))
			Graphics_WritePixel(Y+Xc,X+Yc,r,g,b);

		if(Pixel_Clip(-Y+Xc,X+Yc))
			Graphics_WritePixel(-Y+Xc,X+Yc,r,g,b);

		if(Pixel_Clip(Y+Xc,-X+Yc))
			Graphics_WritePixel(Y+Xc,-X+Yc,r,g,b);

		if(Pixel_Clip(-Y+Xc,-X+Yc))
			Graphics_WritePixel(-Y+Xc,-X+Yc,r,g,b);
	}
}

/*Filled Circle Rasterizer*/
void FILL_CIRCLE(float xc,float yc, int R,unsigned char r,unsigned char g,unsigned char b)
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
			Graphics_WritePixel(x,y,r,g,b);
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
				Graphics_WritePixel(x,y,r,g,b);
		}

		if((y=X+Yc)>=ClipRect.top && y<ClipRect.bottom)
		{
			x=-Y+Xc;if(x<ClipRect.left){x=ClipRect.left;};
			xEnd=Y+Xc;if(xEnd>ClipRect.right-1){xEnd=ClipRect.right-1;};

			for(;x<=xEnd;x++)
				Graphics_WritePixel(x,y,r,g,b);
		}

		if((y=-Y+Yc)>=ClipRect.top && y<ClipRect.bottom)
		{
			x=-X+Xc;if(x<ClipRect.left){x=ClipRect.left;};
			xEnd=X+Xc;if(xEnd>ClipRect.right-1){xEnd=ClipRect.right-1;};

			for(;x<=xEnd;x++)
				Graphics_WritePixel(x,y,r,g,b);
		}

		if((y=-X+Yc)>=ClipRect.top && y<ClipRect.bottom)
		{
			x=-Y+Xc;if(x<ClipRect.left){x=ClipRect.left;};
			xEnd=Y+Xc;if(xEnd>ClipRect.right-1){xEnd=ClipRect.right-1;};

			for(;x<=xEnd;x++)
				Graphics_WritePixel(x,y,r,g,b);
		}
	}
}

long Ceil(float g)
{
	/*Function to return the ceiling of a float*/
	long l=(long)g;
	return ((l==g)|(g<0))?l:l+1;
}

void TRIANGLE(Point_Float *PF, unsigned char r,unsigned char g,unsigned char b)
{
	int i, j;
	for(i=0, j=1; i<3; i++, j++)
	{
		if(j==3)
			j=0;
		Point_Float V0=PF[i],V1=PF[j];
		if(LB_Clipp(&V0,&V1))
			LINE(V0.x, V0.y, V1.x, V1.y,r,g,b);
	}
}


void FILL_TRIANGLE(Point_Float *triangleVtx,unsigned char r,unsigned char g,unsigned char b)
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
			Graphics_WritePixel(x,y,r,g,b);

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
			Graphics_WritePixel(x,y,r,g,b);

		xL+=InvSlope[leftEdge];
		xR+=InvSlope[rightEdge];
	}
}

/*Collision Line*/
void COLLISION_LINE(float x0,float y0,float x1,float y1,unsigned int r,unsigned int g,unsigned int b)
{
	/*Declaring and defining all variables needed for algorithm*/
	int xStep,yStep,dx=(int)(x1+=0.5f)-(int)(x0+=0.5f),dy=(int)(y1+=0.5f)-(int)(y0+=0.5f),X0=(int)x0,Y0=(int)y0,X1=(int)x1,Y1=(int)y1;
	int PixelCount, PixelLeft;
	char Flip=0,mod=0;
	xStep=(dx>0)?1:-1;
	yStep=(dy>0)?1:-1;
	dx=(dx>0)?dx:-dx;
	dy=(dy>0)?dy:-dy;

	/*if both dx and dy are zero, return from function (nothing to draw)*/
	if(!dx&!dy)
		return;

	Graphics_WritePixel(X0,Y0,r,g,b);
	Graphics_WritePixel(X1,Y1,r,g,b);
	mod+=2;

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
			if(Flip)
			{
				Graphics_WritePixel(X0,Y0,r,g,b);
				Graphics_WritePixel(X1,Y1,r,g,b);
			}
			if(!(mod%COLLISION_SPACING))
				Flip=!Flip;
			mod+=2;
		}
		if(PixelLeft)
		{
			X0+=xStep;
			if(d0>=0)
				Y0+=yStep;
			Graphics_WritePixel(X0,Y0,r,g,b);
			mod++;
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
			if(Flip)
			{
				Graphics_WritePixel(X0,Y0,r,g,b);
				Graphics_WritePixel(X1,Y1,r,g,b);
			}
			if(!(mod%COLLISION_SPACING))
				Flip=!Flip;
			mod+=2;
		}
		if(PixelLeft)
		{
			Y0+=yStep;
			if(d0>=0)
				X0+=xStep;
			Graphics_WritePixel(X0,Y0,r,g,b);
			mod++;
		}
	}
}

/*Circle Rasterizer*/
void COLLISION_CIRCLE(float xc,float yc, int R,unsigned char r,unsigned char g,unsigned char b)
{
	/*Defining all needed variables*/
	int Xc=(int)(xc+0.5f),Yc=(int)(yc+0.5f);
	int X=0,Y=R,d=1-R;
	char Flip=0,mod=0;

	/*Draw first 8 points on circle (8 way symmetric)*/
	/*
		Note that if coordinates are greater than the buffersize or less than zero, point will not be added to avoid run-time error
		Instead point (Xc+R,Y) which is already on circle is added
	*/

	if(Pixel_Clip(X+Xc,Y+Yc))
		Graphics_WritePixel(X+Xc,Y+Yc,r,g,b);

	if(Pixel_Clip(-X+Xc,Y+Yc))
		Graphics_WritePixel(-X+Xc,Y+Yc,r,g,b);

	if(Pixel_Clip(X+Xc,-Y+Yc))
		Graphics_WritePixel(X+Xc,-Y+Yc,r,g,b);

	if(Pixel_Clip(-X+Xc,-Y+Yc))
		Graphics_WritePixel(-X+Xc,-Y+Yc,r,g,b);

	if(Pixel_Clip(Y+Xc,X+Yc))
		Graphics_WritePixel(Y+Xc,X+Yc,r,g,b);

	if(Pixel_Clip(-Y+Xc,X+Yc))
		Graphics_WritePixel(-Y+Xc,X+Yc,r,g,b);

	if(Pixel_Clip(Y+Xc,-X+Yc))
		Graphics_WritePixel(Y+Xc,-X+Yc,r,g,b);

	if(Pixel_Clip(-Y+Xc,-X+Yc))
		Graphics_WritePixel(-Y+Xc,-X+Yc,r,g,b);

	mod+=8;

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
		if(Flip)
		{
			if(Pixel_Clip(X+Xc,Y+Yc))
				Graphics_WritePixel(X+Xc,Y+Yc,r,g,b);

			if(Pixel_Clip(-X+Xc,Y+Yc))
				Graphics_WritePixel(-X+Xc,Y+Yc,r,g,b);

			if(Pixel_Clip(X+Xc,-Y+Yc))
				Graphics_WritePixel(X+Xc,-Y+Yc,r,g,b);

			if(Pixel_Clip(-X+Xc,-Y+Yc))
				Graphics_WritePixel(-X+Xc,-Y+Yc,r,g,b);

			if(Pixel_Clip(Y+Xc,X+Yc))
				Graphics_WritePixel(Y+Xc,X+Yc,r,g,b);

			if(Pixel_Clip(-Y+Xc,X+Yc))
				Graphics_WritePixel(-Y+Xc,X+Yc,r,g,b);

			if(Pixel_Clip(Y+Xc,-X+Yc))
				Graphics_WritePixel(Y+Xc,-X+Yc,r,g,b);

			if(Pixel_Clip(-Y+Xc,-X+Yc))
				Graphics_WritePixel(-Y+Xc,-X+Yc,r,g,b);
		}
		if(!(mod%COLLISION_SPACING))
			Flip=!Flip;
		mod+=8;
	}
}

int Sort_Y(const void* arg1, const void* arg2)
{
	return ((struct Point_Float*)arg1)->y - ((struct Point_Float*)arg2)->y>=0;
}

void SortVertices(Point_Float *triangleVtx)
{
	float dx,dy,B;
	qsort(triangleVtx,3,sizeof(Point_Float),Sort_Y);
	dx=triangleVtx[2].x-triangleVtx[0].x;
	dy=triangleVtx[2].y-triangleVtx[0].y;
	B=triangleVtx[0].y-(dy/dx)*triangleVtx[0].x;

	/*Determining Middle Vertex Position position with respect to top-bot line and swapping as necessary*/
	if(dy*triangleVtx[1].x-dx*triangleVtx[1].y+B*dx>0)
	{
		Point_Float tmp=triangleVtx[1];
		triangleVtx[1]=triangleVtx[2];
		triangleVtx[2]=tmp;
	}
}