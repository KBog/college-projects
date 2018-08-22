#include"windows.h"

/*Definitions*/
#define Rows 3
#define Columns 3

typedef struct Float_Rect	
{	
	float left,top,right,bottom;	
}Float_Rect;	
	
/*Defining Camera structure type*/	
typedef struct Camera	
{	
	float Pos_x,Pos_y;		//Camera Position	
	float Angle;			//Camera Angle	
	float Scale;			//Camera Scale	
	float Width,Height;		//Camera Dimensions	
	Float_Rect Srolling_Box;//Scrolling Bounding Box	
}Camera;	
	
typedef struct ViewPort	
{	
	float Pos_x,Pos_y;	
	float Width,Height;	
}ViewPort;
	
/*Function Prototypes*/	
void BuildCameraMatrix(float Matrix[Rows][Columns]);	
void BuildRadarMatrix(float Matrix[Rows][Columns]);
