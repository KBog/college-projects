#include "Standard Header.h"
#include "Design.h"

void Add_Character_ToList(Character **head,Character **tail,Character *New)			
{			
	if(!*head)			
	{			
		*head=*tail=New;			
		return;			
	}			
	(*tail)->next=New;			
	New->prev=*tail;			
	*tail=New;			
}

Character *Init_NodeShape(void)
{
	/*Character List is a Two-Way Linked list*/			
	Character *head=NULL,*tail=NULL;			
	Character *New;

	/*Initializing "Node_Shape" and Adding it to the linked list*/
	New=new Character("Node_Shape",Node_Shape_Primitives,Node_Shape_Collisions,4,0,0,0,0.1f,0,0,0,0,0,0);
	Add_Character_ToList(&head,&tail,New);

	return head;
}

Character *Init_Characters(void)
{
	/*Character List is a Two-Way Linked list*/			
	Character *head=NULL,*tail=NULL;			
	Character *New;

	/*Initializing "MainCharacter" and Adding it to the linked list*/
	New=new Character("MainCharacter",MainCharacter_Primitives,MainCharacter_Collisions,39,0,0,0,0.10f,0,0,0.01f,0,0,0);
	Add_Character_ToList(&head,&tail,New);

	return head;
}

Character *Init_Map (void)
{
	/*Character List is a Two-Way Linked list*/			
	Character *head=NULL,*tail=NULL;			
	Character *New;

	/*Initializing "Final0" and Adding it to the linked list*/
	//New=new Character("Final0",Final0_Primitives,Final0_Collisions,0,21,0,0,1,0,0,0,0,0,349.813);
	//Add_Character_ToList(&head,&tail,New);
	/*Initializing "Map" and Adding it to the linked list*/
	New=new Character("Map",Map_Primitives,Map_Collisions,0,16,0,0,1,0,0,0,0,0,162.25);
	Add_Character_ToList(&head,&tail,New);

	
	/*Initializing "Solid_Small_2" and Adding it to the linked list*/
	//New=new Character("Solid_Small_2",Solid_Small_2_Primitives,Solid_Small_2_Collisions,0,5,0,0,1,0,0,0,0,0,241.696f);
	//Add_Character_ToList(&head,&tail,New);

	return head;
}
