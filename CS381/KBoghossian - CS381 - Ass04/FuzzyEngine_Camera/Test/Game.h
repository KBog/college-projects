#ifndef _GAME_H_
#define _GAME_H_


#include "Level.h"
#include "Weapon.h"
#include "MainCharacter.h"
#include "KeyboardBoss.h"
#include "GTime.h"

extern KeyboardBoss *Keyboard_Boss;
extern GameTime *Game_Time;
extern MainCharacter *GliCycle, *GliCycle2;
extern bool NetIsOn;


Level *Create_L1 (void);
Level *Create_L2 (void);
Level *Create_L3 (void);
Level *Create_Menu (void);

/* Game class */
class Game
{
	public:
			/* A pointer to the current level to be updated and rendered */
			Level *Current_Level;
			/* A pointer to the current bullets to be updated */
			Weapon_Ammo *Current_WeaponsAmmoList;

			/* Game Constructor */
			Game (Level *initial_level) : 
			Current_Level(initial_level), Current_WeaponsAmmoList(NULL)
			{
			}
	
			/* Function to delete all the areas and its members inside the Current Level */
			void Unload_Level (void)
			{
				delete Current_Level;
				Current_Level=NULL;
			}

			/* A function to move to a next level */
			void Next_Level (void);

			/* A function to restart the current level */
			void Restart_Level (void);

			/* A function to restart the game by unloading the current level and loading
			   the menu */
			void Restart_Game (void)
			{
				Unload_Level();
				Current_Level = Create_Menu ();
			}

			/* A function to update the game engine */
			void Update (bool update_game);

			/* Function to render the list in the current area */
			void Render (void);

			~ Game()
			{
				delete Current_Level;
			}

};

#endif