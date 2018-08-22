#ifndef _ITEM_AMMO_H_
#define _ITEM_AMMO_H_

#include "Item.h"
#include "Weapon.h"

extern MainCharacter * GliCycle;

/* Item Ammo class */
class Item_MainCharacter_Ammo: public Item
{
	public:
			/* Variabl that sets the current item Health Capacity */
			int Ammo_Refill;
			/* Type of the ammo */
			Weapon_Ammo_Type Type;
			/* Source and Destination Positions */
			float Source_Position, Destination_Position;
			/* Distance from the Source to the Destination */
			float Offset_Distance;
			float Displacement_Speed;
			bool Up;
	
			/* Constructor */
			Item_MainCharacter_Ammo(string Render_Poly, string Collision_Poly, int type, int ammo_refill, float offset_distance, float displacement_speed, Point3D position=Point3D(0,0,0), BodyCoordinates *bodycoordinates=new BodyCoordinates, Vector3D orientation=Vector3D(0,0,0), Vector3D rotational_speed=Vector3D(1,1,1),Point3D scale=Point3D(1,1,1)):
			Type((Weapon_Ammo_Type)type),Ammo_Refill(ammo_refill),Offset_Distance(offset_distance),Displacement_Speed(displacement_speed),
			Item(Render_Poly,Collision_Poly,position,bodycoordinates,orientation,rotational_speed,scale)
			{
				Up=true;
			}
			/* Initialize the items according to the updated ones from the area */
			void Init_Item (void) 
			{
				Source_Position = Position.Y;
				Destination_Position = Position.Y + Offset_Distance;
			}
			/* Function that allow the Special Effect of a certain Items at each game loop */
			void Update_AI (void)
			{
				if(Position.Y<Destination_Position && Up)
				{
					Position.Y+=Displacement_Speed*Slow_Motion;
					if(Position.Y>=Destination_Position)
						Up=false;
				}
				else if(Position.Y>=Source_Position && !Up)
				{
					Position.Y-=Displacement_Speed*Slow_Motion;
					if(Position.Y<=Source_Position)
						Up=true;
				}

				Body_Coordinates->BodyCoordinates_Orientation = Matrix3D::GetRotateArbitraryAxis(Body_Coordinates->y,-Rotational_Speed.Y*Slow_Motion) * Body_Coordinates->BodyCoordinates_Orientation;

				Update_ObjectBodyCoordinates();
				Update_ObjectMatrix(Local_ObjectTransformation);
				Update_ObjectBoundingBox(Local_ObjectTransformation,Parent->Parent_ObjectMatrix);
			}
			/* Function that allow the Special Effect when the main character takes it 
			   and behaving with the character's weapon's according to its type */
			void BehaviorWhenTaken(void)
			{
				switch(Type)
				{
					case Bullet_Type:
						GliCycle->Weapon_List[MachineGun_Type]->Ammo+=Ammo_Refill;
						break;
					case Rocket_Type:
						GliCycle->Weapon_List[RocketLauncher_Type]->Ammo+=Ammo_Refill;
						break;
				}
			}
			/* Item Ammo destructor */
			~Item_MainCharacter_Ammo()
			{
			}
};

#endif