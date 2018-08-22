#ifndef _ALARM_ENDLEVEL_H_
#define _ALARM_ENDLEVEL_H_

#include "Alarm.h"

extern Game *Game_Engine;

/* Alarm door class */
class Alarm_EndLevel : public Alarm
{
	public:
			/* Alarm Door constructor */
			Alarm_EndLevel(string s, bool render, bool istriggered, Vector3D normal, Point3D position=Point3D(0,0,0), Vector3D direction=Vector3D(0,0,0), Vector3D orientation=Vector3D(0,0,0), Point3D scale=Point3D(1,1,1)) :
			Alarm(s,render,istriggered,normal,position,direction,orientation,scale)
			{
			}
			/* Behavior when the main character collides with it */
			void BehaviorAtCollision (void)
			{
				//close the door or open it ...
				Game_Engine->Next_Level();
			}
};

#endif