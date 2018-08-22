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

Character *Init_Characters(void)
{
	/*Character List is a Two-Way Linked list*/			
	Character *head=NULL,*tail=NULL;			
	Character *New;

	return head;
}
