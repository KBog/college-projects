#include<math.h>
#include"Collision.h"
#include"Graphics.h"

#define INTERPOLATION_FACTOR 30
#define NORMAL_LEN 15

Collision_Data *BuildCollision(Vertex V0,Vertex V1,enum Collision_Type type)
{
	/*Building New Collision Data*/
	Collision_Data* p=new Collision_Data;
	p->Type=type;
	p->next=0;
	/*Collision Vertices*/
	p->V0=V0;
	p->V1=V1;
	/*Collision Vector in case of Line*/
	if(type==Type_Collision_Line)
	{
		Vector V;
		/*Line Vector Direction*/
		V.x=V1.x-V0.x;V.y=V1.y-V0.y;
		float Magnitude=(float)sqrt(V.x*V.x+V.y*V.y);
		/*Collision Normal*/
		if(Magnitude)//Avoid Division by zero
			{p->Normal.x=-V.y/Magnitude;p->Normal.y=V.x/Magnitude;}
		else
			p->Normal.x=p->Normal.y=0;
	}
	else
		p->Normal.x=p->Normal.y=0;

	/*Return Built Collision Data*/
	return p;
}

void DrawCollisionNormal(Vertex V0,Vertex V1,Vector N)
{
	Vector V;			//Unit Vector Direction of line
	Vertex N_V;			//Second Vertex of Normal

	float Magnitude;
	V.x=V1.x-V0.x;
	V.y=V1.y-V0.y;
	int Loop_Times=(int)(Magnitude=(float)sqrt((V.x*V.x+V.y*V.y)))/INTERPOLATION_FACTOR;
	/*Make V a unit vector*/
	V.x/=Magnitude;
	V.y/=Magnitude;

	if(!Loop_Times)			//Line is too small
	{
		/*Get Middle of the Line*/
		V0.x=(V1.x+V0.x)/2;
		V0.y=(V1.y+V0.y)/2;
		/*Ending edge of Normal*/
		N_V.x=V0.x+NORMAL_LEN*N.x;
		N_V.y=V0.y+NORMAL_LEN*N.y;
		/*Clip and Draw Normal*/
		Point_Float P1={V0.x,V0.y},P2={N_V.x,N_V.y};
		if(LB_Clipp(&P1,&P2))
			COLLISION_LINE(P1.x,P1.y,P2.x,P2.y,COLLISION_COLOR_R,COLLISION_COLOR_G,COLLISION_COLOR_B);
		return;
	}
	while(Loop_Times--)
	{
		/*Walk on line*/
		V0.x+=INTERPOLATION_FACTOR*V.x;
		V0.y+=INTERPOLATION_FACTOR*V.y;
		/*Ending edge of Normal*/
		N_V.x=V0.x+NORMAL_LEN*N.x;
		N_V.y=V0.y+NORMAL_LEN*N.y;
		/*Clip and Draw Normal*/
		Point_Float P1={V0.x,V0.y},P2={N_V.x,N_V.y};
		if(LB_Clipp(&P1,&P2))
			COLLISION_LINE(P1.x,P1.y,P2.x,P2.y,COLLISION_COLOR_R,COLLISION_COLOR_G,COLLISION_COLOR_B);
	}
}