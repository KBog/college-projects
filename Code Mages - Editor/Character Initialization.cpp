#include "Standard Header.h"
#include "Design.h"

void Add_Character_ToList(Character **head,Character **tail,Character *New)			
{			
	if(!*head)			
	{			
		*head=*tail=New;			
		return;			
	}			
	*tail->next=New;			
	New->prev=*tail;			
	*tail=New;			
}

Character *Init_Characters(void)
{
	/*Character List is a Two-Way Linked list*/			
	Character *head=NULL,*tail=NULL;			
	Character *New;

	/*Initializing "Character 1" and Adding it to the linked list*/
	New=new Character("Character_1",Character_1_Primitives,Character_1_Collisions,4,0,0,0,1,0,0,0,0,0);
Add_Character_ToList(&head,&tail,New);

	/*Initializing "Character 2" and Adding it to the linked list*/
	New=new Character("Character_2",Character_2_Primitives,Character_2_Collisions,1,0,0,0,1,0,0,0,0,0);
Add_Character_ToList(&head,&tail,New);

}
