#include"View.h"
#include"math.h"

/*Cameras*/	
Camera MainCamera={0,0,0,2,200,150,{0,0,0,0}};	
Camera RadarCamera={0,0,0,0.5,200,150,{0,0,0,0}};

/*ViewPorts*/	
ViewPort MainViewPort={480,300,400,300};	
ViewPort RadarViewPort={50,40,100,75};
RECT ClipRect;

void BuildCameraMatrix(float Matrix[Rows][Columns])	
{	
	float Cosine=(float)cos(-MainCamera.Angle*3.1415f/180),Sine=(float)sin(-MainCamera.Angle*3.1415f/180);	
	float Tx=-MainCamera.Pos_x,Ty=-MainCamera.Pos_y,Sx=MainCamera.Scale,Sy=-Sx,Vpx=MainViewPort.Pos_x,Vpy=MainViewPort.Pos_y;	
		
	/*Build camera matrix based on current camera settings*/	
	Matrix[0][0]=Sx*Cosine;	
	Matrix[0][1]=-Sx*Sine;	
	Matrix[0][2]=Vpx+Sx*Cosine*Tx-Sx*Sine*Ty;	
	Matrix[1][0]=Sy*Sine;	
	Matrix[1][1]=Sy*Cosine;	
	Matrix[1][2]=Vpy+Sy*Sine*Tx+Sy*Cosine*Ty;	
	Matrix[2][0]=Matrix[2][1]=0;	
	Matrix[2][2]=1;	
}

void BuildRadarMatrix(float Matrix[Rows][Columns])	
{	
	float Cosine=(float)cos(-RadarCamera.Angle*3.1415f/180),Sine=(float)sin(-RadarCamera.Angle*3.1415f/180);	
	float Tx=-RadarCamera.Pos_x,Ty=-RadarCamera.Pos_y,Sx=RadarCamera.Scale,Sy=-Sx,Vpx=RadarViewPort.Pos_x,Vpy=RadarViewPort.Pos_y;	
		
	/*Build camera matrix based on current camera settings*/	
	Matrix[0][0]=Sx*Cosine;	
	Matrix[0][1]=-Sx*Sine;	
	Matrix[0][2]=Vpx+Sx*Cosine*Tx-Sx*Sine*Ty;	
	Matrix[1][0]=Sy*Sine;	
	Matrix[1][1]=Sy*Cosine;	
	Matrix[1][2]=Vpy+Sy*Sine*Tx+Sy*Cosine*Ty;	
	Matrix[2][0]=Matrix[2][1]=0;	
	Matrix[2][2]=1;	
}
