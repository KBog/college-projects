#include "windows.h"		//Include the Windows library
#include "Direct.h"
#include "Resource.h"		//Include the Resource Header
#include "Character.h"
#include "Code Generation.h"
#include <math.h>

extern HWND hwndDlg;
RECT srcRect,destRect;
extern char Dir[250];
void Generate_Main(Game_Settings GameSettings)
{
	/*Create a folder for the generated code in the same working directory as the editor executable*/
	/*Get Temp Current Working Directory*/
	_getcwd(Dir,250);
	CreateDirectory("Generated Code",NULL);
	SetCurrentDirectory("Generated Code");
	/*Main output file stream*/
	ofstream Main("main.cpp",ios_base::out);
	/*Header Includes*/
	Main<<"#include<windows.h>\n#include<stdio.h>\n#include\"Graphics.h\"\n#include\"FrameRateController.h\"\n#include\"Settings.h\"\n#include\"Keyboard.h\"\n#include\"Mouse.h\""<<endl;
	/*Variables*/
	Main<<"\nHWND GlobalHwnd;\nstatic char FrameRateStr[32]={0};\n"<<endl;
	/*Window Procedure*/
	Main<<"LRESULT CALLBACK WndProc(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam)\n";
	Main<<"{\n\tswitch(message)\n\t{\n\t\tcase WM_CREATE:\n\t\t\t\t\t\t/*Create Windowed Display at program start-up*/\n\t\t\t\t\t\tGraphics_CreateWindowedDisplay(hwnd,WINDOW_WIDTH,WINDOW_HEIGHT);\n\t\t\t\t\t\treturn 0;\n";
	Main<<"\t\tcase WM_KEYDOWN:\n\t\t\t\t\t\tswitch(wParam)\n\t\t\t\t\t\t{\n\t\t\t\t\t\t\tcase VK_ESCAPE:\n\t\t\t\t\t\t\t\t\t\t\tPostQuitMessage(0);\n\t\t\t\t\t\t\t\t\t\t\tbreak;\n\t\t\t\t\t\t}\n\t\t\t\t\t\treturn 0;\n";
	Main<<"\t\tcase WM_DESTROY:\n\t\t\t\t\t\t/*Free DDRAW object*/\n\t\t\t\t\t\tGraphics_FreeDirectDraw();\n\t\t\t\t\t\tPostQuitMessage(0);\n\t\t\t\t\t\treturn 0;\n\t}"<<endl;
	Main<<"\treturn DefWindowProc(hwnd,message,wParam,lParam);\n}\n"<<endl;
	/*Win Main*/
	Main<<"int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nCmdShow)//WinMain() function declaration\n{"<<endl;
	/*Variables*/Main<<"\tATOM cID;				//Declare an ATOM variable to store the registered class ID\n\tWNDCLASSEXA wndclass;	//Declare a window class variable\n\tHWND hwnd;				//Declare a handle window variable\n\tMSG msg;				//Declare a MSG variable for messages\n"<<endl;
	/*Window Class*/Main<<"\t/*Initialize the wndClass structure*/\n\twndclass.cbSize=sizeof(WNDCLASSEXA);				//set size of structure\n\twndclass.style=CS_HREDRAW|CS_VREDRAW;				//set class style\n\twndclass.cbClsExtra=0;\n\twndclass.cbWndExtra=0;\n\twndclass.hInstance=hInstance;						//set handle Instance of window\n\twndclass.lpfnWndProc=WndProc;\t\t\t\t\t\t//set Window Procedure function\n\twndclass.hIcon=LoadIcon(NULL,IDI_APPLICATION);		//set default Icon\n\twndclass.hIconSm=LoadIcon(NULL,IDI_APPLICATION);	//set default Small Icon\n\twndclass.hCursor=LoadCursor(NULL,IDC_ARROW);		//set default Cursor\n\twndclass.hbrBackground=(HBRUSH)GetStockObject(WHITE_BRUSH);	//set background color\n\twndclass.lpszMenuName=NULL;							//No menu\n\twndclass.lpszClassName=\"Team Name\";\t\t			//Set Class name\n"<<endl;
	/*Register Class*/Main<<"\tcID=RegisterClassEx(&wndclass);						//Register the class and set the return value to cID ATOM\n"<<endl;
	/*Create Window*/Main<<"\t/*Call create window functionto create the program's window*/\n\tGlobalHwnd=hwnd=CreateWindow(wndclass.lpszClassName,\"\","<<GameSettings.Style<<",(GetSystemMetrics(SM_CXSCREEN)-WINDOW_WIDTH)/2,(GetSystemMetrics(SM_CYSCREEN)-WINDOW_HEIGHT)/2,WINDOW_WIDTH,WINDOW_HEIGHT,NULL,NULL,hInstance,NULL);\n\n\tif(!hwnd)\n\t\treturn 0;\n"<<endl;
	/*Show Window*/Main<<"\tShowWindow(hwnd,nCmdShow);							//Show the window after it has been created\n\tUpdateWindow(hwnd);									//Update the window\n"<<endl;
	/*Init Frame Rate*/Main<<"\tFRC_Init();											//Initialize frame rate\n"<<endl;
	/*Game Loop*/Main<<"\t/*Game Loop*/\n\twhile(1)\n\t{\n\t\tFRC_Start();\n\n\t\t/*Get Messages in queue*/\n\t\twhile(PeekMessage(&msg,NULL,0,0,PM_REMOVE))\n\t\t{\n\t\t\tif(msg.message==WM_QUIT)\n\t\t\t\tgoto End;\n\t\t\telse\n\t\t\t{\n\t\t\t\tTranslateMessage(&msg);\n\t\t\t\tDispatchMessage(&msg);\n\t\t\t}\n\t\t}\n"<<endl;
				 Main<<"\t\tKeyboard_Update();\n\t\tMouse_Update();\n"<<endl;
				 Main<<"\t\t//Client_Handle(Object_List);\n\t\tInit_Buffer();\n\t\t//Client_Draw(Object_List);\n\t\tInit_MainView();\n\t\tInit_RadarView();\n"<<endl;
				 Main<<"\t\t/*Copy buffer to Video RAM*/\n\t\tGraphics_Draw(NULL);\n\n\t\t/*Display FrameRate*/\n\t\tGraphics_TextOut(720,0,FrameRateStr);\n\t\tGraphics_Flip();\n\n\t\tFRC_End();\n\n\t\t/*Get Frame Rate String*/\n\t\tFrameRateStr[0]=\'\\0\';\n\t\tsprintf(FrameRateStr,\"FPS = %.2f\",FRC_GetCurrentFrameRate());\n\t}\n"<<endl;
				 Main<<"End:\n\tFRC_ShutDown();\n\tDestroyWindow(hwnd);\n\treturn (int)msg.wParam;\n}"<<endl;
	Main.close();
}

void Generate_GameSettings(Game_Settings GameSettings)
{
	ofstream Settings_Header("Settings.h",ios_base::out);
	/*Generating Settings header file based on user input*/
	Settings_Header<<"\n/*Define Window Dimensions and FrameRate*/"<<endl;
	/*Window Width*/Settings_Header<<"#define WINDOW_WIDTH	"<<GameSettings.Width<<endl;
	/*Window Height*/Settings_Header<<"#define WINDOW_HEIGHT	"<<GameSettings.Height<<endl;
	/*Frame Rate*/Settings_Header<<"#define FRAMERATE		"<<GameSettings.FrameRate<<endl;
	Settings_Header<<endl;

	/*Defining Key State Structure*/
	Settings_Header<<"/*Structure specifying all different key states*/"<<endl;
	Settings_Header<<"typedef struct"<<endl;
	Settings_Header<<"{\n\tunsigned char m_IsPressed;\n\tunsigned char m_IsTriggered;\n\tunsigned char m_WasPressed;\n}KeyState;"<<endl;
	Settings_Header.close();
}

void Generate_Graphics(void)
{
	/*Graphics Header*/
	/////////////////////////////////////////////////////////////////////////////////////////
	ofstream Graphics_Header("Graphics.h",ios_base::out);
	/*Includes*/Graphics_Header<<"#include\"Windowsx.h\"\n#include\"ddraw.h\"\n"<<endl;
	/*Externals*/Graphics_Header<<"extern DWORD bitCount;\nextern char isWindowed;\nextern int Width,Height;\nextern RECT ClipRect;\nextern unsigned char *VideoBuffer;\n"<<endl;
	/*Macros*/Graphics_Header<<"/*Defining Write_Pixel Macro*/\n#define Graphics_WritePixel(x,y,r,g,b)\\\n{\\\n\tunsigned char *mem=VideoBuffer + ((x)+Width*(y))*bitCount;\\\n\t/*Write Pixel depending on the bitCount*/\\\n\tswitch(bitCount)\\\n\t{\\\n\t\tcase 2:\\\n\t\t\t{\\\n\t\t\t\tWORD color=RGB16(r,g,b);\\\n\t\t\t\t*mem=(unsigned char)color;\\\n\t\t\t\t*++mem=(unsigned char)(color>>8);\\\n\t\t\t\tbreak;\\\n\t\t\t}\\\n\t\tcase 3:\\\n\t\t\t\t*mem=b;\\\n\t\t\t\t*++mem=g;\\\n\t\t\t\t*++mem=r;\\\n\t\t\t\tbreak;\\\n\t\tcase 4:\\\n\t\t\t\t*mem=b;\\\n\t\t\t\t*++mem=g;\\\n\t\t\t\t*++mem=r;\\\n\t\t\t\t*++mem=0;\\\n\t\t\t\tbreak;\\\n\t}\\\n}\n"<<endl;
			  Graphics_Header<<"/*Macros Definitions*/\n#define SAFE_RELEASE(p) {if(p){(p)->Release();(p)=NULL;}}\n"<<endl;
			  Graphics_Header<<"#define RGB16(r,g,b) ((b>>3)|((g>>2)<<5)|((r>>3)<<11))\n#define RGB32(a,r,g,b) ((b)+((g)<<8)+((r)<<16)+((a)<<24))\n"<<endl;
	/*Type Definitions*/Graphics_Header<<"/*Point Float Structure*/\n#ifndef _POINT_FLOAT_\n#define _POINT_FLOAT_\ntypedef struct Point_Float\n{\n\tfloat x,y;\n}Point_Float;\n#endif\n"<<endl;
	/*Prototypes*/Graphics_Header<<"/*Function Definitions*/\nvoid Graphics_Flip();\nvoid Graphics_CreateBuffer();\nvoid Graphics_FreeDirectDraw();\nvoid Graphics_TextOut(int x,int y,const char *str);\nvoid Graphics_Draw(void (*)());\nHRESULT Graphics_CreateFullScreenDisplay(HWND hwnd,WORD bdp);\nHRESULT Graphics_CreateWindowedDisplay(HWND hwnd,DWORD width,DWORD height);\nvoid Init_Buffer(void);\nvoid Init_MainView(void);\nvoid Init_RadarView(void);\nBOOL Pixel_Clip(int x,int y);\n\n/*Primitive Rasterization Functions*/\nvoid LINE(float x0,float y0,float x1,float y1,unsigned int r,unsigned int g,unsigned int b);\nvoid CIRCLE(float xc,float yc, int R,unsigned char r,unsigned char g,unsigned char b);\nvoid FILL_CIRCLE(float xc,float yc, int R,unsigned char r,unsigned char g,unsigned char b);\nvoid TRIANGLE(Point_Float *PF, unsigned char r,unsigned char g,unsigned char b);\nvoid FILL_TRIANGLE(Point_Float *triangleVtx,unsigned char r,unsigned char g,unsigned char b);"<<endl;
	Graphics_Header.close();
	/////////////////////////////////////////////////////////////////////////////////////////

	/*Graphics Source File*/
	/////////////////////////////////////////////////////////////////////////////////////////
	ofstream Graphics("Graphics.cpp",ios_base::out);
	/*Includes*/Graphics<<"#include\"Clipping.h\"\n#include\"Settings.h\"\n#include\"View.h\"\n"<<endl;
	/*Externals*/Graphics<<"extern HWND GlobalHwnd;\nextern ViewPort MainViewPort,RadarViewPort;\n"<<endl;
	/*Variables*/Graphics<<"DWORD bitCount=0;\nchar isWindowed=1;\nint Width,Height;\nunsigned char *VideoBuffer=NULL;\n\nLPDIRECTDRAW lpdd=NULL;					//Direct Draw object\nLPDIRECTDRAWSURFACE lpdds_Primary=NULL;	//Primary Surface\nLPDIRECTDRAWSURFACE lpdds_Secondary=NULL;//Secondary Surface\nLPDIRECTDRAWCLIPPER lpclipper=NULL;		//Clipper Object\n"<<endl;
	
	/*Full Screen Display function*/
	Graphics<<"HRESULT Graphics_CreateFullScreenDisplay(HWND hwnd,WORD bdp)\n";
	Graphics<<"{\n\tDDSURFACEDESC ddsd;\n\n\t/*Get System Resolution*/\n\tWidth=GetSystemMetrics(SM_CXSCREEN);\n\tHeight=GetSystemMetrics(SM_CYSCREEN);\n\n\t/*Creating Direct Draw Object*/\n\tif(FAILED(DirectDrawCreate(NULL,&lpdd,NULL)))\n\t\t/*ERROR*/return E_FAIL;\n\n\t/*Setting cooperative level to full screen mode*/\n\tif(FAILED(lpdd->SetCooperativeLevel(hwnd,DDSCL_EXCLUSIVE|DDSCL_FULLSCREEN)))\n\t\t/*ERROR*/return E_FAIL;\n\n\t/*Setting the Display Mode*/\n\tif(FAILED(lpdd->SetDisplayMode(Width,Height,bdp)))\n\t\t/*ERROR*/return E_FAIL;\n\n\t/*Creating the surface and back-buffer*/\n\n\t/*Initializing the Direct Draw surface description*/\n\tZeroMemory(&ddsd,sizeof(ddsd));\n\tddsd.dwSize=sizeof(DDSURFACEDESC);\n\tddsd.dwFlags=DDSD_CAPS|DDSD_BACKBUFFERCOUNT;\n\tddsd.dwBackBufferCount=1;\n\tddsd.ddsCaps.dwCaps=DDSCAPS_PRIMARYSURFACE|DDSCAPS_COMPLEX|DDSCAPS_FLIP;\n\n\t/*Creating the primary surface*/\n\tif(FAILED(lpdd->CreateSurface(&ddsd,&lpdds_Primary,NULL)))\n\t\t/*ERROR*/return E_FAIL;\n\n\t/*Creating the Back-Buffer surface*/\n\tddsd.ddsCaps.dwCaps=DDSCAPS_BACKBUFFER;\n\tif(FAILED(lpdds_Primary->GetAttachedSurface(&ddsd.ddsCaps,&lpdds_Secondary)))\n\t\t/*ERROR*/return E_FAIL;\n\n\tGraphics_CreateBuffer();\n\treturn S_OK;\n}\n"<<endl;

	/*Windowed Display function*/
	Graphics<<"HRESULT Graphics_CreateWindowedDisplay(HWND hwnd,DWORD width,DWORD height)\n";
	Graphics<<"{\n\tDDSURFACEDESC ddsd;\n\tint x=(GetSystemMetrics(SM_CXSCREEN)-width)/2,y=(GetSystemMetrics(SM_CYSCREEN)-height)/2;\n\tRECT r;\n\n\t/*Setting the Width and Height*/\n\tWidth=width;\n\tHeight=height;\n\n\t/*Creating Direct Draw Object*/\n\tif(FAILED(DirectDrawCreate(NULL,&lpdd,NULL)))\n\t\t/*ERROR*/return E_FAIL;\n\n\t/*Setting cooperative level to window mode*/\n\tif(FAILED(lpdd->SetCooperativeLevel(hwnd,DDSCL_NORMAL)))\n\t\t/*ERROR*/return E_FAIL;\n\n\tZeroMemory(&ddsd,sizeof(ddsd));\n\tddsd.dwSize=sizeof(ddsd);\n\tddsd.dwFlags=DDSD_CAPS;\n\tddsd.ddsCaps.dwCaps=DDSCAPS_PRIMARYSURFACE;\n\n\t/*Creating the primary surface*/\n\tif(FAILED(lpdd->CreateSurface(&ddsd,&lpdds_Primary,NULL)))\n\t\t/*ERROR*/return E_FAIL;\n\n\t/*Creating the Off-Screen surface*/\n\tZeroMemory(&ddsd,sizeof(ddsd));\n\tddsd.dwSize=sizeof(ddsd);\n\tddsd.dwFlags=DDSD_CAPS|DDSD_WIDTH|DDSD_HEIGHT;\n\tddsd.dwWidth=width;\n\tddsd.dwHeight=height;\n\tddsd.ddsCaps.dwCaps=DDSCAPS_OFFSCREENPLAIN|DDSCAPS_VIDEOMEMORY;\n\n\tif(FAILED(lpdd->CreateSurface(&ddsd,&lpdds_Secondary,NULL)))\n\t\treturn E_FAIL;\n\n\t/*Setting Window Rectangle*/\n\tSetRect(&r,x,y,x+width,y+height);\n\n\t/*Adjusting Client Area rectangle*/\n\tAdjustWindowRectEx(&r,GetWindowStyle(hwnd),GetMenu(hwnd)!=NULL,GetWindowExStyle(hwnd));\n\tMoveWindow(hwnd,r.left,r.top,r.right-r.left,r.bottom-r.top,FALSE);\n\n\t/*Creating the clipper and associating it to the window*/\n\tif(FAILED(lpdd->CreateClipper(0,&lpclipper,NULL)))\n\t\treturn E_FAIL;\n\tif(FAILED(lpclipper->SetHWnd(0,hwnd)))\n\t{\n\t\tlpclipper->Release();\n\t\treturn E_FAIL;\n\t}\n\tif(FAILED(lpdds_Primary->SetClipper(lpclipper)))\n\t{\n\t\tlpclipper->Release();\n\t\treturn E_FAIL;\n\t}\n\n\tGraphics_CreateBuffer();\n\treturn S_OK;\n}\n"<<endl;

	/*Create Buffer function*/
	Graphics<<"void Graphics_CreateBuffer()\n";
	Graphics<<"{\n\tDDSURFACEDESC ddsd;\n\n\t/*Getting Secondary Surface Description*/\n\tZeroMemory(&ddsd,sizeof(ddsd));\n\tddsd.dwSize=sizeof(ddsd);\n\n\tlpdds_Secondary->GetSurfaceDesc(&ddsd);\n\t/*Pixel Depth*/\n\tbitCount=ddsd.ddpfPixelFormat.dwRGBBitCount>>3;\n\n\t/*Allocate Memory for VideBuffer*/\n\tVideoBuffer = (unsigned char*)malloc(Width*Height*bitCount);\n\tmemset(VideoBuffer,0xFF,Width*Height*bitCount);\n}\n"<<endl;

	/*Free DDRAW function*/
	Graphics<<"void Graphics_FreeDirectDraw()\n";
	Graphics<<"{\n\t/*Check if pointer is valid*/\n\tif(lpdd)\n\t{\n\t\t/*Restore normal window and default resolution and depth*/\n\t\tlpdd->SetCooperativeLevel(GlobalHwnd,DDSCL_NORMAL);\n\t\tlpdd->RestoreDisplayMode();\n\t}\n\n\t/*Releasing the clipper (if it exists)*/\n\tSAFE_RELEASE(lpclipper);\n\n\t/*Releasing the Secondary Surface (if it exists)*/\n\tSAFE_RELEASE(lpdds_Secondary);\n\n\t/*Releasing the Primary Surface (if it exists)*/\n\tSAFE_RELEASE(lpdds_Primary);\n\n\t/*Free Direct Draw object*/\n\tSAFE_RELEASE(lpdd);\n\n\t/*Free VideoBuffer if it's pointer is valid*/\n\tif(VideoBuffer){free(VideoBuffer);VideoBuffer=NULL;}\n}\n"<<endl;

	/*Graphics Flip function*/
	Graphics<<"void Graphics_Flip()\n";
	Graphics<<"{\n\tif(lpdds_Primary&&lpdds_Secondary)\n\t\tif(isWindowed)\n\t\t{\n\t\t\t/*If in Windowed mode, we need to Blit*/\n\t\t\tRECT client_rect;\n\t\t\tGetClientRect(GlobalHwnd,&client_rect);\n\t\t\tClientToScreen(GlobalHwnd,(POINT*)&client_rect);\n\t\t\tClientToScreen(GlobalHwnd,(POINT*)&client_rect+1);\n\t\t\tlpdds_Primary->Blt(&client_rect,lpdds_Secondary,NULL,DDBLT_WAIT,NULL);\n\t\t}\n\t\telse\n\t\t\t/*If in Full Screen mode, we only need to Flip*/\n\t\t\tlpdds_Primary->Flip(NULL,DDFLIP_WAIT);\n}\n"<<endl;

	/*Graphics Text Out function*/
	Graphics<<"void Graphics_TextOut(int x,int y,const char *str)\n";
	Graphics<<"{\n\tHDC hdc;\n\n\tif(lpdds_Secondary)\n\t{\n\t\t/*Get Handle to Device Context from secondary surface*/\n\t\tlpdds_Secondary->GetDC(&hdc);\n\n\t\t/*Set text background and color*/\n\t\tSetBkMode(hdc,TRANSPARENT);\n\t\tSetTextColor(hdc,RGB(0,0,220));\n\n\t\t/*Output text to surface*/\n\t\tTextOut(hdc,x,y,str,(int)strlen(str));\n\n\t\t/*Release Handle to Device Context*/\n\t\tlpdds_Secondary->ReleaseDC(hdc);\n\t}\n}\n"<<endl;

	/*Graphics Draw function*/
	Graphics<<"void Graphics_Draw(void(*drawingFn)())\n";
	Graphics<<"{\n\tint i;\n\tint lPitch;\n\tunsigned char *buffer=NULL;\n\tunsigned char *tempBuffer=VideoBuffer;\n\n\tif(lpdds_Secondary)\n\t{\n\t\tDDSURFACEDESC ddsd;\n\t\tZeroMemory(&ddsd,sizeof(ddsd));\n\t\tddsd.dwSize=sizeof(ddsd);\n\n\t\t/*Lock the Secondary Surface*/\n\t\tlpdds_Secondary->Lock(NULL,&ddsd,DDLOCK_SURFACEMEMORYPTR|DDLOCK_WAIT,NULL);\n\n\t\t/*Set the Surface memory pointer and the Pitch*/\n\t\tbuffer=(unsigned char*)ddsd.lpSurface;\n\t\tlPitch=ddsd.lPitch;\n\n\t\tif(drawingFn)\n\t\t\tdrawingFn();\n\n\t\tif(lPitch==bitCount*Width)\n\t\t\t/*Block Copy*/\n\t\t\tmemcpy((void*)buffer,(void*)tempBuffer,Width*Height*bitCount);\n\t\telse\n\t\t{\n\t\t\tfor(i=0;i<Height;i++)\n\t\t\t{\n\t\t\t\t/*Line by Line Copy*/\n\t\t\t\tmemcpy((void*)buffer,(void*)tempBuffer,Width*bitCount);\n\t\t\t\tbuffer+=lPitch;\n\t\t\t\ttempBuffer+=bitCount*Width;\n\t\t\t}\n\t\t}\n\t\t/*Unlock Surface*/\n\t\tlpdds_Secondary->Unlock(NULL);\n\t}\n}\n"<<endl;

	/*Init Buffer function*/
	Graphics<<"void Init_Buffer(void)\n";
	Graphics<<"{\n\t/*Initialize all VideoBuffer to Black*/\n\tmemset(VideoBuffer,0x00,Width*Height*bitCount);\n}\n"<<endl;
	
	/*Init Main View function*/
	Graphics<<"void Init_MainView(void)\n";
	Graphics<<"{\n\tint i,x0,y0;\n\tunsigned char *p;\n\n\t/*Drawing Main ViewPort*/\n\tx0=(int)MainViewPort.Pos_x-(((int)MainViewPort.Width)>>1);\n\ty0=(int)MainViewPort.Pos_y-(((int)MainViewPort.Height)>>1);\n\tp=&VideoBuffer[(x0+Width*y0)*bitCount];\n\t/*memset Line by Line*/\n\tfor(i=0;i<MainViewPort.Height;i++)\n\t{\n\t\tmemset(p,0xFF,(size_t)MainViewPort.Width*bitCount);\n\t\tp+=Width*bitCount;\n\t}\n}\n"<<endl;
	
	/*Init Radar View function*/
	Graphics<<"void Init_RadarView(void)\n";
	Graphics<<"{\n\tint i,x0,y0;\n\tunsigned char *p;\n\n\t/*Drawing Radar ViewPort*/\n\tx0=(int)RadarViewPort.Pos_x-(((int)RadarViewPort.Width)>>1);\n\ty0=(int)RadarViewPort.Pos_y-(((int)RadarViewPort.Height)>>1);\n\tp=&VideoBuffer[(x0+Width*y0)*bitCount];\n\t/*memset Line by Line*/\n\tfor(i=0;i<RadarViewPort.Height;i++)\n\t{\n\t\tmemset(p,0xFF,(size_t)RadarViewPort.Width*bitCount);\n\t\tp+=Width*bitCount;\n\t}\n}\n"<<endl;
	
	///*Pixel Function*/
	//Graphics<<"BOOL Pixel_Clip(int x,int y)\n";
	//Graphics<<"{\n\tif(x>ClipRect.left&&x<ClipRect.right&&y>ClipRect.top&&y<ClipRect.bottom)\n\t\treturn TRUE;\n\treturn FALSE;\n}"<<endl;
	/*Primitive Rasterization functions*/
	Graphics<<"/*Primitive Rasterization functions*/\n"<<endl;
	/*Line Rasterizer*/
	Graphics<<"/*Line Rasterizer*/"<<endl;
	Graphics<<"void LINE(float x0,float y0,float x1,float y1,unsigned int r,unsigned int g,unsigned int b)\n";
	Graphics<<"{\n\t/*Declaring and defining all variables needed for algorithm*/\n\tint xStep,yStep,dx=(int)(x1+=0.5f)-(int)(x0+=0.5f),dy=(int)(y1+=0.5f)-(int)(y0+=0.5f),X0=(int)x0,Y0=(int)y0,X1=(int)x1,Y1=(int)y1;\n\tint PixelCount, PixelLeft;\n\txStep=(dx>0)?1:-1;\n\tyStep=(dy>0)?1:-1;\n\tdx=(dx>0)?dx:-dx;\n\tdy=(dy>0)?dy:-dy;\n\n\t/*if both dx and dy are zero, return from function (nothing to draw)*/\n\tif(!dx&!dy)\n\t\treturn;\n\n\tGraphics_WritePixel(X0,Y0,r,g,b);\n\tGraphics_WritePixel(X1,Y1,r,g,b);\n\n\tif(dx>=dy)\n\t{\n\t\t/*Declare and Define Decision variables*/\n\t\tint d0=2*dy-dx;\n\t\tint dA=2*(dy-dx);\n\t\tint dB=2*dy;\n\n\t\tPixelCount=(dx-1)/2;\n\t\tPixelLeft=(dx-1)%2;\n\n\t\twhile(PixelCount--)\n\t\t{\n\t\t\tX0+=xStep;\n\t\t\tX1-=xStep;\n\t\t\tif(d0>=0)\n\t\t\t{\n\t\t\t\td0+=dA;\n\t\t\t\tY0+=yStep;\n\t\t\t\tY1-=yStep;\n\t\t\t}\n\t\t\telse\n\t\t\t\td0+=dB;\n\t\t\tGraphics_WritePixel(X0,Y0,r,g,b);\n\t\t\tGraphics_WritePixel(X1,Y1,r,g,b);\n\t\t}\n\t\tif(PixelLeft)\n\t\t{\n\t\t\tX0+=xStep;\n\t\t\tif(d0>=0)\n\t\t\t\tY0+=yStep;\n\t\t\tGraphics_WritePixel(X0,Y0,r,g,b);\n\t\t}\n\t}\n\telse\n\t{\n\t\t/*Declare and Define Decision variables*/\n\t\tint d0=2*dx-dy;\n\t\tint dA=2*(dx-dy);\n\t\tint dB=2*dx;\n\n\t\tPixelCount=(dy-1)/2;\n\t\tPixelLeft=(dy-1)%2;\n\n\t\twhile(PixelCount--)\n\t\t{\n\t\t\tY0+=yStep;\n\t\t\tY1-=yStep;\n\t\t\tif(d0>=0)\n\t\t\t{\n\t\t\t\td0+=dA;\n\t\t\t\tX0+=xStep;\n\t\t\t\tX1-=xStep;\n\t\t\t}\n\t\t\telse\n\t\t\t\td0+=dB;\n\t\t\tGraphics_WritePixel(X0,Y0,r,g,b);\n\t\t\tGraphics_WritePixel(X1,Y1,r,g,b);\n\t\t}\n\t\tif(PixelLeft)\n\t\t{\n\t\t\tY0+=yStep;\n\t\t\tif(d0>=0)\n\t\t\t\tX0+=xStep;\n\t\t\tGraphics_WritePixel(X0,Y0,r,g,b);\n\t\t}\n\t}\n}\n"<<endl;
	
	/*Circle Rasterizer*/
	Graphics<<"/*Circle Rasterizer*/"<<endl;
	Graphics<<"void CIRCLE(float xc,float yc, int R,unsigned char r,unsigned char g,unsigned char b)\n";
	Graphics<<"{\n\t/*Defining all needed variables*/\n\tint Xc=(int)(xc+0.5f),Yc=(int)(yc+0.5f);\n\tint X=0,Y=R,d=1-R;\n\n\t/*Draw first 8 points on circle (8 way symmetric)*/\n\n\tif(Pixel_Clip(X+Xc,Y+Yc))\n\t\tGraphics_WritePixel(X+Xc,Y+Yc,r,g,b);\n\tif(Pixel_Clip(-X+Xc,Y+Yc))\n\t\tGraphics_WritePixel(-X+Xc,Y+Yc,r,g,b);\n\tif(Pixel_Clip(X+Xc,-Y+Yc))\n\t\tGraphics_WritePixel(X+Xc,-Y+Yc,r,g,b);\n\tif(Pixel_Clip(-X+Xc,-Y+Yc))\n\t\tGraphics_WritePixel(-X+Xc,-Y+Yc,r,g,b);\n\tif(Pixel_Clip(Y+Xc,X+Yc))\n\t\tGraphics_WritePixel(Y+Xc,X+Yc,r,g,b);\n\tif(Pixel_Clip(-Y+Xc,X+Yc))\n\t\tGraphics_WritePixel(-Y+Xc,X+Yc,r,g,b);\n\tif(Pixel_Clip(Y+Xc,-X+Yc))\n\t\tGraphics_WritePixel(Y+Xc,-X+Yc,r,g,b);\n\tif(Pixel_Clip(-Y+Xc,-X+Yc))\n\t\tGraphics_WritePixel(-Y+Xc,-X+Yc,r,g,b);\n\n\twhile(X<Y)							//Loop while X is less than Y ==> dx>dy\n\t{\n\t\tif(d<0)							//if the decision variable is less than zero\n\t\t\td+=2*X+3;					//Update decision variable by 2*X+3\n\t\telse							//else\n\t\t{\n\t\t\td+=2*(X-Y)+5;				//Update decision variable by 2*(X-Y)+5\n\t\t\tY--;						//Decrement Y by 1\n\t\t}\n\t\tX++;							//Increment X by 1\n\n\t\t/*Draw all 8 symmetric points*/\n\t\tif(Pixel_Clip(X+Xc,Y+Yc))\n\t\t\tGraphics_WritePixel(X+Xc,Y+Yc,r,g,b);\n\t\tif(Pixel_Clip(-X+Xc,Y+Yc))\n\t\t\tGraphics_WritePixel(-X+Xc,Y+Yc,r,g,b);\n\t\tif(Pixel_Clip(X+Xc,-Y+Yc))\n\t\t\tGraphics_WritePixel(X+Xc,-Y+Yc,r,g,b);\n\t\tif(Pixel_Clip(-X+Xc,-Y+Yc))\n\t\t\tGraphics_WritePixel(-X+Xc,-Y+Yc,r,g,b);\n\t\tif(Pixel_Clip(Y+Xc,X+Yc))\n\t\t\tGraphics_WritePixel(Y+Xc,X+Yc,r,g,b);\n\t\tif(Pixel_Clip(-Y+Xc,X+Yc))\n\t\t\tGraphics_WritePixel(-Y+Xc,X+Yc,r,g,b);\n\t\tif(Pixel_Clip(Y+Xc,-X+Yc))\n\t\t\tGraphics_WritePixel(Y+Xc,-X+Yc,r,g,b);\n\t\tif(Pixel_Clip(-Y+Xc,-X+Yc))\n\t\t\tGraphics_WritePixel(-Y+Xc,-X+Yc,r,g,b);\n\t}\n}\n"<<endl;

	/*Filled Circle Rasterizer*/
	Graphics<<"/*Filled Circle Rasterizer*/"<<endl;
	Graphics<<"void FILL_CIRCLE(float xc,float yc, int R,unsigned char r,unsigned char g,unsigned char b)\n";
	Graphics<<"{\n\t/*Defining all needed variables*/\n\tint X=0,Y=R,Xc=(int)(xc+0.5f),Yc=(int)(yc+0.5f),d=1-R,de=3,dse=-2*R+5;\n\tint x,xEnd,y;\n\n\t/*Draw first horizontal diameter*/\n\tif((y=X+Yc)>=ClipRect.top && y<ClipRect.bottom)\n\t{\n\t\tx=-Y+Xc;if(x<ClipRect.left){x=ClipRect.left;};\n\t\txEnd=Y+Xc;if(xEnd>ClipRect.right-1){xEnd=ClipRect.right-1;};\n\n\t\tfor(;x<=xEnd;x++)\n\t\t\tGraphics_WritePixel(x,y,r,g,b);\n\t}\n\n\twhile(X<Y)\n\t{\n\t\tif(d<0)\n\t\t{\n\t\t\td+=de;\n\t\t\tde+=2;\n\t\t\tdse+=2;\n\t\t}\n\t\telse\n\t\t{\n\t\t\td+=dse;\n\t\t\tde+=2;\n\t\t\tdse+=4;\n\t\t\tY--;\n\t\t}\n\t\tX++;\n\n\t\t/*Draw all 4 lines*/\n\t\tif((y=Y+Yc)>=ClipRect.top && y<ClipRect.bottom)\n\t\t{\n\t\t\tx=-X+Xc;if(x<ClipRect.left){x=ClipRect.left;};\n\t\t\txEnd=X+Xc;if(xEnd>ClipRect.right-1){xEnd=ClipRect.right-1;};\n\n\t\t\tfor(;x<=xEnd;x++)\n\t\t\t\tGraphics_WritePixel(x,y,r,g,b);\n\t\t}\n\n\t\tif((y=X+Yc)>=ClipRect.top && y<ClipRect.bottom)\n\t\t{\n\t\t\tx=-Y+Xc;if(x<ClipRect.left){x=ClipRect.left;};\n\t\t\txEnd=Y+Xc;if(xEnd>ClipRect.right-1){xEnd=ClipRect.right-1;};\n\n\t\t\tfor(;x<=xEnd;x++)\n\t\t\t\tGraphics_WritePixel(x,y,r,g,b);\n\t\t}\n\n\t\tif((y=-Y+Yc)>=ClipRect.top && y<ClipRect.bottom)\n\t\t{\n\t\t\tx=-X+Xc;if(x<ClipRect.left){x=ClipRect.left;};\n\t\t\txEnd=X+Xc;if(xEnd>ClipRect.right-1){xEnd=ClipRect.right-1;};\n\n\t\t\tfor(;x<=xEnd;x++)\n\t\t\t\tGraphics_WritePixel(x,y,r,g,b);\n\t\t}\n\n\t\tif((y=-X+Yc)>=ClipRect.top && y<ClipRect.bottom)\n\t\t{\n\t\t\tx=-Y+Xc;if(x<ClipRect.left){x=ClipRect.left;};\n\t\t\txEnd=Y+Xc;if(xEnd>ClipRect.right-1){xEnd=ClipRect.right-1;};\n\n\t\t\tfor(;x<=xEnd;x++)\n\t\t\t\tGraphics_WritePixel(x,y,r,g,b);\n\t\t}\n\t}\n}\n"<<endl;
	
	/*Ceil Function*/
	Graphics<<"long Ceil(float g)"<<endl;
	Graphics<<"{\n\t/*Function to return the ceiling of a float*/\n\tlong l=(long)g;\n\treturn ((l==g)|(g<0))?l:l+1;\n}\n"<<endl;

	/*Triangle Rasterizer*/
	Graphics<<"/*Triangle Rasterizer*/"<<endl;
	Graphics<<"void TRIANGLE(Point_Float *PF, unsigned char r,unsigned char g,unsigned char b)\n";
	Graphics<<"{\n\tint i, j;\n\tfor(i=0, j=1; i<3; i++, j++)\n\t{\n\t\tif(j==3)\n\t\t\tj=0;\n\t\tPoint_Float V0=PF[i],V1=PF[j];\n\t\tif(LB_Clipp(&V0,&V1))\n\t\t\tLINE(V0.x, V0.y, V1.x, V1.y,r,g,b);\n\t}\n}\n"<<endl;
	

	/*Filled Triangle Rasterizer*/
	Graphics<<"/*Filled Triangle Rasterizer*/"<<endl;
	Graphics<<"void FILL_TRIANGLE(Point_Float *triangleVtx,unsigned char r,unsigned char g,unsigned char b)\n";
	Graphics<<"{\n\tchar topVtx,midVtx,botVtx,MidIsLeft,leftEdge,rightEdge;\n\tint y,yEnd,x,xEnd;\n\tfloat InvSlope[3],xL,xR;\n\n\t/*\n\t	Determining Positions of the three vertices and updating\n\t\ttopVtx, midVtx, botVtx and MidIsLeft variables accordingly\n\t*/\n\n\t///////////////////////////////////////////////\n\tif(triangleVtx[0].y<triangleVtx[1].y)\n\t{\n\t\tif(triangleVtx[2].y<triangleVtx[0].y)\n\t\t{\n\t\t\ttopVtx=2;\n\t\t\tmidVtx=0;\n\t\t\tbotVtx=1;\n\t\t\tMidIsLeft=1;\n\t\t}\n\t\telse\n\t\t{\n\t\t\ttopVtx=0;\n\t\t\tif(triangleVtx[1].y<triangleVtx[2].y)\n\t\t\t{\n\t\t\t\tmidVtx=1;\n\t\t\t\tbotVtx=2;\n\t\t\t\tMidIsLeft=1;\n\t\t\t}\n\t\t\telse\n\t\t\t{\n\t\t\t\tmidVtx=2;\n\t\t\t\tbotVtx=1;\n\t\t\t\tMidIsLeft=0;\n\t\t\t}\n\t\t}\n\t}\n\telse\n\t{\n\t\tif(triangleVtx[2].y<triangleVtx[1].y)\n\t\t{\n\t\t\ttopVtx=2;\n\t\t\tmidVtx=1;\n\t\t\tbotVtx=0;\n\t\t\tMidIsLeft=0;\n\t\t}\n\t\telse\n\t\t{\n\t\t\ttopVtx=1;\n\t\t\tif(triangleVtx[0].y<triangleVtx[2].y)\n\t\t\t{\n\t\t\t\tmidVtx=0;\n\t\t\t\tbotVtx=2;\n\t\t\t\tMidIsLeft=0;\n\t\t\t}\n\t\t\telse\n\t\t\t{\n\t\t\t\tmidVtx=2;\n\t\t\t\tbotVtx=0;\n\t\t\t\tMidIsLeft=1;\n\t\t\t}\n\t\t}\n\t}\n\t///////////////////////////////////////////////\n\t/*Updating Inverse Slope Values*/\n\tInvSlope[0]=(triangleVtx[botVtx].x-triangleVtx[topVtx].x)/(triangleVtx[botVtx].y-triangleVtx[topVtx].y);\n\tInvSlope[1]=(triangleVtx[midVtx].x-triangleVtx[topVtx].x)/(triangleVtx[midVtx].y-triangleVtx[topVtx].y);\n\tInvSlope[2]=(triangleVtx[botVtx].x-triangleVtx[midVtx].x)/(triangleVtx[botVtx].y-triangleVtx[midVtx].y);\n\n\t/*Updating the leftEdge and rightEdge Variables*/\n\tleftEdge=MidIsLeft;\n\trightEdge=!MidIsLeft;\n\n\t/*Getting starting and ending y values*/\n\ty=Ceil(triangleVtx[topVtx].y);\n\tyEnd=Ceil(triangleVtx[midVtx].y)-1;\n\n\t/*Getting Initial x values*/\n\txL=xR=triangleVtx[topVtx].x;\n\n\t/*Clipping First half of triangle by interpolating x and y*/\n\tif(y<=ClipRect.top)\n\t{\n\t\txL+=InvSlope[leftEdge]*(ClipRect.top-y);\n\t\txR+=InvSlope[rightEdge]*(ClipRect.top-y);\n\t\ty=ClipRect.top;\n\t}\n\tif(yEnd>=ClipRect.bottom-1)\n\t\tyEnd=ClipRect.bottom-1;\n\n\t";
	Graphics<<"/*Filling the triangle from Top to Mid vertex*/\n\tfor(;y<=yEnd;y++)\n\t{\n\t\tx=Ceil(xL);if(x<ClipRect.left){x=ClipRect.left;};\n\t\txEnd=Ceil(xR)-1;if(xEnd>ClipRect.right-1){xEnd=ClipRect.right-1;};\n\t\tfor(;x<=xEnd;x++)\n\t\t\tGraphics_WritePixel(x,y,r,g,b);\n\n\t\txL+=InvSlope[leftEdge];\n\t\txR+=InvSlope[rightEdge];\n\t}\n\n	/*Updating the leftEdge and rightEdge Variables*/\n\tleftEdge=MidIsLeft<<1;\n\trightEdge=(!MidIsLeft)<<1;\n\n\t/*Updating starting and ending y Values*/\n\ty=Ceil(triangleVtx[midVtx].y);\n\tyEnd=Ceil(triangleVtx[botVtx].y)-1;\n\n\t/*Clipping Second half of triangle by interpolating x and y*/\n\tif(MidIsLeft)\n\t{\n\t\txL=triangleVtx[midVtx].x;\n\t\tif(y<=ClipRect.top)\n\t\t{\n\t\t\txL=triangleVtx[midVtx].x+InvSlope[2]*(ClipRect.top-y);\n\t\t\txR=triangleVtx[topVtx].x+InvSlope[0]*(ClipRect.top-Ceil(triangleVtx[topVtx].y));\n\t\t\ty=ClipRect.top;\n\t\t}\n\t\tif(yEnd>=ClipRect.bottom-1)\n\t\t\tyEnd=ClipRect.bottom-1;\n\t}\n\telse\n\t{\n\t\txR=triangleVtx[midVtx].x;\n\t\tif(y<=ClipRect.top)\n\t\t{\n\t\t\txR=triangleVtx[midVtx].x+InvSlope[2]*(ClipRect.top-y);\n\t\t\txL=triangleVtx[topVtx].x+InvSlope[0]*(ClipRect.top-Ceil(triangleVtx[topVtx].y));\n\t\t\ty=ClipRect.top;\n\t\t}\n\t\tif(yEnd>=ClipRect.bottom-1)\n\t\t\tyEnd=ClipRect.bottom-1;\n\t}\n\n\t/*Filling the triangle from Mid to Bot vertex*/\n\tfor(;y<=yEnd;y++)\n\t{\n\t\tx=Ceil(xL);if(x<ClipRect.left){x=ClipRect.left;};\n\t\txEnd=Ceil(xR)-1;if(xEnd>ClipRect.right-1){xEnd=ClipRect.right-1;};\n\t\tfor(;x<=xEnd;x++)\n\t\t\tGraphics_WritePixel(x,y,r,g,b);\n\n\t\txL+=InvSlope[leftEdge];\n\t\txR+=InvSlope[rightEdge];\n\t}\n}"<<endl;
	Graphics.close();
	/////////////////////////////////////////////////////////////////////////////////////////	
}

void GenerateCode_StandardTypes(void)
{
	/*This function will generate a header containing all the types needed in the game*/
	ofstream Header("Standard Header.h",ios_base::out);
	
	/*Make sure header will not cause user any complications*/
	Header<<"#ifndef _STANDARD_HEADER_\n#define _STANDARD_HEADER_\n"<<endl;
	Header<<"/*Including Windows Header*/\n#include<windows.h>"<<endl;
	/*Vertex Structure*/
	Header<<"\n#ifndef _VERTEX_STRUCT_\
			\n#define _VERTEX_STRUCT_\
			\n/*Vertex Structure*/\
			\nstruct Vertex\
			\n{\
			\n	float x,y;\
			\n};\
			\n#endif\n"<<endl;

	/*Vector Structure*/
	Header<<"#ifndef _VECTOR_STRUCT_\
			\n#define _VECTOR_STRUCT_\
			\n/*Vector Structure*/\
			\nstruct Vector\
			\n{\
			\n	float x,y;\
			\n};\
			\n#endif\n"<<endl;

	/*Color Structure*/
	Header<<"#ifndef _COLOR_STRUCT_\
			\n#define _COLOR_STRUCT_\
			\n/*Color Structure*/\
			\nstruct Color_RGB\
			\n{\
			\n	unsigned char r,g,b;\
			\n};\
			\n#endif\n"<<endl;

	/*Point Float Structure*/
	Header<<"#ifndef _POINT_FLOAT_\
			\n#define _POINT_FLOAT_\
			\n/*Point Float Structure*/\
			\ntypedef struct Point_Float\
			\n{\
			\n	float x,y;\
			\n}Point_Float;\
			\n#endif\n"<<endl;

	/*Primitive Type Enumeration*/
	Header<<"/*Primitive Type Enumeration*/\
			\nenum Primitive_Type\
			\n{\
			\n	Type_Line,\
			\n	Type_Circle,\
			\n	Type_Filled_Circle,\
			\n	Type_Triangle,\
			\n	Type_Filled_Triangle,\
			\n};\n"<<endl;

	/*Collision Type Enumeration*/
	Header<<"/*Collision Type Enumeration*/\
			\nenum Collision_Type\
			\n{\
			\n	Type_Collision_Line,\
			\n	Type_Collision_Circle,\
			\n};\n"<<endl;

	/*Primitive Structure*/
	Header<<"/*Primitive Structure*/\
			\nstruct Primitive\
			\n{\
			\n	enum Primitive_Type Type;	//Primitive Type\
			\n	Vertex V0,V1,V2;			//Primitive Vertices\
			\n	Color_RGB Color;			//Primitive Color\
			\n};\n"<<endl;

	/*Collision Structure*/
	Header<<"/*Collision Structure*/\
			\nstruct Collision_Data\
			\n{\
			\n	enum Collision_Type Type;	//Collision Data Type\
			\n	Vertex V0,V1;				//Two Vertices Represent both Collision Data Types\
			\n	Vector Normal;				//Normal in case of Line Collision Data\
			\n};\n"<<endl;
	
	/*Character Settings and Class*/
	Header<<"/*Character Settings*/\
			\nstruct Character_Settings\
			\n{\
			\n	Vertex Position;\
			\n	float Scale;\
			\n	float Angle;\
			\n	float Speed,Angular_Speed;\
			\n	Vector Vector_Direction;\
			\n};\n"<<endl;
	Header<<"/*Character class*/\
			\nclass Character\
			\n{\
			\n	private:\
			\n			char Name[50];\
			\n			Primitive *Primitives;\
			\n			Collision_Data *Collisions;\
			\n			int Primitives_Num,Collisions_Num;\
			\n			float BoundingCircleRadius;\
			\n			Character_Settings Settings;\
			\n	public:\
			\n			Character *next,*prev;\
			\n			/*Constructor*/\
			\n			Character():next(0),prev(0),Primitives(0),Collisions(0),Primitives_Num(0),Collisions_Num(0)	{memset(&Settings,0,sizeof(Settings));Settings.Scale=1;}\
			\n			Character(char *Name,Primitive *p,Collision_Data *c,int P_Num,int C_Num,float Pos_x,float Pos_y,float Scale,float Angle,float Speed,float Angular_Speed,float Direction_x,float Direction_y,float Bounding_Circle_Radius)\
			\n			{\
			\n				/*Set All Character Data*/\
			\n				this->SetName(Name);\
			\n				Primitives=p;Collisions=c;\
			\n				Primitives_Num=P_Num;Collisions_Num=C_Num;\
			\n				BoundingCircleRadius=Bounding_Circle_Radius;\
			\n				/*Set Character Settings*/\
			\n				Settings.Position.x=Pos_x;Settings.Position.y=Pos_y;\
			\n				Settings.Scale=Scale;\
			\n				Settings.Angle=Angle;\
			\n				Settings.Speed=Speed;\
			\n				Settings.Angular_Speed=Angular_Speed;\
			\n				Settings.Vector_Direction.x=Direction_x;Settings.Vector_Direction.y=Direction_y;\
			\n			}\
			\n			/*Destructor*/\
			\n			~Character(){}\
			\n			/*Get Primitives and Collisions*/\
			\n			Primitive *GetPrimitives(void){return Primitives;}\
			\n			Collision_Data *GetCollisions(void){return Collisions;}\
			\n			/*Get Primitives and Collisions Count*/\
			\n			int GetPrimitivesCount(void){return Primitives_Num;}\
			\n			int GetCollisionsCount(void){return Collisions_Num;}\
			\n			/*Get reference to character Settings*/\
			\n			Character_Settings& GetSettings(void){return Settings;}\
			\n";
			Header<<"			\n			/*Function that Sets Character Settings*/\
			\n			void Set(Vertex Position,float Scale,float Angle,float Speed,float Angular_Speed,Vector Vector_Direction)\
			\n			{\
			\n				/*Set Character Settings*/\
			\n				Settings.Position=Position;\
			\n				Settings.Scale=Scale;\
			\n				Settings.Angle=Angle;\
			\n				Settings.Speed=Speed;\
			\n				Settings.Angular_Speed=Angular_Speed;\
			\n				Settings.Vector_Direction=Vector_Direction;\
			\n			}\
			\n\
			\n			/*Function to Draw the Character*/\
			\n			void Draw(float Matrix[3][3]);\
			\n			/*Function to Check if two Characters collide*/\
			\n			bool CheckCollision(Character &CheckWith);\
			\n			/*Function to build Character Matrix*/\
			\n			void BuildCharacterMatrix(float Matrix[3][3]);\
			\n			/* Function to check whether the BoundingCircles of \
			\n			   two character collide or not (to know if we call \
			\n			   CheckCollision function */ \
			\n			void BoundingCircleCollision(Character &CheckWith);\
			\n\
			\n			/*Function to return the name*/\
			\n			char* GetName(){return Name;}\
			\n			void SetName (char *str){strcpy(Name, str);}\
			\n			/*Function to create a copy of character*/\
			\n			void Instance(const Character& src);\
			\n};\n"<<endl;

	Header<<"/*Function Prototypes*/"<<endl;
	Header<<"inline Point_Float operator*(float M[3][3], Point_Float V)\
			\n{\
			\n	Point_Float result;\
			\n	\
			\n	result.x=M[0][0]*V.x + M[0][1]*V.y + M[0][2]*1;\
			\n	result.y=M[1][0]*V.x + M[1][1]*V.y + M[1][2]*1;\
			\n	\
			\n	return result;\
			\n}"<<endl;

	/*Final Pre-Processor command*/
	Header<<"#endif"<<endl;

	/*Character Source File*/
	ofstream Character("Character.cpp",ios_base::out);

	Character<<"#include<math.h>\
				\n#include\"Standard Header.h\"\
				\n#include\"Clipping.h\"\
				\n#include\"Collision.h\"\
				\n\
				\nvoid Character::BuildCharacterMatrix(float Matrix[3][3])\
				\n{\
				\n	float Cosine=(float)cos(Settings.Angle*3.1415f/180),Sine=(float)sin(Settings.Angle*3.1415f/180);//To avoid Multiple function calls overhead\
				\n	/*A Matrix combination of rotation, scaling and translation (in order)*/\
				\n	Matrix[0][0]=Settings.Scale*Cosine;\
				\n	Matrix[0][1]=-Settings.Scale*Sine;\
				\n	Matrix[0][2]=Settings.Position.x;\
				\n	Matrix[1][0]=Settings.Scale*Sine;\
				\n	Matrix[1][1]=Settings.Scale*Cosine;\
				\n	Matrix[1][2]=Settings.Position.y;\
				\n	Matrix[2][0]=Matrix[2][1]=0;\
				\n	Matrix[2][2]=1;\n}\n\n\
				\n";
		Character<<"\nvoid Character::Draw(float Matrix[3][3])\
				\n{\
				\n	Point_Float V[3];\
				\n	for(int i=0;i<Primitives_Num;i++)\
				\n	{\
				\n		/*Filling Temporary Point Float array*/\
				\n		V[0].x=Primitives[i].V0.x;V[1].x=Primitives[i].V1.x;V[2].x=Primitives[i].V2.x;\
				\n		V[0].y=Primitives[i].V0.y;V[1].y=Primitives[i].V1.y;V[2].y=Primitives[i].V2.y;\
				\n\
				\n		/*Get Transformed Vertices*/\
				\n		V[0]=Matrix*V[0];\
				\n		V[1]=Matrix*V[1];\
				\n		V[2]=Matrix*V[2];\
				\n\
				\n		/*Draw Each Primitive Depending on its type*/\
				\n		switch(Primitives[i].Type)\
				\n		{\
				\n			case Type_Line:\
				\n							if(LB_Clipp(&V[0],&V[1]))\
				\n								LINE(V[0].x,V[0].y,V[1].x,V[1].y,Primitives[i].Color.r,Primitives[i].Color.g,Primitives[i].Color.b);\
				\n							break;\
				\n			case Type_Circle:\
				\n							CIRCLE(V[0].x,V[0].y,(int)V[1].x,Primitives[i].Color.r,Primitives[i].Color.g,Primitives[i].Color.b);\
				\n							break;\
				\n			case Type_Filled_Circle:\
				\n							FILL_CIRCLE(V[0].x,V[0].y,(int)V[1].x,Primitives[i].Color.r,Primitives[i].Color.g,Primitives[i].Color.b);\
				\n							break;\
				\n			case Type_Triangle:\
				\n							TRIANGLE(V,Primitives[i].Color.r,Primitives[i].Color.g,Primitives[i].Color.b);\
				\n							break;\
				\n			case Type_Filled_Triangle:\
				\n							FILL_TRIANGLE(V,Primitives[i].Color.r,Primitives[i].Color.g,Primitives[i].Color.b);\
				\n							break;\
				\n		}\
				\n	}\
				\n}\n"<<endl;
		/* Character collision function */
		Character<<"\nbool Character::CheckCollision(Character &CheckWith) \
			\n{ \
			\n	int CheckType=0; \
			\n	bool CheckCollision=0; \
			\n	/* Looping through the number of collision data in the calling class */ \
			\n	for(int i=0; i<this->GetCollisionsCount(); i++) \
			\n	{ \
			\n		/* Defining the type of the calling collision data */ \
			\n		switch(Collisions[i].Type) \
			\n		{ \
			\n			/* Line collision type */ \
			\n			case Type_Collision_Line: \
			\n				CheckType=0; \
			\n				break; \
			\n			/* Circle collision type */ \
			\n			case Type_Collision_Circle: \
			\n				CheckType=1; \
			\n				break; \
			\n		} \
			\n		/* Looping through the number of collision data in the with character */ \
			\n		for(int j=0; j<CheckWith.GetCollisionsCount(); j++) \
			\n		{ \
			\n			/* According to the type of collision data */ \
			\n			switch(CheckWith.Collisions[j].Type) \
			\n			{ \
			\n				/* Line collision */ \
			\n				case Type_Collision_Line: \
			\n					/* Line with Line collision */ \
			\n					if(!CheckType) \
			\n						CheckCollision=Line_Line_Collision(Collisions[i], CheckWith.Collisions[j]); \
			\n					/* Circle with Line collision */ \
			\n					else \
			\n						CheckCollision=Line_Circle_Collision(Collisions[i], CheckWith.Collisions[j]); \
			\n					break; \
			\n				/* Circle collision */ \
			\n				case Type_Collision_Circle: \
			\n					/* Circle with Line collision */ \
			\n					if(!CheckType) \
			\n						CheckCollision=Line_Circle_Collision(CheckWith.Collisions[j], Collisions[i]); \
			\n					/* Circle with Circle collision */ \
			\n					else \
			\n						CheckCollision=Circle_Circle_Collision(Collisions[i], CheckWith.Collisions[j]); \
			\n					break; \
			\n			} \
			\n		} \
			\n	} \
			\n	/* Returining the variable that will be 0 or 1 if there was a collision or not */ \
			\n	return CheckCollision; \
			\n}"<<endl;

		Character<<"\nvoid Character::BoundingCircleCollision (Character &CheckWith) \
					\n{ \
					\n	/* Checking if the distance between the two characters is less or equal than the BoundingCircleRadius_1 + BoundingCircleRadius_2 */ \
					\n	if(sqrt(((Settings.Position.x - CheckWith.Settings.Position.x)*(Settings.Position.x - CheckWith.Settings.Position.x)) + ((Settings.Position.y - CheckWith.Settings.Position.y)*(Settings.Position.y - CheckWith.Settings.Position.y)))<= (BoundingCircleRadius + CheckWith.BoundingCircleRadius)) \
					\n		CheckCollision(CheckWith); \
					\n}"<<endl;
}

void GenerateCode_Collision(void)
{
	ofstream Collision("Collision.cpp",ios_base::out);
	ofstream Collision_h("Collision.h",ios_base::out);
	
	/* Include the standard header */
	Collision_h<<"#include <math.h>\n#include \"Standard Header.h\""<<endl;
	/* Functions Prototypes */
	Collision_h<<"\n/* Functions Prototypes */"<<endl;
	Collision_h<<"bool Line_Circle_Collision (Collision_Data Circle, Collision_Data Line);"<<endl;
	Collision_h<<"bool Circle_Circle_Collision (Collision_Data Circle_1, Collision_Data Circle_2);"<<endl;
	Collision_h<<"bool Line_Line_Collision (Collision_Data Line_1, Collision_Data Line_2);"<<endl;
	
	/* Include the Collision.h header */
	Collision<<"#include \"Collision.h\""<<endl;
	/* Line_Circle_Collision Function */
	Collision<<"\nbool Line_Circle_Collision (Collision_Data Circle, Collision_Data Line) \
				\n{ \
				\n	Vector s_m, VD; \
				\n	float A, B, C, D; \
				\n	float t, t1, t2; \
				\n	/* Getting the vector s(Vector from a point on the line to O(0,0)) - m(Vector from the Center of the circle to O(0,0))*/ \
				\n	s_m.x=Line.V0.x-Circle.V0.x; \
				\n	s_m.y=Line.V0.y-Circle.V0.y; \
				\n	/* Defining the vector direction of the line */ \
				\n	VD.x=Line.V1.x-Line.V0.x; \
				\n	VD.y=Line.V1.y-Line.V0.y; \
				\n	/* The equation is of the form At^2 + Bt + C = 0 */ \
				\n	A = VD.x*VD.x + VD.y*VD.y; \
				\n	B = (2*s_m.x*VD.x + 2*s_m.y*VD.y); \
				\n	C = (s_m.x*s_m.x + s_m.y*s_m.y) - Circle.V1.x*Circle.V1.x; \
				\n	/* Descriminant */ \
				\n	D = B*B - 4*A*C; \
				\n	float sqrt_D=(float)sqrt(D); \
				\n	/* Checking if the descriminant is >=0 */ \
				\n	if(D>=0) \
				\n	{ \
				\n		/* One point of intersection */ \
				\n		if(D==0) \
				\n			t = (-B)/(2*A); \
				\n		/* Two points of intersection */ \
				\n		else \
				\n		{ \
				\n			/* Taking the parameter t of the 2 points of intersection */ \
				\n			t1 = (-B + sqrt_D)/(2*A); \
				\n			t2 = (-B - sqrt_D)/(2*A); \
				\n			/* Taking the smaller value of t */ \
				\n			t = (t1<t2)?t1:t2; \
				\n		} \
				\n		/* Taking the line as a segment */ \
				\n		if(t>=0 && t<=1) \
				\n			/* Collision */ \
				\n			return true; \
				\n	} \
				\n	/* No collision */ \
				\n	return false; \
				\n}"<<endl;
	/* Circle_Circle_Collision Function */
	Collision<<"\nbool Circle_Circle_Collision (Collision_Data Circle_1, Collision_Data Circle_2) \
				\n{ \
				\n	/* Calculate the distance from the center of circle_1 to the center of circle_2, if its \
				\n	less than the radius of circle_1 + the radius of circle_2 --> there is collision */ \
				\n	if(sqrt(((Circle_1.V0.x - Circle_2.V0.x)*(Circle_1.V0.x - Circle_2.V0.x)) + ((Circle_1.V0.y - Circle_2.V0.y)*(Circle_1.V0.y - Circle_2.V0.y)))<= (Circle_1.V1.x + Circle_2.V1.x)) \
				\n		return true; \
				\n	/* No collision */ \
				\n	return false; \
				\n}"<<endl;
	/* Line_Line Collision Function */
	Collision<<"\nbool Line_Line_Collision (Collision_Data Line_1, Collision_Data Line_2) \
				\n{ \
				\n	Vector VD_2, s_1, s_2; \
				\n	/* Defining the vector direction of the second line */ \
				\n	VD_2.x=Line_2.V1.x-Line_2.V0.x; \
				\n	VD_2.y=Line_2.V1.y-Line_2.V0.y; \
				\n	/* Defining the vector which is from any point on the line to the origin */ \
				\n	/* Line 1 */ \
				\n	s_1.x=Line_1.V0.x; \
				\n	s_1.y=Line_1.V0.y; \
				\n	/* Line 2 */ \
				\n	s_2.x=Line_2.V0.x; \
				\n	s_2.y=Line_2.V0.y; \
				\n	/* Calculating the dot product of the vector normal \
				\n	N and the vector director C */ \
				\n	float N_C = Line_1.Normal.x*VD_2.x + Line_1.Normal.y*VD_2.y; \
				\n	/* if the 2 lines are perpendi */ \
				\n	if(N_C==0) \
				\n		return false; \
				\n	/* If the dot product result is different from 0 \
				\n	--> calculate the time t */ \
				\n	float t = ((Line_1.Normal.x*s_1.x + Line_1.Normal.y*s_1.y) - (Line_1.Normal.x*s_2.x + Line_1.Normal.y*s_2.y)) / N_C; \
				\n	/* Take the segments */ \
				\n	if(t>=0 && t<=1) \
				\n		return true; \
				\n	return false; \
				\n}"<<endl;
}

char* Filter_Identifier(char *p)
{
	int len=(int)strlen(p),i=0;
	/*Valid Argument Check*/
	if(!len)
		return NULL;
	/*Allocate Memory for name, it'll need a max of len + 2 elements (one for NULL character and one for potential _ add*/
	char *Name=new char[len+2];

	/*If First Character is not a valid Identifier, add under score to it*/
	if(!(__iscsymf (*p)))
		Name[i++]='_';	
	/*Copy Name while filtering out none valid identifier characters*/
	for(;*p;p++,i++)
	{
		if(__iscsym(*p))
			Name[i]=*p;
		else
			Name[i]='_';
	}
	Name[i]='\0';
	/*Return Built Identifier*/
	return Name;
}

void ScreenToWorld(Character &c)
{
	int Num;
	float Sx=(float)(destRect.right-destRect.left)/(srcRect.right-srcRect.left);
	float Sy=(float)(destRect.bottom-destRect.top)/(srcRect.bottom-srcRect.top);
	/*Transform Primitives to world Coordinates*/
	for(Primitive *Primitives=c.GetPrimitives();Primitives;Primitives=Primitives->next)
	{
		Num=2;
		if(Primitives->Type==Type_Triangle || Primitives->Type==Type_Filled_Triangle)
			Num=3;
		/*Transform each vertex*/
		for(int i=0;i<Num;i++)
		{
			if(i==1)
			{
				if(Primitives->Type!=Type_Circle && Primitives->Type!=Type_Filled_Circle)
				{
					/*Translate to World Center (Dont translate circle radius)*/
					Primitives->V[i].x-=320;
					Primitives->V[i].y-=240;
				}
			}
			else
			{
				/*Translate to World Center*/
				Primitives->V[i].x-=320;
				Primitives->V[i].y-=240;
			}
			/*Scale to World Coordinates*/
			Primitives->V[i].x*=Sx;
			Primitives->V[i].y*=-Sy;
		}
	}
	for(Collision_Data *Collisions=c.GetCollisions();Collisions;Collisions=Collisions->next)
	{
		/*Transform the first vertex*/
		/*Translate to World Center*/
		Collisions->V0.x-=320;
		Collisions->V0.y-=240;
		/*Scale to World Coordinates*/
		Collisions->V0.x*=Sx;
		Collisions->V0.y*=-Sy;
		if(Collisions->Type==Type_Collision_Circle)
		{
			/*Radius cannot be translated only scaled*/
			Collisions->V1.x*=Sx;
			Collisions->V1.y*=-Sy;
		}
		else
		{
			/*Translate to World Center*/
			Collisions->V1.x-=320;
			Collisions->V1.y-=240;
			/*Scale to World Coordinates*/
			Collisions->V1.x*=Sx;
			Collisions->V1.y*=-Sy;
		}
	}
}

void GenerateCode_CharacterDesign(void)
{
	/*This function will generate a source file and a header for character design*/
	ofstream Design("Design.cpp",ios_base::out);
	ofstream Design_Header("Design.h",ios_base::out);
	ofstream Data("Character Initialization.cpp",ios_base::out);
	int Count=(int)SendDlgItemMessage(hwndDlg,IDC_LIST_CHAR,LB_GETCOUNT,0,0);

	/*Include Standard Header to Design file*/
	Design<<"#include \"Standard Header.h\"\n"<<endl;
	Design<<"/*Defining All Characters Primitives and Collisions*/\n"<<endl;

	Data<<"#include \"Standard Header.h\""<<endl;
	Data<<"#include \"Design.h\"\n"<<endl;
	/*Function to Add Characters to list*/
	Data<<"void Add_Character_ToList(Character **head,Character **tail,Character *New)\
			\n{\
			\n	if(!*head)\
			\n	{\
			\n		*head=*tail=New;\
			\n		return;\
			\n	}\
			\n	(*tail)->next=New;\
			\n	New->prev=*tail;\
			\n	*tail=New;\
			\n}\n"<<endl;

	/*Function to Initialize all characters in linked list*/
	Data<<"Character *Init_Characters(void)\n{\n";
	Data<<"\t/*Character List is a Two-Way Linked list*/\
			\n\tCharacter *head=NULL,*tail=NULL;\
			\n\tCharacter *New;\n"<<endl;

	/*Go through all Saved Characters*/
	for(int i=0;i<Count;i++)
	{
		Character *p=(Character*)SendDlgItemMessage(hwndDlg,IDC_LIST_CHAR,LB_GETITEMDATA,i,0);
		Character Tmp_Char;
		/*Get Instance of Current Character*/
		Tmp_Char.Instance(*p);

		/* Radius of the bounding circle */
		float BoundingCircleRadius=0;
		/*Filer Character Name and make it a valid identifier*/
		char tmp[50];strcpy(tmp,p->GetName());
		char *Name=Filter_Identifier(tmp);

		/*Add Externals Relating to Current Character to header file*/
		Design_Header<<"/*\""<<p->GetName()<<"\" Primitives and Collision Data externs*/"<<endl;
		Design_Header<<"extern "<<"Primitive "<<Name<<"_Primitives["<<p->GetPrimitivesCount()+1<<"];"<<endl;
		Design_Header<<"extern "<<"Collision_Data "<<Name<<"_Collisions["<<p->GetCollisionsCount()+1<<"];\n"<<endl;

		/*Character Primitives*/
		Design<<"/*\""<<p->GetName()<<"\" Primitives and Collision Data*/"<<endl;
		if(p->GetPrimitivesCount())
			Design<<"Primitive "<<Name<<"_Primitives["<<p->GetPrimitivesCount()+1<<"]=\n{"<<endl;
		else
			Design<<"Primitive "<<Name<<"_Primitives["<<p->GetPrimitivesCount()+1<<"];"<<endl;
		/*Get Source and Destination Rectangles*/
		srcRect=Tmp_Char.GetSourceRect();
		destRect=Tmp_Char.GetDestinationRect();
		/*Transform Character Vertices to World*/
		ScreenToWorld(Tmp_Char);
		for(Primitive *Primitives=Tmp_Char.GetPrimitives();Primitives;Primitives=Primitives->next)
		{
			char Type[50];
			/*Build Primitive Type String*/
			switch(Primitives->Type)
			{
				case Type_Line:strcpy(Type,"Type_Line");break;
				case Type_Circle:strcpy(Type,"Type_Circle");break;
				case Type_Filled_Circle:strcpy(Type,"Type_Filled_Circle");break;
				case Type_Triangle:	strcpy(Type,"Type_Triangle");break;
				case Type_Filled_Triangle:strcpy(Type,"Type_Filled_Triangle");break;
			}
			Design<<"\t{"<<Type<<" , ";

			/*If Primitive is a triangle ===> 3 vertices are needed for generation*/
			if(Primitives->Type==Type_Triangle || Primitives->Type==Type_Filled_Triangle)
				Design<<"{"<<"(float)"<<Primitives->V[0].x<<","<<"(float)"<<Primitives->V[0].y<<"} , "<<"{"<<"(float)"<<Primitives->V[1].x<<","<<"(float)"<<Primitives->V[1].y<<"} , "<<"{"<<"(float)"<<Primitives->V[2].x<<","<<"(float)"<<Primitives->V[2].y<<"} , ";
			else
				Design<<"{"<<"(float)"<<Primitives->V[0].x<<","<<"(float)"<<Primitives->V[0].y<<"} , "<<"{"<<"(float)"<<Primitives->V[1].x<<","<<"(float)"<<Primitives->V[1].y<<"} , "<<"{0,0} , ";
			
			/*Finally Set Primitive Color*/
			Design<<"{"<<(int)Primitives->Color.r<<","<<(int)Primitives->Color.g<<","<<(int)Primitives->Color.b<<"}},\n";
		}
		if(p->GetPrimitivesCount())
			Design<<"};\n"<<endl;
		/*Character Collisions*/
		if(p->GetCollisionsCount())
			Design<<"Collision_Data "<<Name<<"_Collisions["<<p->GetCollisionsCount()+1<<"]=\n{"<<endl;
		else
			Design<<"Collision_Data "<<Name<<"_Collisions["<<p->GetCollisionsCount()+1<<"];"<<endl;
		for(Collision_Data *Collisions=Tmp_Char.GetCollisions();Collisions;Collisions=Collisions->next)
		{
			/* Looping through all the vertices of the primitive */
			/* Updating the radius of the bounding circle */
			if(Collisions->Type==Type_Collision_Line)
			{
				BoundingCircleRadius=(float)((sqrt(Collisions->V0.x*Collisions->V0.x+Collisions->V0.y*Collisions->V0.y))>BoundingCircleRadius?(sqrt(Collisions->V0.x*Collisions->V0.x+Collisions->V0.y*Collisions->V0.y)):BoundingCircleRadius);
				BoundingCircleRadius=(float)((sqrt(Collisions->V1.x*Collisions->V1.x+Collisions->V1.y*Collisions->V1.y))>BoundingCircleRadius?(sqrt(Collisions->V1.x*Collisions->V1.x+Collisions->V1.y*Collisions->V1.y)):BoundingCircleRadius);
			}
			else
			{
				/* Creating the 4 vertices around the circle collision data */
				Vertex V[4];
				V[0].x=Collisions->V0.x-Collisions->V1.x;	V[0].y=Collisions->V0.y;
				V[1].x=Collisions->V0.x+Collisions->V1.x;	V[1].y=V[0].y;
				V[2].x=Collisions->V0.x;					V[2].y=Collisions->V0.y+Collisions->V1.x;
				V[3].x=Collisions->V0.x;					V[3].y=Collisions->V0.y-Collisions->V1.x;
				for(int k=0; k<4; k++)
					BoundingCircleRadius=(float)((sqrt(V[k].x*V[k].x+V[k].y*V[k].y)>BoundingCircleRadius)?(sqrt(V[k].x*V[k].x+V[k].y*V[k].y)):BoundingCircleRadius);
			}
			char Type[50];
			/*Build Collision Type String*/
			switch(Collisions->Type)
			{
				case Type_Collision_Line:strcpy(Type,"Type_Collision_Line");break;
				case Type_Collision_Circle:strcpy(Type,"Type_Collision_Circle");break;
			}
			Design<<"\t{"<<Type<<" , ";
			/*Collision Vertices*/
			Design<<"{"<<"(float)"<<Collisions->V0.x<<","<<"(float)"<<Collisions->V0.y<<"} , "<<"{"<<"(float)"<<Collisions->V1.x<<","<<"(float)"<<Collisions->V1.y<<"} , ";
			/*Collision Normal*/
			Design<<"{"<<"(float)"<<Collisions->Normal.x<<","<<"(float)"<<Collisions->Normal.y<<"}},\n";
		}
		if(p->GetCollisionsCount())
			Design<<"};\n"<<endl;
		/*Initializing Character and Adding it to the linked list*/
		Data<<"\t/*Initializing \""<<p->GetName()<<"\" and Adding it to the linked list*/"<<endl;
		Data<<"\tNew=new Character";
		/*Character Data*/
		Data<<"(\""<<Name<<"\","<<Name<<"_Primitives,"<<Name<<"_Collisions,"<<p->GetPrimitivesCount()<<","<<p->GetCollisionsCount()<<","<<p->GetSettings().Position.x<<","<<p->GetSettings().Position.y<<","<<p->GetSettings().Scale<<","<<p->GetSettings().Angle<<","<<p->GetSettings().Speed<<","<<p->GetSettings().Angular_Speed<<","<<p->GetSettings().Vector_Direction.x<<","<<p->GetSettings().Vector_Direction.y<<","<<BoundingCircleRadius<<");"<<endl;
		Data<<"\tAdd_Character_ToList(&head,&tail,New);\n"<<endl;
		/*Free Name*/if(Name)delete Name;
	}
	/*Close Character Initialization function bracket*/
	Data<<"\treturn head;\n}"<<endl;
}

void GenerateCode_FrameRateController(void)
{	
	ofstream file("FrameRateController.cpp", ios_base::out);

	file << "#include <windows.h>\n#include \"FrameRateController.h\"\n#include \"Settings.h\"\n";
	
	file << "\nint m_FrameRate; \
		        \nfloat m_FrameTime; \
	            \nfloat m_CurrentFrameTime; \
				\nunsigned int m_OldTime;\n";
	
	file << "\nvoid FRC_Init() \
				\n{ \
				\n	m_OldTime = 0; \
				\n	FRC_SetFrameRate(FRAMERATE); \
				\n	m_CurrentFrameTime = m_FrameTime; \
				\n}\n";
	
	file << "\nvoid FRC_ShutDown() \
				\n{ \
				\n	timeEndPeriod(1); \
				\n}\n";

	file << "\nvoid FRC_SetFrameRate(int frameRate) \
				\n{ \
				\n	m_FrameRate = frameRate; \
				\n	m_FrameTime = 1000.0f/m_FrameRate; \
				\n}\n";

	file << "\nfloat FRC_GetCurrentFrameRate() \
				\n{ \
				\n	return 1000.0f/m_CurrentFrameTime; \
				\n}\n";

	file << "\nvoid FRC_Start() \
				\n{ \
				\n	m_OldTime = timeGetTime(); \
				\n}\n";

	file << "\nvoid FRC_End() \
				\n{ \
				\n	float delta; \
				\n	m_CurrentFrameTime = (float)(timeGetTime() - m_OldTime); \
				\n	delta = m_FrameTime - m_CurrentFrameTime; \
				\n \
				\n	if(delta>0) \
				\n	{ \
				\n		m_OldTime = timeGetTime(); \
				\n \
				\n		while(timeGetTime()-m_OldTime<delta); \
				\n		m_CurrentFrameTime = m_FrameTime; \
				\n	} \
				\n}\n";

	file.close();

	file.open("FrameRateController.h", ios_base::out);

	file << "void FRC_End(); \
				\nvoid FRC_Init(); \
				\nvoid FRC_Start(); \
				\nvoid FRC_ShutDown(); \
				\nvoid FRC_SetFrameRate(int); \
				\nfloat FRC_GetCurrentFrameRate();";

	file.close();
}

void GenerateCode_Keyboard(void)
{
	ofstream file("Keyboard.cpp", ios_base::out);

	file << "#include<windows.h> \
				\n#include \"Keyboard.h\" \
				\n#include \"Settings.h\"";

	file << "\n\nstatic KeyState m_Keys[256]={0};";

	file << "\nint Keyboard_IsPressed(int vKey) \
				\n{ \
				\n	return m_Keys[vKey].m_IsPressed; \
				\n}\n";

	file << "\nint Keyboard_IsTriggered(int vKey) \
				\n{ \
				\n	return m_Keys[vKey].m_IsTriggered; \
				\n}\n";

	file << "\nvoid Keyboard_Update() \
				\n{ \
				\n	int i=0; \
				\n	BYTE KeyStatus[256]={0}; \
				\n	GetKeyboardState(KeyStatus); \
				\n \
				\n	for(i=0;i<256;i++) \
				\n	{ \
				\n		m_Keys[i].m_IsPressed = (KeyStatus[i] & (1 << 7))?1:0;\
				\n \
				\n		if(m_Keys[i].m_IsPressed) \
				\n		{ \
				\n			if(m_Keys[i].m_WasPressed) \
				\n				m_Keys[i].m_IsTriggered = 0; \
				\n			else{ \
				\n				m_Keys[i].m_WasPressed  = 1; \
				\n				m_Keys[i].m_IsTriggered = 1; \
				\n			} \
				\n		} \
				\n		else{ \
				\n			m_Keys[i].m_IsTriggered = 0; \
				\n			m_Keys[i].m_WasPressed  = 0; \
				\n		} \
				\n	} \
				\n}\n";

	file.close();

	file.open("Keyboard.h", ios_base::out);

	file << "void Keyboard_Update(); \
				\nint Keyboard_IsPressed(int vKey); \
				\nint Keyboard_IsTriggered(int vKey);";

	file.close();
}

void GenerateCode_Mouse(void)
{
	ofstream file("Mouse.cpp", ios_base::out);

	file << "#include <windows.h> \
				\n#include \"Mouse.h\" \
				\n#include \"Settings.h\"";

	file << "\n\nextern HWND GlobalHwnd; \
				\nstatic KeyState m_Keys[3] = {0};";

	file << "\n\nint Mouse_IsPressedLButton() \
				\n{ \
				\n	return m_Keys[0].m_IsPressed; \
				\n}\n";

	file << "\nint Mouse_IsTriggeredLButton() \
				\n{ \
				\n	return m_Keys[0].m_IsTriggered; \
				\n}\n";

	file << "\nint Mouse_IsPressedMButton() \
				\n{ \
				\n	return m_Keys[1].m_IsPressed; \
				\n}\n";

	file << "\nint Mouse_IsTriggeredMButton() \
				\n{ \
				\n	return m_Keys[1].m_IsTriggered; \
				\n}\n";

	file << "\nint Mouse_IsPressedRButton() \
				\n{ \
				\n	return m_Keys[2].m_IsPressed; \
				\n}\n";

	file << "\nint Mouse_IsTriggeredRButton() \
				\n{ \
				\n	return m_Keys[2].m_IsTriggered; \
				\n}\n";

	file << "\nvoid Mouse_GetCursorPosition(int *x, int *y) \
				\n{ \
				\n	POINT pt = {0}; \
				\n \
				\n	GetCursorPos(&pt); \
				\n	ScreenToClient(GlobalHwnd, &pt); \
				\n	*x = pt.x; \
				\n	*y = pt.y; \
				\n}\n";

	file << "\nvoid Mouse_ShowCursor(int b) \
				\n{ \
				\n	if(b) \
				\n		while(ShowCursor(TRUE)<0); \
				\n	else \
				\n		while(ShowCursor(FALSE)>=0); \
				\n}\n";

	file << "\nvoid Mouse_Update() \
				\n{ \
				\n	int i=0; \
				\n \
				\n	SHORT ks[3] = {0}; \
				\n	ks[0] = GetAsyncKeyState(VK_LBUTTON); \
				\n	ks[1] = GetAsyncKeyState(VK_MBUTTON); \
				\n	ks[2] = GetAsyncKeyState(VK_RBUTTON); \
				\n \
				\n	for(i=0; i<3; i++) \
				\n	{ \
				\n		m_Keys[i].m_IsPressed = (ks[i] & (1 << 15)) ? 1:0; \
				\n		if(m_Keys[i].m_IsPressed) \
				\n		{ \
				\n			if(m_Keys[i].m_WasPressed) \
				\n				m_Keys[i].m_IsTriggered = 0; \
				\n			else \
				\n			{ \
				\n				m_Keys[i].m_WasPressed = 1; \
				\n				m_Keys[i].m_IsTriggered = 1; \
				\n			} \
				\n		} \
				\n		else \
				\n		{ \
				\n			m_Keys[i].m_WasPressed = 0; \
				\n			m_Keys[i].m_IsTriggered = 0; \
				\n		} \
				\n	} \
				\n}\n";

	file.close();

	file.open("Mouse.h", ios_base::out);

	file << "void Mouse_Update(); \
				\nvoid Mouse_ShowCursor(int b); \
				\nvoid Mouse_GetCursorPosition(int *x, int *y); \
				\n \
				\nint Mouse_IsPressedLButton(); \
				\nint Mouse_IsTriggeredLButton(); \
				\n \
				\nint Mouse_IsPressedMButton(); \
				\nint Mouse_IsTriggeredMButton(); \
				\n \
				\nint Mouse_IsPressedRButton(); \
				\nint Mouse_IsTriggeredRButton();";

	file.close();
}

void GenerateCode_Clipping(void)
{
	ofstream file("Clipping.cpp", ios_base::out);
	
	file << "#include \"Clipping.h\"";

	file <<	"\n\nBOOL LB_Clipp(Point_Float *p1,Point_Float *p2) \
			\n{ \
			\n	int i; \
			\n	float ta=0,tb=1,pk[4],qk[4]; \
			\n \
			\n	/*Getting pk and qk for all the edges*/ \
			\n	pk[0]=p1->x-p2->x; \
			\n	pk[1]=-pk[0]; \
			\n	pk[2]=p1->y-p2->y; \
			\n	pk[3]=-pk[2]; \
			\n \
			\n	qk[0]=p1->x-ClipRect.left; \
			\n	qk[1]=ClipRect.right-1-p1->x; \
			\n	qk[2]=p1->y-ClipRect.top; \
			\n	qk[3]=ClipRect.bottom-1-p1->y; \
			\n \
			\n	/*Checking for trivial rejection of line*/ \
			\n	for(i=0;i<4;i++) \
			\n	{ \
			\n		if(pk[i]==0&&qk[i]<0) \
			\n		return FALSE; \
			\n	} \
			\n \
			\n	/*Getting new ta and tb values to clip line boundaries*/ \
			\n	for(i=0;i<4;i++) \
			\n	{ \
			\n		if(pk[i]<0 && ta<qk[i]/pk[i]) \
			\n			ta=qk[i]/pk[i]; \
			\n		else if(pk[i]>0 && tb>qk[i]/pk[i]) \
			\n			tb=qk[i]/pk[i]; \
			\n \
			\n	/*Reject line if necessary*/ \
			\n	if(ta>tb) \
			\n		return FALSE; \
			\n	} \
			\n \
			\n	/*Getting new starting and ending point values*/ \
			\n	p2->x=p1->x+pk[1]*tb; \
			\n	p2->y=p1->y+pk[3]*tb; \
			\n	\
			\n	p1->x+=pk[1]*ta; \
			\n	p1->y+=pk[3]*ta; \
			\n \
			\n	return TRUE; \
			\n}";
	
	file.close();

	file.open("Clipping.h", ios_base::out);

	file << "#include \"Graphics.h\" \
			\n \
			\n/*External Clipping Rectangle*/ \
			\nextern RECT ClipRect; \
			\n \
			\n/*Function Prototypes*/ \
			\nBOOL LB_Clipp(Point_Float *p1,Point_Float *p2); \
			\n\n/*Inline Pixel Clipping Function*/ \
			\n__inline BOOL Pixel_Clip(int x,int y) \
			\n{ \
			\n	if(x>ClipRect.left&&x<ClipRect.right&&y>ClipRect.top&&y<ClipRect.bottom) \
			\n		return TRUE; \
			\n	return FALSE; \
			\n}";

	file.close();
}