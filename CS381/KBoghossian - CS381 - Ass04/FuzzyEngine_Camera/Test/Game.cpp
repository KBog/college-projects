#include "Level.h"
#include "Game.h"
#include "Camera.h"

extern Camera *Main_Camera;

Level *Create_Menu (void)
{
	Level *l = new Level(Level_Name(0), Init_Menu());
	return l;
}

Level *Create_L1 (void)
{
	Level *l = new Level(Level_Name(1), Init_L1());
	return l;
}

Level *Create_L2 (void)
{
	Level *l = new Level(Level_Name(2), Init_L2());
	return l;
}

Level *Create_L3 (void)
{
	Level *l = new Level(Level_Name(3), Init_L3());
	return l;
}

/* A function to move to a next level */
void Game::Next_Level (void)
{
	/* Check the name of the current level */
	switch(Current_Level->Name)
	{
		/* if we are in the first level */
		case L1:
			/* Unload the current level and reload the next level */
			Unload_Level();
			Current_Level = Create_L2();
			break;

		case L2:
			/* Unload the current level and reload the next level */
			Unload_Level();
			Current_Level = Create_L3();
			break;
	}
}


/* A function to restart the current level */
void Game::Restart_Level (void)
{
	/* Check the name of the current level */
	switch(Current_Level->Name)
	{
		/* if we are in the first level */
		case L1:
			/* Unload the current level and reload the next level */
			Unload_Level();
			Current_Level = Create_L1();
			break;

		case L2:
			/* Unload the current level and reload the next level */
			Unload_Level();
			Current_Level = Create_L2();
			break;

		case L3:
			/* Unload the current level and reload the next level */
			Unload_Level();
			Current_Level = Create_L3();
			break;
	}
}

/* A function to update the game engine */
void Game::Update (bool update_game)
{
	Main_Camera->Update();
	/* If the game must be updated --> Call the update
		function inside the current level */
	if(update_game)
	{
		/* Update the time inside the game */
		Game_Time->Update_Time();
		/* Update the main character behavior */
		GliCycle->Update_Object();
		
		//if(NetIsOn)		
		//	GliCycle2->Update_Object();
		//Keyboard_Boss->Update_Managers();
		/* Update the areas with everything in them */
		Current_Level->Update();
		/* Update the current weapons in the area */
		for(Weapon_Ammo *tmp=Current_WeaponsAmmoList; tmp; tmp=tmp->next)
		{
			tmp->Update_AI();
			tmp->Update_Effect();
		}
	}
}

/* Function to render the list in the current area */
void Game::Render (void)
{
	for(Area *tmp_area = Current_Level->Current_Area; tmp_area; tmp_area=tmp_area->next)
	{
		tmp_area->Render_Area();
		if(TEST_ENGINE)
			tmp_area->Render_Test();
	}
	if(NetIsOn)
	{
		//renderer->SetWorldTransformation(GliCycle2->Final_ObjectTransformation);
		GliCycle2->Draw_MainCharacter();
	}
	GliCycle->Draw_MainCharacter();
	//Keyboard_Boss->Render_Managers();
}