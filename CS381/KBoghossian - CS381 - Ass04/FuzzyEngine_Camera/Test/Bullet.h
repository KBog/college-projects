#ifndef _BULLET_H_
#define _BULLET_H_

#include "Weapon.h"

/* Rocket class */
class Bullet : public Weapon_Ammo
{
	public:
			//Particle System
			
			/* Bullet Constructor */
			Bullet(string Render_Poly, string Collision_Poly, int type, int damage, int range, float speed, Point3D position=Point3D(0,0,0), BodyCoordinates *bodycoordinates=new BodyCoordinates, Vector3D orientation=Vector3D(0,0,0), Point3D scale=Point3D(1,1,1)) :
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
				}
				//else explode
			}
			void Update_Effect (void)
			{
			}
			~Bullet()
			{
			}
};

#endif