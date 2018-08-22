#include "MainCharacter.h"
#include "Game.h"
#include "time.h"

Game *Game_Engine = new Game(Create_L1());
MainCharacter GliCycle(3,100,Point3D(0,0,0),10,NULL,NULL,Point3D(0,0,0),Vector3D(0,0,0),Vector3D(0,0,0),Point3D(1,1,1));

void main (void)
{
	int i=0;
	srand((unsigned int)time(NULL));
	//Weapon *W = new Weapon(RocketLauncher_Type, new Weapon_Ammo(Rocket_Type,50,50,50,50, Point3D(0,0,0), Vector3D(0,0,0), Vector3D(0,0,-1), Point3D(1,1,1)), 100, Point3D(0,0,0), Vector3D(0,0,0), Vector3D(0,0,-1), Point3D(1,1,1));
	//W->Update_WeaponStatus();
	//W->Update_WeaponStatus();
	//Game_Engine->Update(1);
	Game_Engine->Next_Level();
	GliCycle.Weapon_List[MachineGun_Type]->Update_WeaponStatus();

	//ziz. 

	//G->Current_Level->Next_Area();
	//G->Current_Level->Remove_PrevArea();
	//G->Next_Level();
	//ea(G-.
	//Level *Head = new Level(0);
	//Add_ToList(&Head, new Level(1));
	//Level *AT = Return_AtIndex(&Head, 1);
	//delete Remove_FromList(&Head, Return_AtIndex(&Head,0));
}