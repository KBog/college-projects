#include<math.h>				
#include"Standard Header.h"				
#include"Clipping.h"				
#include"Collision.h"				
				
void Character::BuildCharacterMatrix(float Matrix[3][3])				
{				
	float Cosine=(float)cos(Settings.Angle),Sine=(float)sin(Settings.Angle);//To avoid Multiple function calls overhead				
	/*A Matrix combination of rotation, scaling and translation (in order)*/				
	Matrix[0][0]=Settings.Scale*Cosine;				
	Matrix[0][1]=-Settings.Scale*Sine;				
	Matrix[0][2]=Settings.Position.x;				
	Matrix[1][0]=Settings.Scale*Sine;				
	Matrix[1][1]=Settings.Scale*Cosine;				
	Matrix[1][2]=Settings.Position.y;				
	Matrix[2][0]=Matrix[2][1]=0;				
	Matrix[2][2]=1;
}

				

void Character::Draw(float Matrix[3][3])				
{				
	Point_Float V[3];				
	for(int i=0;i<Primitives_Num;i++)				
	{				
		/*Filling Temporary Point Float array*/				
		V[0].x=Primitives[i].V0.x;V[1].x=Primitives[i].V1.x;V[2].x=Primitives[i].V2.x;				
		V[0].y=Primitives[i].V0.y;V[1].y=Primitives[i].V1.y;V[2].y=Primitives[i].V2.y;				
				
		/*Get Transformed Vertices*/				
		V[0]=Matrix*V[0];				
		V[1]=Matrix*V[1];				
		V[2]=Matrix*V[2];				
				
		/*Draw Each Primitive Depending on its type*/				
		switch(Primitives[i].Type)				
		{				
			case Type_Line:				
							if(LB_Clipp(&V[0],&V[1]))				
								LINE(V[0].x,V[0].y,V[1].x,V[1].y,Primitives[i].Color.r,Primitives[i].Color.g,Primitives[i].Color.b);				
							break;				
			case Type_Circle:				
							CIRCLE(V[0].x,V[0].y,(int)(Primitives[i].V1.x*Settings.Scale),Primitives[i].Color.r,Primitives[i].Color.g,Primitives[i].Color.b);				
							break;				
			case Type_Filled_Circle:				
							FILL_CIRCLE(V[0].x,V[0].y,(int)(Primitives[i].V1.x*Settings.Scale),Primitives[i].Color.r,Primitives[i].Color.g,Primitives[i].Color.b);				
							break;				
			case Type_Triangle:				
							TRIANGLE(V,Primitives[i].Color.r,Primitives[i].Color.g,Primitives[i].Color.b);				
							break;				
			case Type_Filled_Triangle:				
							FILL_TRIANGLE(V,Primitives[i].Color.r,Primitives[i].Color.g,Primitives[i].Color.b);				
							break;				
		}				
	}				
}


bool Character::CheckCollision(Character &CheckWith) 			
{ 			
	int CheckType=0; 			
	bool CheckCollision=0; 			
	/* Looping through the number of collision data in the calling class */ 			
	for(int i=0; i<this->GetCollisionsCount(); i++) 			
	{ 			
		/* Defining the type of the calling collision data */ 			
		switch(Collisions[i].Type) 			
		{ 			
			/* Line collision type */ 			
			case Type_Collision_Line: 			
				CheckType=0; 			
				break; 			
			/* Circle collision type */ 			
			case Type_Collision_Circle: 			
				CheckType=1; 			
				break; 			
		} 			
		/* Looping through the number of collision data in the with character */ 			
		for(int j=0; j<CheckWith.GetCollisionsCount(); j++) 			
		{ 			
			/* According to the type of collision data */ 			
			switch(CheckWith.Collisions[j].Type) 			
			{ 			
				/* Line collision */ 			
				case Type_Collision_Line: 			
					/* Line with Line collision */ 			
					if(!CheckType) 			
						CheckCollision=Line_Line_Collision(Collisions[i], CheckWith.Collisions[j]); 			
					/* Circle with Line collision */ 			
					else 			
						CheckCollision=Line_Circle_Collision(Collisions[i], CheckWith.Collisions[j]); 			
					break; 			
				/* Circle collision */ 			
				case Type_Collision_Circle: 			
					/* Circle with Line collision */ 			
					if(!CheckType) 			
						CheckCollision=Line_Circle_Collision(CheckWith.Collisions[j], Collisions[i]); 			
					/* Circle with Circle collision */ 			
					else 			
						CheckCollision=Circle_Circle_Collision(Collisions[i], CheckWith.Collisions[j]); 			
					break; 			
			} 			
		} 			
	} 			
	/* Returining the variable that will be 0 or 1 if there was a collision or not */ 			
	return CheckCollision; 			
}

void Character::BoundingCircleCollision (Character &CheckWith) 					
{ 					
	/* Checking if the distance between the two characters is less or equal than the BoundingCircleRadius_1 + BoundingCircleRadius_2 */ 					
	if(sqrt(((Settings.Position.x - CheckWith.Settings.Position.x)*(Settings.Position.x - CheckWith.Settings.Position.x)) + ((Settings.Position.y - CheckWith.Settings.Position.y)*(Settings.Position.y - CheckWith.Settings.Position.y)))<= (BoundingCircleRadius + CheckWith.BoundingCircleRadius)) 					
		CheckCollision(CheckWith); 					
}
