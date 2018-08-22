#include "Weapon.h"
#include "Rocket.h"
#include "Bullet.h"
#include "Game.h"

extern Game *Game_Engine;

void Weapon::Update_WeaponStatus (void)
{
	/* Updating the available ammunitions */
	if(Ammo>0)
	{	
		switch(Weapon_AmmoType->Type)
		{
			case Rocket_Type:
				Add_ToList(&Game_Engine->Current_WeaponsAmmoList, (Weapon_Ammo *)new Rocket("NO_3DS","NO_3DS",Rocket_Type, Weapon_AmmoType->Damage,Weapon_AmmoType->Range,Weapon_AmmoType->Speed, NULL, Position, Body_Coordinates, Orientation, Scale));
				break;
		}
		Ammo--;
	}
	else if(Ammo==-1)
	{
		switch(Weapon_AmmoType->Type)
		{
			case Bullet_Type:
				Add_ToList(&Game_Engine->Current_WeaponsAmmoList, (Weapon_Ammo *)new Bullet("NO_3DS","NO_3DS",Bullet_Type, Weapon_AmmoType->Damage,Weapon_AmmoType->Range,Weapon_AmmoType->Speed, Position, Body_Coordinates, Orientation, Scale));
				break;
		}
	}
}