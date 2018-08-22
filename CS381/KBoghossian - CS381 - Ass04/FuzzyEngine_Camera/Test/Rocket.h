#ifndef _ROCKET_H_
#define _ROCKET_H_

#include "Weapon.h"

/* Rocket class */
class Rocket : public Weapon_Ammo
{
	public:
			/* The target to follow */
			Object *Target;
			//Particle System
			
			/* Rocket Constructor */
			Rocket(string Render_Poly, string Collision_Poly, int type, int damage, int range, float speed, Object *target, Point3D position=Point3D(0,0,0), BodyCoordinates *bodycoordinates=new BodyCoordinates, Vector3D orientation=Vector3D(0,0,0), Point3D scale=Point3D(1,1,1)) :
			Target(target),
			Weapon_Ammo(Render_Poly,Collision_Poly,type,damage,range,speed,position,bodycoordinates,orientation,scale)
			{
			}
			/* Weapon's Behavior must be called if its in the list of 
			   Currently_updating in the main character*/
			void Update_AI (void)
			{
				if(Range)
				{
					Range--;
					//follow the target
				}
				//else explode
			}
			void Update_Effect (void)
			{
			}
			~Rocket()
			{
				delete Target;
			}
};

#endif