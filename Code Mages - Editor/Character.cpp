#include"Character.h"
#include"resource.h"
void Character::Draw()
{
	/*Draw All Character Primitives in List*/
	for(Primitive *tmp=Primitives;tmp;tmp=tmp->next)
	{
		/*Draw Primitive According to it's type*/
		switch(tmp->Type)
		{
			case Type_Line:
							LINE(tmp->V[0].x,tmp->V[0].y,tmp->V[1].x,tmp->V[1].y,tmp->Color.r,tmp->Color.g,tmp->Color.b);
							break;
			case Type_Circle:
							CIRCLE(tmp->V[0].x,tmp->V[0].y,(int)tmp->V[1].x,tmp->Color.r,tmp->Color.g,tmp->Color.b);
							break;
			case Type_Filled_Circle:
							FILL_CIRCLE(tmp->V[0].x,tmp->V[0].y,(int)tmp->V[1].x,tmp->Color.r,tmp->Color.g,tmp->Color.b);
							break;
			case Type_Triangle:
							{
								Point_Float p[3];
								for(int i=0;i<3;i++)
								{
									p[i].x=tmp->V[i].x;
									p[i].y=tmp->V[i].y;
								}
								TRIANGLE(p,tmp->Color.r,tmp->Color.g,tmp->Color.b);
							}
							break;
			case Type_Filled_Triangle:
							{
								Point_Float p[3];
								for(int i=0;i<3;i++)
								{
									p[i].x=tmp->V[i].x;
									p[i].y=tmp->V[i].y;
								}
								FILL_TRIANGLE(p,tmp->Color.r,tmp->Color.g,tmp->Color.b);
							}
							break;
		}
	}
}

void Character::DrawCollision()
{
	/*Draw All Collision Primitives in List*/
	for(Collision_Data *tmp=Collisions;tmp;tmp=tmp->next)
	{
		/*Draw Primitive According to it's type*/
		switch(tmp->Type)
		{
			case Type_Collision_Line:
							//LINE(tmp->V[0].x,tmp->V[0].y,tmp->V[1].x,tmp->V[1].y,tmp->Color.r,tmp->Color.g,tmp->Color.b);
							FILL_CIRCLE(tmp->V0.x,tmp->V0.y,2,0,0,255);	//drawing the connections of the beginning of the edge
							COLLISION_LINE(tmp->V0.x,tmp->V0.y,tmp->V1.x,tmp->V1.y,COLLISION_COLOR_R,COLLISION_COLOR_G,COLLISION_COLOR_B);
							FILL_CIRCLE(tmp->V1.x,tmp->V1.y,2,0,0,255);	//drawing the connections of the beginning of the edge
							DrawCollisionNormal(tmp->V0,tmp->V1,tmp->Normal);
							break;
			case Type_Collision_Circle:
							COLLISION_CIRCLE(tmp->V0.x,tmp->V0.y,(int)tmp->V1.x,COLLISION_COLOR_R,COLLISION_COLOR_G,COLLISION_COLOR_B);
							break;
		}
	}
}

void Character::Instance(const Character& src)
{
	/*Create an exact copy of source character*/
	Settings=src.Settings;
	strcpy(Name,src.Name);
	SourceRect=src.SourceRect;
	DestinationRect=src.DestinationRect;
	/*Copy Primitives List*/
	for(Primitive *tmp=src.Primitives;tmp;tmp=tmp->next)
	{
		Primitive *p=new Primitive;
		/*Fast Transfer*/
		*p=*tmp;
		p->next=0;
		/*Add Primitive to Linked-List*/
		AddPrimitive(p);
	}
	/*Copy Collisions List*/
	for(Collision_Data *tmp=src.Collisions;tmp;tmp=tmp->next)
	{
		Collision_Data *p=new Collision_Data;
		/*Fast Transfer*/
		*p=*tmp;
		p->next=0;
		/*Add Primitive to Linked-List*/
		AddCollision(p);
	}
}

void Character::Remove(Primitive *p,Collision_Data *c)
{
	/*If there is a primitive to remove*/
	if(p)
	{
		for(Primitive *tmp=Primitives,*prev=NULL;tmp;prev=tmp,tmp=tmp->next)
		{
			/*In case p is found*/
			if(p==tmp)
			{
				if(prev)
					prev->next=tmp->next;
				else
					Primitives=Primitives->next;
				delete p;
				/*break from loop*/
				break;
			}
		}
	}
	/*If there is a collision data to remove*/
	if(c)
	{
		for(Collision_Data *Ctmp=Collisions,*Cprev=NULL;Ctmp;Cprev=Ctmp,Ctmp=Ctmp->next)
		{
			/*In case p is found*/
			if(c==Ctmp)
			{
				if(Cprev)
					Cprev->next=Ctmp->next;
				else
					Collisions=Collisions->next;
				delete c;
				/*break from loop*/
				break;
			}
		}
	}
}

void DestroyCharacterList(HWND hwnd)
{
	/*Get All Data Associated to each element in list and free it*/
	int Count=(int)SendMessage(GetDlgItem(hwnd,IDC_LIST_CHAR),LB_GETCOUNT,0,0);
	for(int i=0;i<Count;i++)
		delete (Character*)SendMessage(GetDlgItem(hwnd,IDC_LIST_CHAR),LB_GETITEMDATA,i,0);
}