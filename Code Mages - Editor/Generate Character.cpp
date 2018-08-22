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

/*Character class*/
class Character
{
	private:
			char Name[50];
			Primitive *Primitives;
			Collision_Data *Collisions;
			int Primitives_Num,Collisions_Num;
			Character_Settings Settings;
	public:
			/*Constructor*/
			Character():Primitives(0),Collisions(0),Primitives_Num(0),Collisions_Num(0)	{memset(&Settings,0,sizeof(Settings));Settings.Scale=1;}
			Character(char *Name,Primitive *p,Collision_Data *c,int P_Num,int C_Num,float Pos_x,float Pos_y,float Scale,float Angle,float Speed,float Angular_Speed,float Direction_x,float Direction_y)
			{
				/*Set All Character Data*/
				this->SetName(Name);
				Primitives=p;Collisions=c;
				Primitives_Num=P_Num;Collisions_Num=C_Num;
				/*Set Character Settings*/
				Settings.Position.x=Pos_x;Settings.Position.y=Pos_y;
				Settings.Scale=Scale;
				Settings.Angle=Angle;
				Settings.Speed=Speed;
				Settings.Angular_Speed=Angular_Speed;
				Settings.Vector_Direction.x=Direction_x;Settings.Vector_Direction.y=Direction_y;
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
			/*Get reference to character Settings*/
			Character_Settings& GetSettings(void){return Settings;}
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

			/*Function to Draw the Character*/
			void Draw();
			/*Function to Check if two Characters collide*/
			void CheckCollision(const Character &CheckWith);

			/*Function to return the name*/
			char* GetName(){return Name;}
			void SetName (char *str){strcpy(Name, str);}

			/*Function to create a copy of character*/
			void Instance(const Character& src);
};