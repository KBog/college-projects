#ifndef _ITEM_CRATE_H_	
#define _ITEM_CRATE_H_

#include "Item.h"

extern MainCharacter * GliCycle;

#define ITEM_RAND_NUMBER 4
enum Item_Rand_Type {IncreaseHealth_Type, DecreaseHealth_Type, IncreaseAmmo_Type, DecreaseAmmo_Type};

/* Item_Crate Class */
class Item_Crate: public Item
{
	public:
			/* Value to use either for increase or decrease health ... */
			int Value;
			/* random type of item */
			Item_Rand_Type Type;

			/* Item Crate constructor */
			Item_Crate(string Render_Poly, string Collision_Poly, int value, Point3D position=Point3D(0,0,0), BodyCoordinates *bodycoordinates=new BodyCoordinates, Vector3D orientation=Vector3D(0,0,0), Vector3D rotational_speed=Vector3D(1,1,1),Point3D scale=Point3D(1,1,1)):
			Value(value),
			Item(Render_Poly,Collision_Poly,position,bodycoordinates,orientation,rotational_speed,scale)
			{
				/* Taking a random between the available number of items */
				Type = (Item_Rand_Type)(rand()%ITEM_RAND_NUMBER);
			}
			/* Function that allow the Special Effect of a certain Itema at each game loop */
			void Behavior(void)
			{
			}
			/* Function that allow the Special Effect when the main character takes it */
			void BehaviorWhenTaken(void)
			{
				switch(Type)
				{
					//Invert the controls

					/* Increase the player's health */
					case IncreaseHealth_Type:
						GliCycle->Health += Value;
						break;

					/* Decrease the player's health */
					case DecreaseHealth_Type:
						GliCycle->Health -= Value;
						break;

					/* Increase all the ammo of all the weapons without the machine gun by a value */
					case IncreaseAmmo_Type:
						for(int i=1; i<MAINCHARACTER_WEAPONS_NUMBER; i++)
							GliCycle->Weapon_List[i]->Ammo += Value;
						break;

					/* Decrease the ammo of all the weapons by a value */
					case DecreaseAmmo_Type:
						for(int i=1; i<MAINCHARACTER_WEAPONS_NUMBER; i++)
						{
							/* if the new value is greater than 0 --> decrement the ammo */
							if((GliCycle->Weapon_List[i]->Ammo-Value)>=0)
								GliCycle->Weapon_List[i]->Ammo -= Value;
							/* else set the ammo to 0 */
							else
								GliCycle->Weapon_List[i]->Ammo=0;
						}
						break;
				}
			}
			/* Item Crate Destructor */
			~Item_Crate()
			{
			}
};

#endif 