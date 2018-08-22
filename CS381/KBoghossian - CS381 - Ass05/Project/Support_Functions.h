#ifndef _SUPPORTFUNCTIONS_H_
#define _SUPPORTFUNCTIONS_H_

#include "Standard Header.h"
#include <math.h>
#define EPSILON 0.0005f

enum CLASSIFY 
{
	INTERSECT,
	NO_INTERSECT,
	OUTSIDE,
	INSIDE,
	LEFT,
	RIGHT,
	BEHIND,
	BEYOND,
	FRONT,
	BACK,
	INTERSECT_ORIGIN,
	INTERSECT_A,
	INTERSECT_B,
};

/* Function to Add anything at Runtime */
template <class T>
void Add_ToList (T **head, T *Part)
{
	if(!*head)			
	{			
		*head=Part;			
		return;			
	}
	for(T *tmp=*head; tmp->next; tmp=tmp->next);
	tmp->next=Part;
	Part->prev=tmp;
}

/* Function to remove a character at run-time */
template <class T>
T *Remove_FromList (T **head, T *Current)
{
	if(Current!=NULL)
	{
		/* If its position is the first
		and the last one (list only
		contains one bullet) */
		if(Current == *head && Current->next==0)
		{
			Current->prev = Current->next = 0;
			*head = 0;
			return Current;
		}
		/* If its position is the first
		one */
		else if(Current == *head)
		{
			T *Temp = Current;
			Current->next->prev = 0;
			*head = Temp->next;
			Current->next=0;
			return Temp;
		}
		/* Else if it's in the middle */
		else
		{
			Current->prev->next = Current->next;
			if(Current->next)
				Current->next->prev = Current->prev;
		}
	}
	return Current;
}

/* Function to return the ANYTHING at any index */
template <class T>
T* Return_AtIndex (T **head, int index)
{
	int i=0;
	for(T *tmp=*head; i<index; tmp=tmp->next, i++);
	return tmp;
}

/* Function to delete the link list */
template <class T>
void Delete_AllList (T **head)
{
	T *Next=NULL;
	/* Looping through the list of objects in the current area of the current level */
	for(T *tmp=*head; tmp; tmp=Next)
	{
		Next=tmp->next;
		delete Remove_FromList(head, tmp);
	}
	delete *head;
	head=NULL;
}

CLASSIFY Classify_PointToLineSegment(Vertex point, Vertex LineSeg_V0, Vertex LineSeg_V1);
bool PointOnLineSegment(Vertex point, Vertex LineSeg_V0, Vertex LineSeg_V1);
void Get_SegmentToSegment_Point (Vertex L1_a, Vertex L1_b, Vertex L2_a, Vertex L2_b, Vertex &PtInter);

inline bool Equal_Points (Vertex p1, Vertex p2)
{
	return (fabsf(p1.x-p2.x)<EPSILON && fabsf(p1.y-p2.y)<EPSILON);
}

inline void Normalize_Vector (Vector &V)
{
	float Magnitude = sqrtf(V.x*V.x + V.y*V.y);
	if(Magnitude)
	{
		V.x/=Magnitude;
		V.y/=Magnitude;
	}
}

#endif