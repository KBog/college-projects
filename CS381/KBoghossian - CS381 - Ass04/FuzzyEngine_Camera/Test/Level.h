#ifndef _LEVEL_H_
#define _LEVEL_H_

#include "Area.h"

Area *Init_Menu (void);
Area *Init_L1 (void);
Area *Init_L2 (void);
Area *Init_L3 (void);

/* Level Names */
enum Level_Name { Menu, L1, L2, L3 };

/* Level Class */
class Level
{
	public:
			/* To hold the level name for the Game Class */
			Level_Name Name;
			/* List of Areas */
			Area *Area_List;
			/* Current Area */
			Area *Current_Area;
			/* Pointer to the previous and next node */
			Level *next, *prev;

			/* Level constructor */
			Level (int name, Area *area_list) :
			Name((Level_Name)name), Area_List(area_list)
			{ 			
				Current_Area=new Area(area_list);
				next=prev=NULL;
			}

			/* Function to update the current area */
			void Update (void);

			/* A function to move the current area pointer to the next area */
			void Next_Area (void);

			/* A function to remove a previous area from the list */
			void Remove_PrevArea (void);

			/* Destructor to loop through the list of areas to unload them */
			~ Level()
			{
				Delete_AllList(&Area_List);
			}
};


#endif