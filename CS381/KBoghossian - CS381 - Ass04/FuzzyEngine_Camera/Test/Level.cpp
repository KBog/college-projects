#include "Object.h"
#include "Level.h"
#include "Area.h"
#include "Game.h"


Area *Init_Menu (void)
{
	/*Character List is a Two-Way Linked list*/			
	Area *head=NULL;			
	
	return head;
}

/* Add all the areas in the level */
Area *Init_L1 (void)
{
	/*Character List is a Two-Way Linked list*/			
	Area *head=NULL;			

	Add_ToList(&head, new Area(app.GetDirectory() + "Resources\\BSP_Test8.3DS", app.GetDirectory() + "Resources\\BSP_Test8.3DS", 0, Point3D(0,0,-0),new BodyCoordinates,Vector3D(0,0,0),Vector3D(0,0,0),Point3D(1,1,1),4,4,Init_Objects_L1_A1(), Init_Items_L1_A1(), Init_Alarms_L1_A1()));
	//Add_ToList(&head, new Area(app.GetDirectory() + "Resources\\AreaTest.3DS", 1, Point3D(12.5f,0,-7.5f),new BodyCoordinates,Vector3D(0,0,0),Vector3D(1,1,1),Point3D(3,3,3),Init_Objects_L1_A2()));
	//Add_ToList(&head, new Area(2, Init_Objects_L1_A2()));
	
	return head;
}

/* Add all the areas in the level */
Area *Init_L2 (void)
{
	/*Character List is a Two-Way Linked list*/			
	Area *head=NULL;			

	//Add_ToList(&head, new Area(0, Init_Objects_L1_A1()));
	//Add_ToList(&head, new Area(1, Init_Objects_L1_A2()));
	//Add_ToList(&head, new Area(2, Init_Objects_L1_A2()));
	
	return head;
}

/* Add all the areas in the level */
Area *Init_L3 (void)
{
	/*Character List is a Two-Way Linked list*/			
	Area *head=NULL;			

	//Add_ToList(&head, new Area(0, Init_Objects_L1_A1()));
	//Add_ToList(&head, new Area(1, Init_Objects_L1_A2()));
	//Add_ToList(&head, new Area(2, Init_Objects_L1_A2()));
	
	return head;
}


/* Function to update the current area */
void Level::Update (void)
{
	/* Looping through all the areas in the current area pointer to update them */
	for(Area *tmp_area=Current_Area; tmp_area; tmp_area=tmp_area->next)
		tmp_area->Update();
}
/* A function to move the current area pointer to the next area */
void Level::Next_Area (void)
{
	switch(Current_Area->Name)
	{
		/* Adding an area for rendering to the current area pointer */
		case A1:
				Add_ToList(&Current_Area, new Area(Return_AtIndex(&Area_List, A2)));
				break;
		/* Adding an area for rendering to the current area pointer */
		case A2:
				Add_ToList(&Current_Area, new Area(Return_AtIndex(&Area_List, A3)));
				break;
	}
}
/* A function to remove a previous area from the list */
void Level::Remove_PrevArea (void)
{
	switch(Current_Area->Name)
	{
		case A1:
				/* Deleting the previous area from the list of the current areas and it
				will automatically be set to the next one */
				delete Remove_FromList(&Current_Area, Return_AtIndex(&Current_Area, A1));
				break;
		case A2:
				/* Deleting the previous area from the list of the current areas and it
				will automatically be set to the next one */
				delete Remove_FromList(&Current_Area, Return_AtIndex(&Current_Area, A2));
				break;
	}
}