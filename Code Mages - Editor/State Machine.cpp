#include"math.h"						//Include the Math.h header file
#include"resource.h"					//Include the Resource.h header file
#include"Graphics.h"					//Include the Graphics.h header file
#include"State Machine.h"				//Include the State Machine.h header file
#include"Character.h"					//Include the Character.h header file
extern enum Algorithm Algorithm_Selection;			//Declaring an external integer
extern Character* Current_Character;
extern RECT SourceRect;

POINT ppt[MAXCLICKS];					//Declaring a Global array of POINT structures
int num=0;								//Declaring a Global integer

bool PointsNumber(HWND hwnd,POINT pt,Color_RGB Color)
{
	int MaxNum=2;
	if(Algorithm_Selection==Algorithm_Triangle||Algorithm_Selection==Algorithm_Filled_Triangle)
		MaxNum=3;
	if(num==MaxNum){num=0;return true;}
	ppt[num]=pt;						//Set corresponding element of ppt array to pt
	Graphics_WritePixel(pt.x,pt.y,Color.r,Color.g,Color.b);//Set corresponding Pixel on screen using BLACK color
	num++;								//Increment num variable by 1
	if(num==MaxNum)						//if num is equal to MAXCLICKS
	{
		num=0;							//Reset num to zero
		return true;					//return true
	}
	else return false;					//else return false
}

void Draw(HWND hwnd,POINT pt,Color_RGB Color)
{
	if(Algorithm_Selection&&PointsNumber(hwnd,pt,Color))		//if Algorithm_Selection variable is not zero and PointsNumber function returned true
	{
		Vertex Vertices[3];
		switch(Algorithm_Selection)						//switch Algorithm_Selection variable
		{
			case Algorithm_Line:
									LINE(Vertices[0].x=(float)ppt[0].x,Vertices[0].y=(float)ppt[0].y,Vertices[1].x=(float)ppt[1].x,Vertices[1].y=(float)ppt[1].y,Color.r,Color.g,Color.b);
									ppt[0]=ppt[1];				//copy the second point to the first one
									num=1;						//readjust the click counter
									/*Build Line Primitive*/
									Current_Character->AddPrimitive(BuildPrimitive(Vertices,Color,Type_Line));
									break;
			case Algorithm_Circle:
									/*Fix Out of Circle points*/
									Graphics_WritePixel(ppt[0].x,ppt[0].y,0,0,0);
									Graphics_WritePixel(ppt[1].x,ppt[1].y,0,0,0);

									/* Getting center */
									Vertices[0].x=(float)(ppt[1].x+ppt[0].x)/2;
									Vertices[0].y=(float)(ppt[1].y+ppt[0].y)/2;
									/* Getting radius */
									Vertices[1].x=Vertices[1].y=(float)((sqrt(pow(ppt[1].x-ppt[0].x,2)+pow(ppt[1].y-ppt[0].y,2)))/2);
									/* Check if circle will be in rectangle */
									if(((Vertices[0].x-Vertices[1].x)>SourceRect.left && (Vertices[0].x+Vertices[1].x)<SourceRect.right) 
										&& ((Vertices[0].y-Vertices[1].x)>SourceRect.top && (Vertices[0].y+Vertices[1].x)<SourceRect.bottom))
									{
										/*Call circle Drawing function*/
										CIRCLE(Vertices[0].x,Vertices[0].y,(int)(Vertices[1].x),Color.r,Color.g,Color.b);
									
										/*Build Circle Primitive*/
										Current_Character->AddPrimitive(BuildPrimitive(Vertices,Color,Type_Circle));
									}
									break;

			case Algorithm_Filled_Circle:
									/*Fix Out of Circle points*/
									Graphics_WritePixel(ppt[0].x,ppt[0].y,0,0,0);
									Graphics_WritePixel(ppt[1].x,ppt[1].y,0,0,0);

									/* Getting center */
									Vertices[0].x=(float)(ppt[1].x+ppt[0].x)/2;
									Vertices[0].y=(float)(ppt[1].y+ppt[0].y)/2;
									/* Getting radius */
									Vertices[1].x=Vertices[1].y=(float)((sqrt(pow(ppt[1].x-ppt[0].x,2)+pow(ppt[1].y-ppt[0].y,2)))/2);
									/* Check if circle will be in rectangle */
									if(((Vertices[0].x-Vertices[1].x)>SourceRect.left && (Vertices[0].x+Vertices[1].x)<SourceRect.right) 
										&& ((Vertices[0].y-Vertices[1].x)>SourceRect.top && (Vertices[0].y+Vertices[1].x)<SourceRect.bottom))
									{
										/*Call circle Drawing function*/
										FILL_CIRCLE(Vertices[0].x,Vertices[0].y,(int)(Vertices[1].x),Color.r,Color.g,Color.b);
									
										/*Build Circle Primitive*/
										Current_Character->AddPrimitive(BuildPrimitive(Vertices,Color,Type_Filled_Circle));
									}

									break;

			case Algorithm_Triangle:
									{
										int i;
										Point_Float p[3];
									
										/*Fix Out of Triangle points*/
										Graphics_WritePixel(ppt[0].x,ppt[0].y,0,0,0);
										Graphics_WritePixel(ppt[1].x,ppt[1].y,0,0,0);
										Graphics_WritePixel(ppt[2].x,ppt[2].y,0,0,0);

										for(i=0;i<3;i++)
										{
											p[i].x=Vertices[i].x=(float)ppt[i].x;
											p[i].y=Vertices[i].y=(float)ppt[i].y;
										}

										TRIANGLE(p,Color.r,Color.g,Color.b);

										/*Build Triangle*/
										Current_Character->AddPrimitive(BuildPrimitive(Vertices,Color,Type_Triangle));
										break;
									}

			case Algorithm_Filled_Triangle:
									{
										int i;
										Point_Float p[3];
									
										/*Fix Out of Triangle points*/
										Graphics_WritePixel(ppt[0].x,ppt[0].y,0,0,0);
										Graphics_WritePixel(ppt[1].x,ppt[1].y,0,0,0);
										Graphics_WritePixel(ppt[2].x,ppt[2].y,0,0,0);

										for(i=0;i<3;i++)
										{
											p[i].x=(float)ppt[i].x;
											p[i].y=(float)ppt[i].y;
										}

										/*Sort Triangle Vertices*/
										SortVertices(p);

										/*Copy Sorted Vertices*/
										for(i=0;i<3;i++)
										{
											Vertices[i].x=p[i].x;
											Vertices[i].y=p[i].y;
										}

										FILL_TRIANGLE(p,Color.r,Color.g,Color.b);
										
										/*Build Triangle*/
										Current_Character->AddPrimitive(BuildPrimitive(Vertices,Color,Type_Filled_Triangle));
										break;
									}
			case Algorithm_Collision_Line:
									{
										Collision_Data *tmp;
										FILL_CIRCLE((float)(ppt[0].x),(float)(ppt[0].y),2,0,0,255);	//drawing the connections of the beginning of the edge
										COLLISION_LINE(Vertices[0].x=(float)ppt[0].x,Vertices[0].y=(float)ppt[0].y,Vertices[1].x=(float)ppt[1].x,Vertices[1].y=(float)ppt[1].y,COLLISION_COLOR_R,COLLISION_COLOR_G,COLLISION_COLOR_B);
										ppt[0]=ppt[1];												//copying the second point to the first one
										FILL_CIRCLE((float)(ppt[0].x),(float)(ppt[0].y),2,0,0,255);	//drwaing the connections of the end of the edge
										/* Adding the line segment collision to the list */
										Current_Character->AddCollision(tmp=BuildCollision(Vertices[0], Vertices[1], Type_Collision_Line));
										DrawCollisionNormal(tmp->V0,tmp->V1,tmp->Normal);
										num=1;														//readjust the counter
									}
									break;

			case Algorithm_Collision_Circle:
									/*Fix Out of Circle points*/
									Graphics_WritePixel(ppt[0].x,ppt[0].y,0,0,0);
									Graphics_WritePixel(ppt[1].x,ppt[1].y,0,0,0);

									/* Getting center */
									Vertices[0].x=(float)(ppt[1].x+ppt[0].x)/2;
									Vertices[0].y=(float)(ppt[1].y+ppt[0].y)/2;
									/* Getting radius */
									Vertices[1].x=Vertices[1].y=(float)((sqrt(pow(ppt[1].x-ppt[0].x,2)+pow(ppt[1].y-ppt[0].y,2)))/2);
									/* Check if circle will be in rectangle */
									if(((Vertices[0].x-Vertices[1].x)>SourceRect.left && (Vertices[0].x+Vertices[1].x)<SourceRect.right) 
										&& ((Vertices[0].y-Vertices[1].x)>SourceRect.top && (Vertices[0].y+Vertices[1].x)<SourceRect.bottom))
									{
										/*Call circle Drawing function*/
										COLLISION_CIRCLE(Vertices[0].x,Vertices[0].y,(int)(Vertices[1].x),COLLISION_COLOR_R,COLLISION_COLOR_G,COLLISION_COLOR_B);
									
										/*Build Circle Collision*/
										Current_Character->AddCollision(BuildCollision(Vertices[0], Vertices[1], Type_Collision_Circle));
									}
									break;
		}
	}
		/*Refresh*/
		Graphics_Draw(NULL);
		Graphics_Flip();
}


