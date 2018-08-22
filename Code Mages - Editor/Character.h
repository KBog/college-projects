#include"Graphics.h"

#ifndef _VECTOR_STRUCT_
#define _VECTOR_STRUCT_
/*Vector Structure*/
struct Vector
{
	float x,y;
};
#endif

#ifndef _VERTEX_STRUCT_
#define _VERTEX_STRUCT_
/*Vertex Structure*/
struct Vertex
{
	float x,y;
};
#endif

/*Character Settings*/
struct Character_Settings
{
	Vertex Position;
	float Scale;
	float Angle;
	float Speed,Angular_Speed;
	Vector Vector_Direction;
};

/*Character class used for editor*/
class Character
{
	private:
			Primitive *Primitives;
			Collision_Data *Collisions;
			int Primitives_Num,Collisions_Num;
			Character_Settings Settings;
			char Name[50];
			RECT SourceRect, DestinationRect;
	public:
			/*Constructor*/
			Character():Primitives(0),Collisions(0),Primitives_Num(0),Collisions_Num(0)
			{
				memset(&Settings,0,sizeof(Settings));
				Settings.Scale=1;
				SourceRect.left=0; SourceRect.right=639; SourceRect.top=0; SourceRect.bottom=450;
				DestinationRect=SourceRect;
			}
			/*Destructor*/
			~Character()
			{
				/*Destroy Primitives List*/
				for(Primitive *tmp;Primitives;Primitives=tmp)
				{
					tmp=Primitives->next;
					delete Primitives;
				}
				/*Destroy Collisions List*/
				for(Collision_Data *tmp;Collisions;Collisions=tmp)
				{
					tmp=Collisions->next;
					delete Collisions;
				}
			}
			/*Get Primitives and Collisions*/
			Primitive *GetPrimitives(void){return Primitives;}
			Collision_Data *GetCollisions(void){return Collisions;}
			/*Get Primitives and Collisions Count*/
			int GetPrimitivesCount(void){return Primitives_Num;}
			int GetCollisionsCount(void){return Collisions_Num;}

			/*Function that Sets Character Settings*/
			void Set(Vertex Position,float Scale,float Angle,float Speed,float Angular_Speed,Vector Vector_Direction)
			{
				/*Set Character Settings*/
				Settings.Position=Position;
				Settings.Scale=Scale;
				Settings.Angle=Angle;
				Settings.Speed=Speed;
				Settings.Angular_Speed=Angular_Speed;
				Settings.Vector_Direction=Vector_Direction;
			}

			void AddPrimitive(Primitive *p)
			{
				/*Add Current Primitive to List*/
				if(!Primitives)
					Primitives=p;
				else
				{
					for(Primitive *tmp=Primitives;tmp->next;tmp=tmp->next);
					tmp->next=p;
				}
				/*Increment Primitives Number*/
				Primitives_Num++;
			}
			void AddCollision(Collision_Data *c)
			{
				/*Add Current Collision to List*/
				if(!Collisions)
					Collisions=c;
				else
				{
					for(Collision_Data *tmp=Collisions;tmp->next;tmp=tmp->next);
					tmp->next=c;
				}
				/*Increment Collisions Number*/
				Collisions_Num++;
			}
			/*Function To Draw the Character*/
			void Draw();
			void DrawCollision();

			/*Function to return the name*/
			char * GetName()
			{
				return Name;
			}

			void SetName (char *str)
			{
				strcpy(Name, str);
			}

			Character_Settings& GetSettings (void)
			{
				return Settings;
			}

			void Instance(const Character& src);
			void Remove(Primitive *p,Collision_Data *c);
			RECT GetSourceRect (void){return SourceRect;}
			void SetSourceRect (RECT rect){SourceRect=rect;}
			RECT GetDestinationRect (void){return DestinationRect;}
			void SetDestinationRect (RECT rect){DestinationRect=rect;}
};

/*Function Prototype*/
void DestroyCharacterList(HWND hwnd);