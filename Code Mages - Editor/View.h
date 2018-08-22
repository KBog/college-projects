#include<fstream>
using namespace std;

#ifndef _FLOAT_RECT_
#define _FLOAT_RECT_
/*Float Rectangle*/
struct Float_Rect
{
	float left,top,right,bottom;
};
#endif

/*Defining ViewPort_Settings and Camera_Settings structure type*/
struct Camera_Settings
{
	float Pos_x,Pos_y;
	float Angle;
	float Scale;
	float Width,Height;
	Float_Rect Scrolling_Box;
};

struct ViewPort_Settings
{
	float Pos_x,Pos_y;
	float Width,Height;
};

void Generate_View(Camera_Settings Main_Camera, Camera_Settings Radar_Camera, ViewPort_Settings Main_ViewPort, ViewPort_Settings Radar_ViewPort);