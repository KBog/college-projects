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
#include "resource.h"

#include "Race.h"

#define CELL_WIDTH 60
#define CELL_HEIGHT 60

HWND GlobalHwnd;
HMENU hMenu;
static char FrameRateStr[32]={0};
extern Camera *Camera_List;
extern ViewPort *ViewPort_List;

bool STARTRACE=false;

Character *Map;

Character *WhiteS;
Character *BlackS;
Character *Square;

Race race(80,10,16,4,false,10);

int Source=0, Destination=0, Source_TrapIdx=-1, Destination_TrapIdx=-1;
int clicks=0;
Vertex destVtx;

VERTEX Get_WorldPosition (POINT Point)
{
	VERTEX Pt;
	Pt.x=(float)Point.x-(float)ViewPort_List->Center.x; Pt.y=(float)Point.y-(float)ViewPort_List->Center.y;
	Pt.y=-Pt.y;
	return Pt;
}

void Render_Map (void)
{
	Matrix<3> Object_Matrix, Mapping_Matrix, Final;
	/* Building the viewing matrix */
	Viewing_Transformation(ViewPort_List, Mapping_Matrix);

	Vertex Initial_Pos;
	Initial_Pos.x=-240;
	Initial_Pos.y=180;

	Square->GetSettings().Position.x=Initial_Pos.x;
	Square->GetSettings().Position.y=Initial_Pos.y;

	Square->GetPrimitives()[0].Color.r=Square->GetPrimitives()[1].Color.r=50;
	Square->GetPrimitives()[0].Color.g=Square->GetPrimitives()[1].Color.g=50;
	Square->GetPrimitives()[0].Color.b=Square->GetPrimitives()[1].Color.b=50;

	for(int j=0; j<10; j++)
	{
		Square->GetSettings().Position.x=Initial_Pos.x;
						/* Transforming the objects from their model coodinates to the world coord */
						Square->BuildCharacterMatrix(Object_Matrix.M);
						/* Concatinating the final matrix */
						Final=Mapping_Matrix*Object_Matrix;
						/* Drawing the object */
						Square->Draw(Final.M);
		Square->GetSettings().Position.x=Initial_Pos.x+440;
						/* Transforming the objects from their model coodinates to the world coord */
						Square->BuildCharacterMatrix(Object_Matrix.M);
						/* Concatinating the final matrix */
						Final=Mapping_Matrix*Object_Matrix;
						/* Drawing the object */
						Square->Draw(Final.M);

		Square->GetSettings().Position.y-=40;
	}

	Client_Draw(ViewPort_List, Map);
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
	int wmld, wmEvent;
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
								if(race.Generation_Count)
									STARTRACE=true;
								break;
						}
						return 0;

		case WM_COMMAND:
						wmld = LOWORD (wParam);
						wmEvent = HIWORD (wParam);

						switch(wmld)
						{
							case ID_REPLACEMENT_TOTALREPLACEMENT:
								race.Replacement=NULL;
								EnableMenuItem(hMenu, ID_REPLACEMENT_ELITEMETHOD, MF_ENABLED);
								EnableMenuItem(hMenu, ID_REPLACEMENT_TOTALREPLACEMENT, MF_GRAYED);
								break;
							case ID_REPLACEMENT_ELITEMETHOD:
								race.Replacement=Replacement_Ellitism;
								EnableMenuItem(hMenu, ID_REPLACEMENT_ELITEMETHOD, MF_GRAYED);
								EnableMenuItem(hMenu, ID_REPLACEMENT_TOTALREPLACEMENT, MF_ENABLED);
								break;

							case ID_SELECTION_ROULETTEWHEEL:
								race.Selection=Selection_RouletteWheel;
								EnableMenuItem(hMenu, ID_SELECTION_ROULETTEWHEEL, MF_GRAYED);
								EnableMenuItem(hMenu, ID_SELECTION_TOURNAMENT, MF_ENABLED);
								break;
							case ID_SELECTION_TOURNAMENT:
								race.Selection=Selection_Tournament;
								EnableMenuItem(hMenu, ID_SELECTION_TOURNAMENT, MF_GRAYED);
								EnableMenuItem(hMenu, ID_SELECTION_ROULETTEWHEEL, MF_ENABLED);
								break;

							case ID_CROSSOVER_ORDERPARTIALLY:
								race.Crossover=Crossover_Order_Partially;
								EnableMenuItem(hMenu, ID_CROSSOVER_ORDERPARTIALLY, MF_GRAYED);
								EnableMenuItem(hMenu, ID_CROSSOVER_ORDERPOSITIONBASED, MF_ENABLED);
								break;
							case ID_CROSSOVER_ORDERPOSITIONBASED:
								race.Crossover=Crossover_Order_PositionBased;
								EnableMenuItem(hMenu, ID_CROSSOVER_ORDERPOSITIONBASED, MF_GRAYED);
								EnableMenuItem(hMenu, ID_CROSSOVER_ORDERPARTIALLY, MF_ENABLED);
								break;

							case ID_MUTATION_ORDEREXCHANGE:
								race.Mutate=Mutate_Order_Exchange;
								EnableMenuItem(hMenu, ID_MUTATION_ORDEREXCHANGE, MF_GRAYED);
								EnableMenuItem(hMenu, ID_MUTATION_ORDERINSERTION, MF_ENABLED);
								break;
							case ID_MUTATION_ORDERINSERTION:
								race.Mutate=Mutate_Order_Insertion;
								EnableMenuItem(hMenu, ID_MUTATION_ORDERINSERTION, MF_GRAYED);
								EnableMenuItem(hMenu, ID_MUTATION_ORDEREXCHANGE, MF_ENABLED);
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
	wndclass.lpszMenuName=(LPCSTR)kmenu;							//No menu
	wndclass.lpszClassName="Team Name";					//Set Class name

	cID=RegisterClassEx(&wndclass);						//Register the class and set the return value to cID ATOM

	/*Call create window functionto create the program's window*/
	GlobalHwnd=hwnd=CreateWindow(wndclass.lpszClassName,"KBoghossian - CS381 - Genetic",WS_BORDER|WS_CAPTION|WS_POPUP,(GetSystemMetrics(SM_CXSCREEN)-WINDOW_WIDTH)/2,(GetSystemMetrics(SM_CYSCREEN)-WINDOW_HEIGHT)/2,WINDOW_WIDTH,WINDOW_HEIGHT,NULL,NULL,hInstance,NULL);

	if(!hwnd)
		return 0;

	ShowWindow(hwnd,nCmdShow);							//Show the window after it has been created
	UpdateWindow(hwnd);									//Update the window
	hMenu=GetMenu(hwnd);

	EnableMenuItem(hMenu, White, MF_GRAYED);
	srand((unsigned int)time(NULL));


	EnableMenuItem(hMenu, ID_MUTATION_ORDERINSERTION, MF_GRAYED);
	EnableMenuItem(hMenu, ID_CROSSOVER_ORDERPOSITIONBASED, MF_GRAYED);
	EnableMenuItem(hMenu, ID_REPLACEMENT_ELITEMETHOD, MF_GRAYED);
	EnableMenuItem(hMenu, ID_SELECTION_TOURNAMENT, MF_GRAYED);


	FRC_Init();											//Initialize frame rate
	/* Adding objects to the world */
	Character *Object_List = NULL;
	/* Creating the Cameras to the world */
	InitCameras(&Camera_List);
	/* Creating the viewports to the world */
	InitViewPorts(&ViewPort_List);

	WhiteS = Init_White();
	BlackS = Init_Black();

	Map = Init_Map();
	Square = Init_Square();

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
		Render_Map();

		race.Draw();
		if(!race.BestSolution && STARTRACE)
			race.UpdateRacers();
		

		/*Copy buffer to Video RAM*/
		Graphics_Draw(NULL);

		char arr[100];

		/*Display FrameRate*/
		Graphics_TextOut(0,0,FrameRateStr, 100, 100, 100);

		//POINT Point;
		//GetCursorPos(&Point);	//Get Cursor Position and save it in Point structure
		//ScreenToClient(hwnd,&Point);//Update Point coordinates to client area of window
		//Point.x=(Point.x>0)?Point.x:0;
		//Point.y=(Point.y>0)?Point.y:0;
		//sprintf(arr,"x=%d - y=%d", Point.x,Point.y);
		//Graphics_TextOut(0,430,arr, 0, 220, 0);

		//Vertex WorldPos = Get_WorldPosition(Point);
		//sprintf(arr,"x=%.1f - y=%.1f", WorldPos.x,WorldPos.y);
		//Graphics_TextOut(0,400,arr, 0, 220, 0);

		sprintf(arr,"Smallest:");
		Graphics_TextOut(505,62,arr, 255, 0, 0);
		sprintf(arr,"Largest:");
		Graphics_TextOut(510,452,arr, 100, 0, 100);

		sprintf(arr,"--------");
		for(int i=0; i<POOLSIZE; i++)
			Graphics_TextOut(580,62+i*26,arr, race.Pool[i].color.r, race.Pool[i].color.g, race.Pool[i].color.b);

		//sprintf(arr,"%i",race.NumberOfFinishedRacers);
		//Graphics_TextOut(0,60,arr,200,0,0);

		if(race.BestFitnessScore==5000)
			sprintf(arr,"FirstArrivalTime = 0");
		else
			sprintf(arr,"FirstArrivalTime = %.0f",race.BestFitnessScore);
		Graphics_TextOut(0,20,arr,0,200,0);
		if(race.WorstFitnessScore==-1)
			sprintf(arr,"LastArrivalTime = 0");
		else
			sprintf(arr,"LastArrivalTime = %.0f",race.WorstFitnessScore);
		Graphics_TextOut(0,40,arr,0,200,0);
		if(race.WorstFitnessScore==-1)
			sprintf(arr,"AverageArrivalTime = 0");
		else
		{
			sprintf(arr,"AverageArrivalTime = %.0f",(race.TotalFitnessScore/race.Population_Size));
			Graphics_TextOut(200,30,arr,0,200,0);
		}
		Graphics_TextOut(200,30,arr,0,200,0);

		sprintf(arr,"RemainingGenerations = %i",race.Generation_Count);
		Graphics_TextOut(430,30,arr,0,0,200);

		Graphics_Flip();

		/*if(race.WorstFitnessScore!=-1 && (race.TotalFitnessScore/race.Population_Size)<=360 && race.Generation_Count)
		{
			MessageBox(hwnd, "Best Solution has been reached!", "", (MB_OK));
			race.Generation_Count=0;
		}*/

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
