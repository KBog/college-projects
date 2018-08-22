#include<windows.h>
#include<stdio.h>
#include"Graphics.h"
#include"FrameRateController.h"
#include"Settings.h"
#include"Keyboard.h"
#include"Mouse.h"

HWND GlobalHwnd;
static char FrameRateStr[32]={0};

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
						}
						return 0;
		case WM_DESTROY:
						/*Free DDRAW object*/
						Graphics_FreeDirectDraw();
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
	GlobalHwnd=hwnd=CreateWindow(wndclass.lpszClassName,"",WS_POPUP|WS_BORDER,(GetSystemMetrics(SM_CXSCREEN)-WINDOW_WIDTH)/2,(GetSystemMetrics(SM_CYSCREEN)-WINDOW_HEIGHT)/2,WINDOW_WIDTH,WINDOW_HEIGHT,NULL,NULL,hInstance,NULL);

	if(!hwnd)
		return 0;

	ShowWindow(hwnd,nCmdShow);							//Show the window after it has been created
	UpdateWindow(hwnd);									//Update the window

	FRC_Init();											//Initialize frame rate

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

		//Client_Handle(Object_List);
		Init_Buffer();
		//Client_Draw(Object_List);
		Init_MainView();
		Init_RadarView();

		/*Copy buffer to Video RAM*/
		Graphics_Draw(NULL);

		/*Display FrameRate*/
		Graphics_TextOut(720,0,FrameRateStr);
		Graphics_Flip();

		FRC_End();

		/*Get Frame Rate String*/
		FrameRateStr[0]='\0';
		sprintf(FrameRateStr,"FPS = %.2f",FRC_GetCurrentFrameRate());
	}

End:
	FRC_ShutDown();
	DestroyWindow(hwnd);
	return (int)msg.wParam;
}
