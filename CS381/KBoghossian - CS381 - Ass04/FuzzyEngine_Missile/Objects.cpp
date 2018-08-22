#include "Objects.h"
#include "Keyboard.h"
#include "General.h"
#include "Viewing.h"

VECTOR Gravity={GRAVITY_VALUE_X, GRAVITY_VALUE_Y};
float Fuel=1.0f;
extern ViewPort *ViewPort_List;
VECTOR Resultant={0,1};
VECTOR TempDirectionVector={0,1};

float Life_Scale=0;

void Ship_Behavior (Character *Object)
{
	/*if(*Motion<=MAXIMUM_MOTION_SPEED)
	{*/
	//Gravity.y-=0.01f;
	//}
	
	//VECTOR UnitDirection;
	//VECTOR ResultantUnitDirection;

	////TempDirectionVector=(Object->GetSettings()).Vector_Direction;

	//if(Keyboard_IsPressed(VK_UP) && Keyboard_IsPressed(VK_RIGHT))
	//{
	//	(Object->GetSettings()).Angle-=0.1f*(Object->GetSettings()).Angular_Speed;

	//	float Rotation[3][3];
	//	Rotation[0][0]=cos((Object->GetSettings()).Angle);	Rotation[0][1]=-sin((Object->GetSettings()).Angle);	Rotation[0][2]=0;
	//	Rotation[1][0]=sin((Object->GetSettings()).Angle);	Rotation[1][1]=cos((Object->GetSettings()).Angle);	Rotation[1][2]=0;
	//	Rotation[2][0]=Rotation[2][1]=0;																		Rotation[2][2]=1;

	//	Point_Float Pt;
	//
	//	Pt.x=(Object->GetSettings()).Vector_Direction.x;
	//	Pt.y=(Object->GetSettings()).Vector_Direction.y;

	//	Pt = Rotation*Pt;

	//	TempDirectionVector.x=Pt.x;
	//	TempDirectionVector.y=Pt.y;

	//	UnitDirection=GetUnitVector(TempDirectionVector);

	//	TempDirectionVector.x+=0.03f*UnitDirection.x;
	//	TempDirectionVector.y+=0.03f*UnitDirection.y;

	//	Resultant.x+=0.5f*(Gravity.x+TempDirectionVector.x);
	//	Resultant.y+=0.5f*(Gravity.y+TempDirectionVector.y);
	//}
	//else if(Keyboard_IsPressed(VK_UP) && Keyboard_IsPressed(VK_LEFT))
	//{
	//	(Object->GetSettings()).Angle+=0.1f*(Object->GetSettings()).Angular_Speed;

	//	float Rotation[3][3];
	//	Rotation[0][0]=cos((Object->GetSettings()).Angle);	Rotation[0][1]=-sin((Object->GetSettings()).Angle);	Rotation[0][2]=0;
	//	Rotation[1][0]=sin((Object->GetSettings()).Angle);	Rotation[1][1]=cos((Object->GetSettings()).Angle);	Rotation[1][2]=0;
	//	Rotation[2][0]=Rotation[2][1]=0;																		Rotation[2][2]=1;

	//	Point_Float Pt;
	//
	//	Pt.x=(Object->GetSettings()).Vector_Direction.x;
	//	Pt.y=(Object->GetSettings()).Vector_Direction.y;

	//	Pt = Rotation*Pt;

	//	TempDirectionVector.x=Pt.x;
	//	TempDirectionVector.y=Pt.y;

	//	UnitDirection=GetUnitVector(TempDirectionVector);

	//	TempDirectionVector.x+=0.03f*UnitDirection.x;
	//	TempDirectionVector.y+=0.03f*UnitDirection.y;

	//	Resultant.x+=0.5f*(Gravity.x+TempDirectionVector.x);
	//	Resultant.y+=0.5f*(Gravity.y+TempDirectionVector.y);
	//}
	//else if(Keyboard_IsPressed(VK_UP))
	//{
	//	UnitDirection=GetUnitVector(TempDirectionVector);

	//	TempDirectionVector.x+=0.03f*UnitDirection.x;
	//	TempDirectionVector.y+=0.03f*UnitDirection.y;

	//	Resultant.x=(Gravity.x+TempDirectionVector.x);
	//	Resultant.y=(Gravity.y+TempDirectionVector.y);
	//}
	//else
	//{
	//	
	//	Resultant.x+=Gravity.x;
	//	Resultant.y+=Gravity.y;
	//}

	//if(Keyboard_IsPressed(VK_RIGHT))
	//{
	//	(Object->GetSettings()).Angle-=(Object->GetSettings()).Angular_Speed;

	//	float Rotation[3][3];
	//	Rotation[0][0]=cos((Object->GetSettings()).Angle);	Rotation[0][1]=-sin((Object->GetSettings()).Angle);	Rotation[0][2]=0;
	//	Rotation[1][0]=sin((Object->GetSettings()).Angle);	Rotation[1][1]=cos((Object->GetSettings()).Angle);	Rotation[1][2]=0;
	//	Rotation[2][0]=Rotation[2][1]=0;																		Rotation[2][2]=1;

	//	Point_Float Pt;
	//
	//	Pt.x=(Object->GetSettings()).Vector_Direction.x;
	//	Pt.y=(Object->GetSettings()).Vector_Direction.y;

	//	Pt = Rotation*Pt;

	//	TempDirectionVector.x=Pt.x;
	//	TempDirectionVector.y=Pt.y;
	//}
	//else if(Keyboard_IsPressed(VK_LEFT))
	//{
	//	(Object->GetSettings()).Angle+=(Object->GetSettings()).Angular_Speed;

	//	float Rotation[3][3];
	//	Rotation[0][0]=cos((Object->GetSettings()).Angle);	Rotation[0][1]=-sin((Object->GetSettings()).Angle);	Rotation[0][2]=0;
	//	Rotation[1][0]=sin((Object->GetSettings()).Angle);	Rotation[1][1]=cos((Object->GetSettings()).Angle);	Rotation[1][2]=0;
	//	Rotation[2][0]=Rotation[2][1]=0;																		Rotation[2][2]=1;

	//	Point_Float Pt;

	//	Pt.x=(Object->GetSettings()).Vector_Direction.x;
	//	Pt.y=(Object->GetSettings()).Vector_Direction.y;

	//	Pt = Rotation*Pt;

	//	TempDirectionVector.x=Pt.x;
	//	TempDirectionVector.y=Pt.y;
	//}

	//(Object->GetSettings()).Position.x+=Resultant.x;
	//(Object->GetSettings()).Position.y+=Resultant.y;


	/* Rotating the life */
	//if((Object->GetSettings()).Angle<0.2f)
		//(Object->GetSettings()).Angle+=(Object->GetSettings()).Angular_Speed;
		/* Scaling the life */
		if((Object->GetSettings()).Scale==0.1f)
			Life_Scale=-0.001f;
		else if((Object->GetSettings()).Scale<=0.05f)
			Life_Scale=0.001f;
		(Object->GetSettings()).Scale+=Life_Scale;
}

