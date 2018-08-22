#ifndef _ITEM_H_
#define _ITEM_H_

#include "Object.h"

/* Item class */
class Item: public Object
{
	public:
			/* A pointer to a next and prev item */
			Item *next, *prev;

			/* Constructor */
			Item(string Render_Poly, string Collision_Poly, Point3D position=Point3D(0,0,0), BodyCoordinates *bodycoordinates=new BodyCoordinates, Vector3D orientation=Vector3D(0,0,0), Vector3D rotational_speed=Vector3D(1,1,1), Point3D scale=Point3D(1,1,1)):
			Object(Render_Poly,Collision_Poly,position,bodycoordinates,orientation,rotational_speed,scale)
			{
				next=prev=NULL;
			}
			/* Initialize the item according to the updated variables from the area */
			virtual void Init_Item (void) {}
			/* Function that allow the Special Effect of a certain Itema at each game loop */
			virtual void Update_AI (void){}
			/* Function that allow the Special Effect when the main character takes it */
			virtual void BehaviorWhenTaken(void){}
			/* Virtual Destructor */
			~Item ()
			{
			}
};

#endif