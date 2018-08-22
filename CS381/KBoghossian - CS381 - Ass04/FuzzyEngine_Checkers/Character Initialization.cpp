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

	//	/*Initializing "TicTacMap" and Adding it to the linked list*/
	//New=new Character("TicTacMap",TicTacMap_Primitives,TicTacMap_Collisions,4,0,0,0,1,0,0,0,0,0,0);
	//Add_Character_ToList(&head,&tail,New);
	
	/*Initializing "CheckersBoard" and Adding it to the linked list*/
	New=new Character("CheckersBoard",CheckersBoard_Primitives,CheckersBoard_Collisions,18,0,0,0,1,0,0,0,0,0,0);
	Add_Character_ToList(&head,&tail,New);

	return head;
}

Character *Init_Menu (void)
{
	/*Character List is a Two-Way Linked list*/			
	Character *head=NULL,*tail=NULL;			
	Character *New;

	/*Initializing "Menu" and Adding it to the linked list*/
	New=new Character("Menu",Title_Primitives,Title_Collisions,15,0,-20,0,1,0,0,0,0,0,0);
	Add_Character_ToList(&head,&tail,New);

	/*Initializing "Menu" and Adding it to the linked list*/
	New=new Character("Menu",Menu_Primitives,Menu_Collisions,111,0,-20,-70,1,0,0,0,0,0,0);
	Add_Character_ToList(&head,&tail,New);

	return head;
}

Character *Init_Credits(void)
{
	/*Character List is a Two-Way Linked list*/			
	Character *head=NULL,*tail=NULL;			
	Character *New;

	///*Initializing "Credits_Karl" and Adding it to the linked list*/
	//New=new Character("Credits_Karl",Credits_Karl_Primitives,Credits_Karl_Collisions,51,0,-20,-50,1,0,0,0,0,0,0);
	//Add_Character_ToList(&head,&tail,New);

	///*Initializing "Credits_Elie" and Adding it to the linked list*/
	//New=new Character("Credits_Elie",Credits_Elie_Primitives,Credits_Elie_Collisions,48,0,-60,-10,0.63f,-45,0,0,0,0,0);
	//Add_Character_ToList(&head,&tail,New);

	//	/*Initializing "Credits_Ziad" and Adding it to the linked list*/
	//New=new Character("Credits_Ziad",Credits_Ziad_Primitives,Credits_Ziad_Collisions,28,0,60,0,0.9f,45,0,0,0,0,0);
	//Add_Character_ToList(&head,&tail,New);

	/*Initializing "Credits_Karl" and Adding it to the linked list*/
	New=new Character("Credits_Karl",Credits_Karl_Primitives,Credits_Karl_Collisions,51,0,-20,-80,1,0,0,0,0,0,0);
	Add_Character_ToList(&head,&tail,New);

	/*Initializing "Credits_Elie" and Adding it to the linked list*/
	New=new Character("Credits_Elie",Credits_Elie_Primitives,Credits_Elie_Collisions,48,0,-60,-10,0.63f,-40*3.14f/180,0,0,0,0,0);
	Add_Character_ToList(&head,&tail,New);

		/*Initializing "Credits_Ziad" and Adding it to the linked list*/
	New=new Character("Credits_Ziad",Credits_Ziad_Primitives,Credits_Ziad_Collisions,28,0,80,30,0.9f,45,0,0,0,0,0);
	Add_Character_ToList(&head,&tail,New);

	return head;
}