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


Character *Init_White(void)
{
	/*Character List is a Two-Way Linked list*/			
	Character *head=NULL,*tail=NULL;			
	Character *New;

	/*Initializing "Cic" and Adding it to the linked list*/
	New=new Character("White",White_Primitives,White_Collisions,1,0,-220,160,1,0,0,0,0,0,0);
	Add_Character_ToList(&head,&tail,New);

	return head;
}

Character *Init_Black(void)
{
	/*Character List is a Two-Way Linked list*/			
	Character *head=NULL,*tail=NULL;			
	Character *New;

	/*Initializing "Cic" and Adding it to the linked list*/
	New=new Character("Black",Black_Primitives,Black_Collisions,1,0,-220,160,1,0,0,0,0,0,0);
	Add_Character_ToList(&head,&tail,New);

	return head;
}

Character *Init_Map (void)
{
	/*Character List is a Two-Way Linked list*/			
	Character *head=NULL,*tail=NULL;			
	Character *New;

	/*Initializing "Board" and Adding it to the linked list*/
	New=new Character("Board",Board_Primitives,Board_Collisions,24,0,0,0,1,0,0,0,0,0,0);
	Add_Character_ToList(&head,&tail,New);

	return head;
}

Character *Init_Square (void)
{
	/*Character List is a Two-Way Linked list*/			
	Character *head=NULL,*tail=NULL;			
	Character *New;

	/*Initializing "Square" and Adding it to the linked list*/
	New=new Character("Square",Square_Primitives,Square_Collisions,2,0,-240,200,1,0,0,0,0,0,0);
	Add_Character_ToList(&head,&tail,New);

	return head;
}