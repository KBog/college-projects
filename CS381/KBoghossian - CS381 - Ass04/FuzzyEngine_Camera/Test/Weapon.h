#ifndef _WEAPON_H_
#define _WEAPON_H_

#include "Object.h"
#include <Application.h>

enum Weapon_Type { MachineGun_Type, RocketLauncher_Type, AtomBombLauncher_Type };
enum Weapon_Ammo_Type { Bullet_Type, Rocket_Type, AtomBomb_Type };

/* Weapon_Ammo Class */
class Weapon_Ammo : public Object
{
	public:
			/* Damage of the weapon */
			int Damage;
			/* Weapon's Range */
			int Range;
			/* Weapon's Translational Speed */
			float Speed;
			/* Type of the ammo */
			Weapon_Ammo_Type Type;
			/* next & prev pointers */
			Weapon_Ammo *next, *prev;

			/* Weapon_Ammo Constructor */
			Weapon_Ammo(string Render_Poly, string Collision_Poly, int type, int damage, int range, float speed, Point3D position=Point3D(0,0,0), BodyCoordinates *bodycoordinates=new BodyCoordinates, Vector3D orientation=Vector3D(0,0,0), Vector3D rotational_speed=Vector3D(1,1,1),Point3D scale=Point3D(1,1,1)):
			Type((Weapon_Ammo_Type)type), Damage(damage), Range(range), Speed(speed),
			Object(Render_Poly,Collision_Poly,position,bodycoordinates,orientation,rotational_speed,scale)
			{
				next=prev=NULL;
			}
			virtual void Update_AI (void) {}
			virtual void Update_Effect (void) {}
			virtual ~Weapon_Ammo ()
			{
			}
};

/* Weapon class */
class Weapon : public Object
{
	public:
			/* Number of bullets - N.B: if number==-1 --> infinity of ammo */
			int Ammo;
			/* Delay */
			int Delay;
			/* Weapon Type */
			Weapon_Type Type;
			/* A pointer to the sample of bullet or rocket ... to fire */
			Weapon_Ammo * Weapon_AmmoType;
			/* Weapon's next & prev pointer */
			Weapon *next, *prev;

			/* Weapon constructor */
			Weapon(string Render_Poly, string Collision_Poly, int type, Weapon_Ammo *weapon_ammotype, int ammo, int delay, Point3D position, BodyCoordinates *bodycoordinates, Vector3D orientation, Vector3D rotational_speed, Point3D scale) :
			Type((Weapon_Type)type), Weapon_AmmoType(weapon_ammotype), Ammo(ammo), Delay(delay),
			Object(Render_Poly,Collision_Poly,position,bodycoordinates,orientation,rotational_speed,scale)
			{
				next=prev=NULL;
			}
			/* Update the Weapon's ammo, ... */
			void Update_WeaponStatus (void);
			/* Function to update the weapon's behavior to do an effect to change the weapon or to look at the target */
			virtual void Update_AI (void) {}
			/* Weapon's Destructor */
			~Weapon () 
			{
				delete Weapon_AmmoType;
			}
};

#endif