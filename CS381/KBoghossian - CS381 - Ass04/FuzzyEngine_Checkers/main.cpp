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

#include "MiniMax.h"
#include "FuzzyMiniMax.h"


HWND GlobalHwnd;
static char FrameRateStr[32]={0};
extern Camera *Camera_List;
extern ViewPort *ViewPort_List;

Character *MainCharacter;
Character *NodeShape;
Character *Map;

Character *WhiteS;
Character *BlackS;
Character *Menu;
Character *Credits;

MiniMax minimax;

int Source=0, Destination=0, Source_TrapIdx=-1, Destination_TrapIdx=-1;
int clicks=0;
Vertex destVtx;

bool GameStarted=false;
bool MainMenu=true;
bool TwoPlayers=false;
bool CreditsMenu=false;
bool SelectDifficulty=false;
bool FuzzyDifficulty=false;

extern int DEPTH;
MiniMax_Fuzzy fuzzy_minimax(3);

VERTEX Get_WorldPosition (POINT Point)
{
	VERTEX Pt;
	Pt.x=(float)Point.x-(float)ViewPort_List->Center.x; Pt.y=(float)Point.y-(float)ViewPort_List->Center.y;
	Pt.y=-Pt.y;
	return Pt;
}

void Get_ColRow (POINT Pos, int &r, int &c)
{
	c = CELL_WIDTH;
	r = CELL_HEIGHT;
	float cf = (float)(Pos.x-80)/(float)c;
	float rf = (float)Pos.y/(float)r;
	if(cf<0)
		c=-1;
	else
		c = (int)cf;
	r = (int)rf;
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
								GameStarted=false;
								MainMenu=true;
								CreditsMenu=false;
								SelectDifficulty=false;
								break;
							case 'S':
								if(!GameStarted && !CreditsMenu && !SelectDifficulty)
								{
									TwoPlayers=false;
									//minimax.ResetGame();
									//GameStarted=true;
									MainMenu=false;
									SelectDifficulty=true;
									FuzzyDifficulty=false;
								}
								break;
							case 'M':
								if(!GameStarted && !CreditsMenu && !SelectDifficulty)
								{
									TwoPlayers=true;
									minimax.ResetGame();
									GameStarted=true;
									MainMenu=false;
								}
								break;

							case 'C':
								if(!GameStarted && MainMenu && !SelectDifficulty)
								{
									CreditsMenu=true;
									MainMenu=false;
								}
								break;

							case 'E':
								if(!GameStarted && !CreditsMenu && !SelectDifficulty)
									PostQuitMessage(0);
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

					int click_r=0, click_c=0;
					if(minimax.TState==Player_Turn)
					{
						Get_ColRow(Point,click_r,click_c);
						if(click_r>=0 && click_r<8 && click_c>=0 && click_c<8)
						{
							minimax.CheckAndExecuteMove(minimax.Board,minimax.White_Soldiers,minimax.Black_Soldiers,White,minimax.Black_Soldier_Count,click_r,click_c);
						}
					}
					else if(TwoPlayers)
					{
						Get_ColRow(Point,click_r,click_c);
						if(click_r>=0 && click_r<8 && click_c>=0 && click_c<8)
						{
							minimax.CheckAndExecuteMove(minimax.Board,minimax.Black_Soldiers,minimax.White_Soldiers,Black,minimax.White_Soldier_Count,click_r,click_c);
						}
					}
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

	WhiteS = Init_White();
	BlackS = Init_Black();
	Menu = Init_Menu();
	Credits = Init_Credits();

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
	
		/* Updating the attributes of the cameras & objects */
		//Client_Handle(Object_List);
		Init_Buffer(100);
		/* Drawing the viewports */
		Init_View(0);
		/* Drawing the objects on the screen */
		//Client_Draw(ViewPort_List, Cross);
		//Client_Draw(ViewPort_List, Circle);
		//Client_Draw(ViewPort_List, Map);
		if(MainMenu)
			Client_Draw(ViewPort_List,Menu);
		

		if(GameStarted)
			minimax.Draw();

		/*Copy buffer to Video RAM*/
		Graphics_Draw(NULL);

		/*Display FrameRate*/
		//Graphics_TextOut(0,0,FrameRateStr, 0, 255, 0);

		if(GameStarted)
		{
			char arr[10];
			sprintf(arr,"%i",minimax.Black_Soldier_Count);
			Graphics_TextOut(33,145,arr, 255, 0, 0);
			sprintf(arr,"%i",minimax.White_Soldier_Count);
			Graphics_TextOut(33,385,arr, 255, 255, 255);


			if(minimax.TState==Player_Turn)
			{
				if(TwoPlayers)
					Graphics_TextOut(13,240,"Player 1", 255,255,255);
				else
					Graphics_TextOut(20,240,"Player", 255,255,255);
			}
			else
			{
				if(TwoPlayers)
					Graphics_TextOut(13,240,"Player 2", 255,0,0);
				else
					Graphics_TextOut(23,240,"AI", 255,0,0);
			}
			if(!TwoPlayers)
			{
				sprintf(arr,"Depth = %i",DEPTH);
				Graphics_TextOut(0,0,arr, 255,0,0);
			}
		}

		if(MainMenu)
		{
			char arr[10];
			sprintf(arr,"%c.",'s');
			Graphics_TextOut(140,245,arr, 255, 0, 0);
			sprintf(arr,"%c.",'m');
			Graphics_TextOut(140,310,arr, 255, 255, 255);
			sprintf(arr,"%c.",'c');
			Graphics_TextOut(140,375,arr, 255, 0, 0);
			sprintf(arr,"%c.",'e');
			Graphics_TextOut(140,425,arr, 255, 255, 255);
		}

		if(CreditsMenu)
		{
			Graphics_TextOut(250,165,"Karl Boghossian", 255, 255, 255);
			Graphics_TextOut(250,185,"Elie Abi Chahine", 255, 255, 255);
			Graphics_TextOut(250,205,"Ziad Hajj", 255, 255, 255);
			Graphics_TextOut(250,225,"Joseph Al Kadi", 255, 255, 255);
			Graphics_TextOut(250,245,"Jean-Paul Hamboyan", 255, 255, 255);
		}

		if(SelectDifficulty)
		{
			char arr[10];
			Graphics_TextOut(280,130,"AI Difficulty", 255, 255, 255);
			Graphics_TextOut(280,140,"------------------", 255, 0, 0);

			for(int i=1; i<8; i++)
			{
				sprintf(arr,"%i",i);
				Graphics_TextOut(195 + i*30,180,arr, 255, 255, 255);
			}

			sprintf(arr,"[ %c ] for fuzzy",'f');
			Graphics_TextOut(130,180,arr, 255, 0, 0);
			if(Keyboard_IsTriggered('F'))
			{
				FuzzyDifficulty=true;
				GameStarted=true;
				SelectDifficulty=false;
				minimax.ResetGame();
			}
			else if(Keyboard_IsTriggered('1'))
			{
				DEPTH=1;
				GameStarted=true;
				SelectDifficulty=false;
				minimax.ResetGame();
			}
			else if(Keyboard_IsTriggered('2'))
			{
				DEPTH=2;
				GameStarted=true;
				SelectDifficulty=false;
				minimax.ResetGame();
			}
			else if(Keyboard_IsTriggered('3'))
			{
				DEPTH=3;
				GameStarted=true;
				SelectDifficulty=false;
				minimax.ResetGame();
			}
			else if(Keyboard_IsTriggered('4'))
			{
				DEPTH=4;
				GameStarted=true;
				SelectDifficulty=false;
				minimax.ResetGame();
			}
			else if(Keyboard_IsTriggered('5'))
			{
				DEPTH=5;
				GameStarted=true;
				SelectDifficulty=false;
				minimax.ResetGame();
			}
			else if(Keyboard_IsTriggered('6'))
			{
				DEPTH=6;
				GameStarted=true;
				SelectDifficulty=false;
				minimax.ResetGame();
			}
			else if(Keyboard_IsTriggered('7'))
			{
				DEPTH=7;
				GameStarted=true;
				SelectDifficulty=false;
				minimax.ResetGame();
			}
		}


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
