#include "Viewing.h"
#include "Keyboard.h"
#include "Settings.h"
#include <math.h>

Camera *Camera_List;
ViewPort *ViewPort_List;
RECT ClipRect;

void Create_Camera (Camera **Cam_List, POINT Position, RECT Rect, float Speed, float Angular_Speed, float Scale)
{
	/* Defining a new camera */
	Camera *Cam = new Camera;
	/* Defining the Position of the camera */
	Cam->Position=Position;
	/* defining the dimensions of the camera */
	Cam->Rect = Rect;
	/* Defining the camera speed */
	Cam->Speed = Speed;
	/* Defining the camera Angular Speed */
	Cam->Angular_Speed = Angular_Speed;
	/* Defining the camera scale value */
	Cam->Scale = Scale;
	/* Initializing the next camera to 0 */
	Cam->next = 0;

	/* Adding the camera to the list of cameras */
	if(!(*Cam_List))
		*Cam_List = Cam;
	else
	{
		for(Camera *tmp = *Cam_List; tmp->next; tmp = tmp->next);
		tmp->next = Cam;
	}
}

void InitCameras (Camera **Cam_List)
{
	/* Main Camera Attributes */
	/* Defining the dimensions of the camera */
	RECT Rect={0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
	/* Defining the Camera Position */
	POINT Position={0, 0};
	/* Adding the first Camera to the list */
	Create_Camera(Cam_List, Position, Rect, 0, 0, 1);
}

void Create_ViewPort (ViewPort **VP_List, POINT Center, float Width, float Height, Camera *Cam)
{
	/* defining a new viewport */
	ViewPort *VP = new ViewPort;
	/* defining the viewport center */
	VP->Center = Center;
	/* defining the dimensions of the viewport */
	VP->Width = Width;	VP->Height=Height;
	/* Linking the current camera to the current viewport */
	VP->Current_Camera = Cam;
	/* Initializing the next viewport to 0 */
	VP->next = 0;

	/* Adding the viewport to the list of viewports */
	if(!(*VP_List))
		*VP_List = VP;
	else
	{
		for(ViewPort *tmp = *VP_List; tmp->next; tmp = tmp->next);
		tmp->next = VP;
	}
}

void InitViewPorts (ViewPort **VP_List)
{
	/* Camera temporary variable */
	Camera *Main_Camera=Camera_List;
	/* Main VP attributes */
	/* Center of the first VP */
	POINT Center={WINDOW_WIDTH/2, WINDOW_HEIGHT/2};
	/* Creating and adding the first VP to the list */
	Create_ViewPort(VP_List, Center, WINDOW_WIDTH, WINDOW_HEIGHT, Main_Camera);
}

void Viewing_Transformation (ViewPort *ViewPort, Matrix<3>& Viewing_Matrix)
{
	/* Modify the clip rectangle */
	ClipRect.left=long(ViewPort->Center.x-ViewPort->Width/2);
	ClipRect.right=long(ViewPort->Center.x+ViewPort->Width/2);
	ClipRect.top=long(ViewPort->Center.y-ViewPort->Height/2);
	ClipRect.bottom=long(ViewPort->Center.y+ViewPort->Height/2);
	
	/* Taking the cosine of the camera angle_z */
	float Cosine = (float)cos(-ViewPort->Current_Camera->Angular_Speed*3.1415f/180);
	/* Taking the sine of the camera angle_z */
	float Sine = (float)sin(-ViewPort->Current_Camera->Angular_Speed*3.1415f/180);
	///* Taking the camera center x and -y (world to screen) */
	///* COULD BE - IN C_Cy */
	//float C_Cx=ViewPort->Current_Camera->Position.x, C_Cy=ViewPort->Current_Camera->Position.y;
	///* Taking the scale factor */
	//float C_Sx=ViewPort->Current_Camera->Scale, C_Sy=-C_Sx;
	///* Viewport center x and y */
	//float V_Cx=(float)ViewPort->Center.x, V_Cy=(float)ViewPort->Center.y;
	///* Filling the viewing matrix */


	float Tx=-(float)ViewPort->Current_Camera->Position.x, Ty=-(float)ViewPort->Current_Camera->Position.y;
	float Sx=ViewPort->Current_Camera->Scale, Sy=-Sx;
	float Vpx=(float)ViewPort->Center.x, Vpy=(float)ViewPort->Center.y;	
		
	/*Build camera matrix based on current camera settings*/	
	Viewing_Matrix.M[0][0]=Sx*Cosine;	
	Viewing_Matrix.M[0][1]=-Sx*Sine;	
	Viewing_Matrix.M[0][2]=Vpx+Sx*Cosine*Tx-Sx*Sine*Ty;	
	Viewing_Matrix.M[1][0]=Sy*Sine;	
	Viewing_Matrix.M[1][1]=Sy*Cosine;	
	Viewing_Matrix.M[1][2]=Vpy+Sy*Sine*Tx+Sy*Cosine*Ty;	
	Viewing_Matrix.M[2][0]=Viewing_Matrix.M[2][1]=0;	
	Viewing_Matrix.M[2][2]=1;	
}

/* Function to delete the memory allocated for the viewports */
void DeleteViewPortList (ViewPort *ViewPort_List)
{
	ViewPort *Next=NULL;
	while(ViewPort_List)
	{
		Next=ViewPort_List->next;
		delete ViewPort_List;
		ViewPort_List=Next;
	}
}

/* Function to delete the memory allocated for the cameras */
void DeleteCameraList (Camera *Camera_List)
{
	Camera *Next=NULL;
	while(Camera_List)
	{
		Next=Camera_List->next;
		delete Camera_List;
		Camera_List=Next;
	}
}