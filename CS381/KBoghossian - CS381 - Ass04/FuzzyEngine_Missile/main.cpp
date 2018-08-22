/* Start Header -------------------------------------------
- End Header ---------------------------------------------- */

#include<windows.h>
#include<stdio.h>
#include"Graphics.h"
#include"FrameRateController.h"
#include"Settings.h"
#include"Keyboard.h"
#include"Standard Header.h"
#include"Mouse.h"
#include"ClientHandle.h"
#include"Viewing.h"
#include"Character Initialization.h"
#include"Objects.h"
#include"Support_Functions.h"
#include <time.h>

#include "Missile.h"
#include "Target.h"


HWND GlobalHwnd;
static char FrameRateStr[32]={0};
extern Camera *Camera_List;
extern ViewPort *ViewPort_List;

int Source=0, Destination=0, Source_TrapIdx=-1, Destination_TrapIdx=-1;
int clicks=0;
Vertex destVtx;

bool StartFollow=false;
bool ReadyToFollow=false;

Missile *missile;
Target *target;
Character *Map;

Missile_Fuzzy missile_fuzzy(4);

VERTEX Get_WorldPosition (POINT Point)
{
	VERTEX Pt;
	Pt.x=(float)Point.x-(float)ViewPort_List->Center.x; Pt.y=(float)Point.y-(float)ViewPort_List->Center.y;
	Pt.y=-Pt.y;
	return Pt;
}


LRESULT CALLBACK WndProc(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam)
{
	switch(message)
	{
		case WM_CREATE:
						/*Create Windowed Display at program start-up*/
						Graphics_CreateWindowedDisplay(hwnd,WINDOW_WIDTH,WINDOW_HEIGHT);
						return 0;
		case WM_KEYDOWN:
						switch(wParam)
						{
							case VK_ESCAPE:
								PostQuitMessage(0);
								break;
	
							case VK_RETURN:
								if(ReadyToFollow)
									if(!StartFollow)
										StartFollow=true;
								break;

							case VK_LEFT:
								if(!StartFollow)
									missile->InitMissile(1);
								break;
							case VK_RIGHT:
								if(!StartFollow)
									missile->InitMissile(2);
								break;

						}
						return 0;

		case WM_LBUTTONDOWN:
			{
				POINT Point;
				GetCursorPos(&Point);	//Get Cursor Position and save it in Point structure
				ScreenToClient(hwnd,&Point);//Update Point coordinates to client area of window
				Point.x=(Point.x>0)?Point.x:0;
				Point.y=(Point.y>0)?Point.y:0;

				ReadyToFollow=true;
				if(!StartFollow)
					missile->Shape->GetSettings().Position=Get_WorldPosition(Point);
			}
			return 0;
		case WM_DESTROY:
						/*Free DDRAW object*/
						Graphics_FreeDirectDraw();
						/* Delete the memory allocated for the viewports */
						DeleteViewPortList(ViewPort_List);
						/* Delete the memory allocated for the cameras */
						DeleteCameraList(Camera_List);
						PostQuitMessage(0);
						return 0;
	}
	return DefWindowProc(hwnd,message,wParam,lParam);
}

int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nCmdShow)//WinMain() function declaration
{
	ATOM cID;				//Declare an ATOM variable to store the registered class ID
	WNDCLASSEXA wndclass;	//Declare a window class variable
	HWND hwnd;				//Declare a handle window variable
	MSG msg;				//Declare a MSG variable for messages

	/*Initialize the wndClass structure*/
	wndclass.cbSize=sizeof(WNDCLASSEXA);				//set size of structure
	wndclass.style=CS_HREDRAW|CS_VREDRAW;				//set class style
	wndclass.cbClsExtra=0;
	wndclass.cbWndExtra=0;
	wndclass.hInstance=hInstance;						//set handle Instance of window
	wndclass.lpfnWndProc=WndProc;						//set Window Procedure function
	wndclass.hIcon=LoadIcon(NULL,IDI_APPLICATION);		//set default Icon
	wndclass.hIconSm=LoadIcon(NULL,IDI_APPLICATION);	//set default Small Icon
	wndclass.hCursor=LoadCursor(NULL,IDC_ARROW);		//set default Cursor
	wndclass.hbrBackground=(HBRUSH)GetStockObject(WHITE_BRUSH);	//set background color
	wndclass.lpszMenuName=NULL;							//No menu
	wndclass.lpszClassName="Team Name";					//Set Class name

	cID=RegisterClassEx(&wndclass);						//Register the class and set the return value to cID ATOM

	/*Call create window functionto create the program's window*/
	GlobalHwnd=hwnd=CreateWindow(wndclass.lpszClassName,"",WS_BORDER|WS_CAPTION|WS_POPUP,(GetSystemMetrics(SM_CXSCREEN)-WINDOW_WIDTH)/2,(GetSystemMetrics(SM_CYSCREEN)-WINDOW_HEIGHT)/2,WINDOW_WIDTH,WINDOW_HEIGHT,NULL,NULL,hInstance,NULL);

	if(!hwnd)
		return 0;

	ShowWindow(hwnd,nCmdShow);							//Show the window after it has been created
	UpdateWindow(hwnd);									//Update the window

	srand((unsigned int)time(NULL));

	FRC_Init();											//Initialize frame rate
	/* Adding objects to the world */
	Character *Object_List = NULL;
	/* Creating the Cameras to the world */
	InitCameras(&Camera_List);
	/* Creating the viewports to the world */
	InitViewPorts(&ViewPort_List);

	missile = new Missile(Init_Missile());
	target = new Target(Init_Target());
	Map = Init_Map();


	/*Game Loop*/
	while(1)
	{
		FRC_Start();

		/*Get Messages in queue*/
		while(PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{
			if(msg.message==WM_QUIT)
				goto End;
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}

		Keyboard_Update();
		Mouse_Update();
	
		/************** UPDATE *************/
		if(StartFollow)
		{
			missile->Update(missile_fuzzy.GetAdjustmentAngle(missile->Shape->GetSettings().Position,missile->Shape->GetSettings().Vector_Direction,target->Shape->GetSettings().Position,target->Damage,missile->Fuel));
			target->Update();
		}

		float dist=0;
		Vector_Distance(dist,missile->Shape->GetSettings().Position,target->Shape->GetSettings().Position);
		if(dist<=12 && dist>=-12)
		{
			ReadyToFollow=false;
			StartFollow=false;
			//missile->Shape->GetSettings().Position.x=-50;
			//missile->Shape->GetSettings().Position.y=0;
			missile->Reset();
			target->Reset();
			missile_fuzzy.AttackPercentage=0;
			MessageBox(GlobalHwnd, "Plane Hit", "", (MB_OK));
		}
	
		//Client_Handle(Object_List);
		Init_Buffer(100);
		/* Drawing the viewports */
		Init_View(0);

		/************** DRAW ***************/
		Client_Draw(ViewPort_List,Map);
		if(ReadyToFollow)
			missile->Draw();
		target->Draw();
		
		/*Copy buffer to Video RAM*/
		Graphics_Draw(NULL);

	

		/*Display FrameRate*/
		//Graphics_TextOut(0,0,FrameRateStr, 0, 255, 0);
		char arr[100];
		/*sprintf(arr,"x=%f - y=%f",missile->Shape->GetSettings().Position.x,missile->Shape->GetSettings().Position.y);
		Graphics_TextOut(0,30,arr, 255, 255, 255);
		sprintf(arr,"x=%f - y=%f",target->Shape->GetSettings().Position.x,target->Shape->GetSettings().Position.y);
		Graphics_TextOut(0,50,arr, 255, 0, 0);*/

		sprintf(arr,"Damage = %.0f",target->Damage);
		Graphics_TextOut(30,0,arr, 0, 255, 0);
		sprintf(arr,"Fuel = %d",missile->Fuel);
		Graphics_TextOut(280,0,arr, 0, 255, 0);
		sprintf(arr,"Attack = %.0f",missile_fuzzy.AttackPercentage*100);
		Graphics_TextOut(540,0,arr, 0, 255, 0);

		//POINT Point;
		//GetCursorPos(&Point);	//Get Cursor Position and save it in Point structure
		//ScreenToClient(hwnd,&Point);//Update Point coordinates to client area of window
		//Point.x=(Point.x>0)?Point.x:0;
		//Point.y=(Point.y>0)?Point.y:0;
		//sprintf(arr,"x=%d - y=%d", Point.x,Point.y);
		//Graphics_TextOut(0,400,arr, 0, 220, 0);

		sprintf(arr,"%d",45);
		Graphics_TextOut(114,110,arr, 40, 0, 0);
		sprintf(arr,"%d",90);
		Graphics_TextOut(162,333,arr, 40, 0, 0);
		sprintf(arr,"%d",75);
		Graphics_TextOut(497,316,arr, 40, 0, 0);
		sprintf(arr,"%d",60);
		Graphics_TextOut(417,107,arr, 40, 0, 0);

		Graphics_Flip();

		FRC_End();

		/*Get Frame Rate String*/
		FrameRateStr[0]='\0';
		//sprintf(FrameRateStr,"FPS = %.2f",FRC_GetCurrentFrameRate());
	}

End:
	FRC_ShutDown();
	DestroyWindow(hwnd);
	return (int)msg.wParam;
}
