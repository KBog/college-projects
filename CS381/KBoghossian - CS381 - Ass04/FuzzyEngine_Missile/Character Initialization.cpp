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


Character *Init_Missile(void)
{
	/*Character List is a Two-Way Linked list*/			
	Character *head=NULL,*tail=NULL;			
	Character *New;

	/*Initializing "Cic" and Adding it to the linked list*/
	New=new Character("White",White_Primitives,White_Collisions,3,0,-150,0,1,0,1.5f,0,1,0,0);
	Add_Character_ToList(&head,&tail,New);

	return head;
}

Character *Init_Target(void)
{
	/*Character List is a Two-Way Linked list*/			
	Character *head=NULL,*tail=NULL;			
	Character *New;

	/*Initializing "Cic" and Adding it to the linked list*/
	New=new Character("White",Black_Primitives,Black_Collisions,1,0,0,0,1,0,1.5f,0,0,0,0);
	Add_Character_ToList(&head,&tail,New);

	return head;
}

Character *Init_Map (void)
{
	/*Character List is a Two-Way Linked list*/			
	Character *head=NULL,*tail=NULL;			
	Character *New;

	/*Initializing "Cic" and Adding it to the linked list*/
	New=new Character("Map",Map_Primitives,Map_Collisions,8,0,0,0,1,0,0,0,0,0,0);
	Add_Character_ToList(&head,&tail,New);

	return head;
}