#ifndef _ITEM_SLOWMOTION_H_
#define _ITEM_SLOWMOTION_H_

#include "Item.h"
#include "Weapon.h"

extern MainCharacter * GliCycle;

/* Item Ammo class */
class Item_MainCharacter_SlowMotion: public Item
{
	public:
			/* Variabl that sets the current item Health Capacity */
			int Time;
	
			/* Constructor */
			Item_MainCharacter_SlowMotion(string Render_Poly, string Collision_Poly, int time, Point3D position=Point3D(0,0,0), BodyCoordinates *bodycoordinates=new BodyCoordinates, Vector3D orientation=Vector3D(0,0,0), Vector3D rotational_speed=Vector3D(1,1,1),Point3D scale=Point3D(1,1,1)):
			Time(time),
			Item(Render_Poly,Collision_Poly,position,bodycoordinates,orientation,rotational_speed,scale)
			{
			}
			/* Initialize the items according to the updated ones from the area */
			void Init_Item (void) 
			{
			}
			/* Function that allow the Special Effect of a certain Items at each game loop */
			void Update_AI (void)
			{
				Body_Coordinates->BodyCoordinates_Orientation = Matrix3D::GetRotateArbitraryAxis(Body_Coordinates->y,-Rotational_Speed.Y*Slow_Motion) * Matrix3D::GetRotateArbitraryAxis(Body_Coordinates->x,-Rotational_Speed.X*Slow_Motion) * Matrix3D::GetRotateArbitraryAxis(Body_Coordinates->z,-Rotational_Speed.Z*Slow_Motion) * Body_Coordinates->BodyCoordinates_Orientation;

				Update_ObjectBodyCoordinates();
				Update_ObjectMatrix(Local_ObjectTransformation);
				Update_ObjectBoundingBox(Local_ObjectTransformation,Parent->Parent_ObjectMatrix);
			}
			/* Function that allow the Special Effect when the main character takes it 
			   and behaving with the character's weapon's according to its type */
			void BehaviorWhenTaken(void)
			{
			}
			/* Item Ammo destructor */
			~Item_MainCharacter_SlowMotion()
			{
			}
};

#endif