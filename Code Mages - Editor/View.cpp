#include"View.h"

void Generate_View(Camera_Settings Main_Camera, Camera_Settings Radar_Camera, ViewPort_Settings Main_ViewPort, ViewPort_Settings Radar_ViewPort)
{
	/*View Header*/
	/////////////////////////////////////////////////////////////////////////////////////////
	ofstream View_Header("View.h",ios_base::out);
	/*Header Includes*/View_Header<<"#include\"windows.h\"\n"<<endl;
	/*Definitions*/
	View_Header<<"/*Definitions*/\n#define Rows 3\n#define Columns 3\n"<<endl;

	/*Defining Structure Types*/
	View_Header<<"typedef struct Float_Rect\
	\n{\
	\n	float left,top,right,bottom;\
	\n}Float_Rect;\
	\n\
	\n/*Defining Camera structure type*/\
	\ntypedef struct Camera\
	\n{\
	\n	float Pos_x,Pos_y;		//Camera Position\
	\n	float Angle;			//Camera Angle\
	\n	float Scale;			//Camera Scale\
	\n	float Width,Height;		//Camera Dimensions\
	\n	Float_Rect Srolling_Box;//Scrolling Bounding Box\
	\n}Camera;\
	\n\
	\ntypedef struct ViewPort\
	\n{\
	\n	float Pos_x,Pos_y;\
	\n	float Width,Height;\
	\n}ViewPort;"<<endl;

	/*Function Prototypes*/
	View_Header<<"\
	\n/*Function Prototypes*/\
	\nvoid BuildCameraMatrix(float Matrix[Rows][Columns]);\
	\nvoid BuildRadarMatrix(float Matrix[Rows][Columns]);"<<endl;
	View_Header.close();
	/////////////////////////////////////////////////////////////////////////////////////////

	/*View Source File*/
	/////////////////////////////////////////////////////////////////////////////////////////
	ofstream View("View.cpp",ios_base::out);
	/*Header Includes*/View<<"#include\"View.h\"\n#include\"math.h\"\n"<<endl;
	/*Cameras*/View<<"/*Cameras*/\
	\nCamera MainCamera={"<<Main_Camera.Pos_x<<","<<Main_Camera.Pos_y<<","<<Main_Camera.Angle<<","<<Main_Camera.Scale<<","<<Main_Camera.Width<<","<<Main_Camera.Height<<","<<"{"<<Main_Camera.Scrolling_Box.left<<","<<Main_Camera.Scrolling_Box.top<<","<<Main_Camera.Scrolling_Box.right<<","<<Main_Camera.Scrolling_Box.bottom<<"}"<<"};\
	\nCamera RadarCamera={"<<Radar_Camera.Pos_x<<","<<Radar_Camera.Pos_y<<","<<Radar_Camera.Angle<<","<<Radar_Camera.Scale<<","<<Radar_Camera.Width<<","<<Radar_Camera.Height<<","<<"{"<<Radar_Camera.Scrolling_Box.left<<","<<Radar_Camera.Scrolling_Box.top<<","<<Radar_Camera.Scrolling_Box.right<<","<<Radar_Camera.Scrolling_Box.bottom<<"}"<<"};\n"<<endl;

	/*ViewPorts*/View<<"/*ViewPorts*/\
	\nViewPort MainViewPort={"<<Main_ViewPort.Pos_x<<","<<Main_ViewPort.Pos_y<<","<<Main_ViewPort.Width<<","<<Main_ViewPort.Height<<"};\
	\nViewPort RadarViewPort={"<<Radar_ViewPort.Pos_x<<","<<Radar_ViewPort.Pos_y<<","<<Radar_ViewPort.Width<<","<<Radar_ViewPort.Height<<"};"<<endl;
	View<<"RECT ClipRect;\n"<<endl;

	/*Build Camera Matrix function*/
	View<<"void BuildCameraMatrix(float Matrix[Rows][Columns])\
	\n{\
	\n	float Cosine=(float)cos(-MainCamera.Angle*3.1415f/180),Sine=(float)sin(-MainCamera.Angle*3.1415f/180);\
	\n	float Tx=-MainCamera.Pos_x,Ty=-MainCamera.Pos_y,Sx=MainCamera.Scale,Sy=-Sx,Vpx=MainViewPort.Pos_x,Vpy=MainViewPort.Pos_y;\
	\n	\
	\n	/*Build camera matrix based on current camera settings*/\
	\n	Matrix[0][0]=Sx*Cosine;\
	\n	Matrix[0][1]=-Sx*Sine;\
	\n	Matrix[0][2]=Vpx+Sx*Cosine*Tx-Sx*Sine*Ty;\
	\n	Matrix[1][0]=Sy*Sine;\
	\n	Matrix[1][1]=Sy*Cosine;\
	\n	Matrix[1][2]=Vpy+Sy*Sine*Tx+Sy*Cosine*Ty;\
	\n	Matrix[2][0]=Matrix[2][1]=0;\
	\n	Matrix[2][2]=1;\
	\n}\n"<<endl;

	/*Build Radar Matrix function*/
	View<<"void BuildRadarMatrix(float Matrix[Rows][Columns])\
	\n{\
	\n	float Cosine=(float)cos(-RadarCamera.Angle*3.1415f/180),Sine=(float)sin(-RadarCamera.Angle*3.1415f/180);\
	\n	float Tx=-RadarCamera.Pos_x,Ty=-RadarCamera.Pos_y,Sx=RadarCamera.Scale,Sy=-Sx,Vpx=RadarViewPort.Pos_x,Vpy=RadarViewPort.Pos_y;\
	\n	\
	\n	/*Build camera matrix based on current camera settings*/\
	\n	Matrix[0][0]=Sx*Cosine;\
	\n	Matrix[0][1]=-Sx*Sine;\
	\n	Matrix[0][2]=Vpx+Sx*Cosine*Tx-Sx*Sine*Ty;\
	\n	Matrix[1][0]=Sy*Sine;\
	\n	Matrix[1][1]=Sy*Cosine;\
	\n	Matrix[1][2]=Vpy+Sy*Sine*Tx+Sy*Cosine*Ty;\
	\n	Matrix[2][0]=Matrix[2][1]=0;\
	\n	Matrix[2][2]=1;\
	\n}"<<endl;
}