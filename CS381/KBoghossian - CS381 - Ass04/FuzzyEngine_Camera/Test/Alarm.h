#ifndef _ALARM_H_
#define _ALARM_H_

#include "Object.h"
#include <Application.h>

/* Alarm Class */
class Alarm : public Object
{
	public:
			/* To know whether the alarm has been triggered or not */
			bool IsTriggered;
			/* To know whether we must render the plane or not */
			bool Render;
			/* The Shape of the alarm */
			Plane3D *P;
			/* A pointer to the next and prev alarms */
			Alarm *next, *prev;

			/* Alarm constructor */
			Alarm (string Render_Poly, string Collision_Poly, bool render, bool istriggered, Vector3D normal, Point3D position=Point3D(0,0,0), BodyCoordinates *bodycoordinates=new BodyCoordinates, Vector3D orientation=Vector3D(0,0,0), Vector3D rotational_speed=Vector3D(1,1,1), Point3D scale=Point3D(1,1,1)) :
			Render(render), IsTriggered(istriggered),
			Object(Render_Poly,Collision_Poly,position,bodycoordinates,orientation,rotational_speed,scale)
			{
				/* Defining the plane equation */
				P = new Plane3D(position,normal);
				next=prev=NULL;
			}
			/* Alarm Destructor */
			~Alarm ()
			{
				delete P;
			}
			/* Behavior for each alarm at collision */
			virtual void BehaviorAtCollision (void) {}
};

#endif