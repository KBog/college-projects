#ifndef _ALARM_DOOR_H_
#define _ALARM_DOOR_H_

#include "Alarm.h"
#include "Game.h"

extern Game *Game_Engine;

/* Alarm door class */
class Alarm_Door : public Alarm
{
	public:
			/* The object to behave at collision */
			Object *Door;

			/* Alarm Door constructor */
			Alarm_Door(string Render_Poly, string Collision_Poly, Object *door, bool render, bool istriggered, Vector3D normal, Point3D position=Point3D(0,0,0), BodyCoordinates *bodycoordinates=new BodyCoordinates, Vector3D orientation=Vector3D(0,0,0), Point3D scale=Point3D(1,1,1)) :
			Door(door),
			Alarm(Render_Poly,Collision_Poly,render,istriggered,normal,position,bodycoordinates,orientation,scale)
			{
			}
			/* Behavior when the main character collides with it */
			void BehaviorAtCollision (void)
			{
				//close the door or open it ...
				Game_Engine->Current_Level->Next_Area();
			}
};

#endif