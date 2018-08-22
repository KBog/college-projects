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

#include "Graph.h"
#include "Trapezoidal_Algorithm.h"

HWND GlobalHwnd;
static char FrameRateStr[32]={0};
extern Camera *Camera_List;
extern ViewPort *ViewPort_List;

Character *MainCharacter;
Character *NodeShape;
Character *Solid_Map;
Graph *graph;
Trapezoidal_Algorithm *Trapezoidal;

int Source=0, Destination=0, Source_TrapIdx=-1, Destination_TrapIdx=-1;;
int clicks=0;
Vertex destVtx;
string AI_Type="Floyd Shortest Path";

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
							case VK_F3:
								AI_Type = "Floyd Shortest Path";
								Trapezoidal->graph->ShortestPathType = Floyd_Type;
								break;
							case VK_F2:
								AI_Type = "BellmanFordMoor Shortest Path";
								Trapezoidal->graph->ShortestPathType = BellmanFordMoor_Type;
								break;
							case VK_F1:
								AI_Type = "Djikstra Shortest Path";
								Trapezoidal->graph->ShortestPathType = Dijkstra_Type;
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
	
				/* While we are not traversing a path */
				if(!Trapezoidal->graph->Traversing_Path)
				{
					int tmp_state=0, tmp_trapstate=0;
					/* Convert the cursor pos to the world */
					Vertex WorldPos = Get_WorldPosition(Point);
					/* save the state of the current point if it's not a solid */
					if((tmp_state=Trapezoidal->Get_TrapezoidState(WorldPos))!=-1)
					{
						tmp_trapstate=tmp_state;
						/* Get the nearest node */
						tmp_state = Trapezoidal->graph->Get_NearestNode(tmp_state,WorldPos);
						if(clicks==0)
						{
							clicks++;
							Source=tmp_state;
							Source_TrapIdx=tmp_trapstate;
							char arr[100];
							sprintf(arr,"%i",Source);
							MessageBox(hwnd, arr, "Source", (MB_OK));
							MainCharacter->GetSettings().Position = WorldPos;
						}
						else
						{
							clicks=0;
							Destination=tmp_state;
							Destination_TrapIdx=tmp_trapstate;
							destVtx=WorldPos;
							char arr[100];
							sprintf(arr,"%i",Destination);
							MessageBox(hwnd, arr, "Destination", (MB_OK));
							string path_str = Trapezoidal->graph->Get_Path(Source,Destination);
							MessageBox(hwnd, path_str.c_str(), "Shortest Path", (MB_OK));
						}
					}
					else
						MessageBox(hwnd, "Invalid - Solid Area!", "Application Info", (MB_OK | MB_ICONINFORMATION));
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
	GlobalHwnd=hwnd=CreateWindow(wndclass.lpszClassName,"KBoghossian - CS381 - AI: Trapezoidal",WS_BORDER|WS_CAPTION|WS_MINIMIZEBOX|WS_POPUP|WS_SYSMENU,(GetSystemMetrics(SM_CXSCREEN)-WINDOW_WIDTH)/2,(GetSystemMetrics(SM_CYSCREEN)-WINDOW_HEIGHT)/2,WINDOW_WIDTH,WINDOW_HEIGHT,NULL,NULL,hInstance,NULL);

	if(!hwnd)
		return 0;

	ShowWindow(hwnd,nCmdShow);							//Show the window after it has been created
	UpdateWindow(hwnd);									//Update the window

	FRC_Init();											//Initialize frame rate
	/* Adding objects to the world */
	Character *Object_List = NULL;
	/* Creating the Cameras to the world */
	InitCameras(&Camera_List);
	/* Creating the viewports to the world */
	InitViewPorts(&ViewPort_List);

	MainCharacter = Init_Characters();
	MainCharacter->pf=Ship_Behavior;

	NodeShape = Init_NodeShape();
	//graph = new Graph(6);

	Solid_Map = Init_Map();
	Trapezoidal = new Trapezoidal_Algorithm(Solid_Map->GetCollisionsCount(),Solid_Map->GetCollisions());
	
	MessageBox(hwnd, "F1 = Dijkstra\nF2 = BellmanFordMoor\nF3 = Floyd", "How to choose the ShortestPath Algorithm", (MB_OK | MB_ICONINFORMATION));
	//VERTEX Pos;

	/*Pos.x=0;
	Pos.y=50;
	graph->Add_Node(Pos.x,Pos.y);
	Pos.x=130;
	Pos.y=-30;
	graph->Add_Node(Pos.x,Pos.y);
	Pos.x=75;
	Pos.y=-80;
	graph->Add_Node(Pos.x,Pos.y);
	Pos.x=0;
	Pos.y=-170;
	graph->Add_Node(Pos.x,Pos.y);
	Pos.x=-90;
	Pos.y=-130;
	graph->Add_Node(Pos.x,Pos.y);
	Pos.x=-160;
	Pos.y=-60;
	graph->Add_Node(Pos.x,Pos.y);*/

	//graph->Init_Nodes();
	//graph->Get_Path(0,4);

	//Trapezoidal->graph->Get_Path(4,0);

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
		Client_Handle(Object_List);
		Trapezoidal->graph->Traverse_Path();
		Init_Buffer(100);
		/* Drawing the viewports */
		Init_View(0);
		/* Drawing the objects on the screen */
		Client_Draw(ViewPort_List, Object_List);
		Client_Draw(ViewPort_List, Solid_Map);
		//graph->Draw(ViewPort_List);
		Trapezoidal->Draw();

		/*Copy buffer to Video RAM*/
		Graphics_Draw(NULL);

		/*Display FrameRate*/
		Graphics_TextOut(0,0,FrameRateStr, 255, 0, 0);
		Trapezoidal->graph->DisplayNodeIndices(ViewPort_List);
		Graphics_TextOut(0,460,AI_Type.c_str(),255,0,0);


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
