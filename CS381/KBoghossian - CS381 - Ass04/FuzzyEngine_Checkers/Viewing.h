#include <windows.h>
#include "General.h"
#include "Standard Header.h"

#ifndef _VIEWING_H_
#define _VIEWING_H_

/* Define Camera Structure */
struct Camera
{
	/* Camera Position */
	POINT Position;
	/* Store the Plane Dimensions (Camera Size) */
	RECT Rect;
	/* Camera Mouvement and Rotational Speed */
	float Speed, Angular_Speed;
	/* Camera Scale value */
	float Scale;
	/* Scrolling Bounding Box */
	RECT Srolling_Box;
	Camera *next;
};

/* Define ViewPort Structure */
struct ViewPort
{
	/* ViewPort Dimensions */
	float Width, Height;
	/* ViewPort Center */
	POINT Center;
	/* Current_Camera related to the viewport */
	Camera *Current_Camera;
	/* Pointer to the next node */
	ViewPort *next;
};

#endif

/* Functions Prototypes */
void Create_Camera (Camera **Cam_List, POINT Position, RECT Rect, float Speed, float Angular_Speed, float Scale);
void InitCameras (Camera **Cam_List);
void Create_ViewPort (ViewPort **VP_List, POINT Center, float Width, float Height, Camera *Cam);
void InitViewPorts (ViewPort **VP_List);
void Viewing_Transformation (ViewPort *ViewPort, Matrix<3>& Viewing_Matrix);
void DeleteViewPortList (ViewPort *ViewPort_List);
void DeleteCameraList (Camera *Camera_List);