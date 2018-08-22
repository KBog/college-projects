#ifndef _OBJECT_CHILD_H_
#define _OBJECT_CHILD_H_

#include "Object.h"

/* Object_Child Class */
class Object_Child : public Object
{
	public:
			/* A pointer to hold its parent */
			Object *Parent;

			/* Object_Part constructor */
			Object_Child (string s, Point3D position, BodyCoordinates *bodycoordinates, Vector3D orientation, Vector3D rotational_speed, Point3D scale) :
			Object(s,position,bodycoordinates,orientation,rotational_speed,scale)
			{
				Parent=NULL;
			}

			virtual void Update_AI (void) 
			{
			}
			virtual void Update_Effect (void)
			{
			}
			virtual ~Object_Child ()
			{
			}
};

#endif