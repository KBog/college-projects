#ifndef _SUPPORT_FUNCTIONS_H_
#define _SUPPORT_FUNCTIONS_H_

#include "Defines.h"
#include "Bsp.h"

/* BodyCoordinates class */
class BodyCoordinates
{
	public:
			/* Initial Body Coordinates */
			Vector3D i, j, k;
			/* Updated Body Coordinates */
			Vector3D x, y, z;
			/* Updated World Body Coordinates */
			Vector3D world_x, world_y, world_z;
			/* Vector to hold the Angle X, Y and Z of the axis */
			Vector3D Angles;
			/* Matrix to hold the current rotation around the desired axis */
			Matrix3D BodyCoordinates_Orientation, BodyCoordinates_WorldOrientation;
			
			/* Body Coordinates constructor */
			BodyCoordinates(Vector3D i_, Vector3D j_, Vector3D k_) :
			i(i_), j(j_), k(k_) 
			{
				x = world_x = i;
				y = world_y = j;
				z = world_z = k;
			}
			/* Body Coordinates default constructor */
			BodyCoordinates() 
			{
				i.SetXYZ(-1,0,0);
				j.SetXYZ(0,1,0);
				k.SetXYZ(0,0,-1);
				x = world_x = i;
				y = world_y = j;
				z = world_z = k;
			}
			//Set direction
			BodyCoordinates(Vector3D k_) :
			k(k_) 
			{
				Vector3D Rent_j(0,1,0);
				i = Rent_j.GetCrossProduct(k_);
				j = k_.GetCrossProduct(i);

				x = world_x = i;
				y = world_y = j;
				z = world_z = k;
			}
			/* Function to update the updated body coordinates according
			   to the initial body coordinates */
			void Update (void)
			{
				x = BodyCoordinates_Orientation*i;
				y = BodyCoordinates_Orientation*j;
				z = BodyCoordinates_Orientation*k;

				world_x = BodyCoordinates_WorldOrientation*i;
				world_y = BodyCoordinates_WorldOrientation*j;
				world_z = BodyCoordinates_WorldOrientation*k;

				x.SetNormalize();
				y.SetNormalize();
				z.SetNormalize();
				world_x.SetNormalize();
				world_y.SetNormalize();
				world_z.SetNormalize();
			}
};

/* Class Bounding Box */
class BoundingBox
{
public:
	Box3D Initial_BBox , World_BBox;
	float BBox_Width , BBox_Height , BBox_Depth;

	void Compute_BoundingBox(Vertex *Vertices , int numberOfVertices)
	{
		float min_X, min_Y , min_Z;
		float max_X , max_Y , max_Z;
		
		min_X = min_Y = min_Z = 5000.0f;
		max_X = max_Y = max_Z = -5000.0f;
		for(int i=0; i<numberOfVertices ; i++)
		{
			min_X = (Vertices[i].X < min_X)? Vertices[i].X : min_X;
			min_Y = (Vertices[i].Y < min_Y)? Vertices[i].Y : min_Y;
			min_Z = (Vertices[i].Z < min_Z)? Vertices[i].Z : min_Z;

			max_X = (Vertices[i].X > max_X)? Vertices[i].X : max_X;
			max_Y = (Vertices[i].Y > max_Y)? Vertices[i].Y : max_Y;
			max_Z = (Vertices[i].Z > max_Z)? Vertices[i].Z : max_Z;
		}

		Initial_BBox.ptA.SetXYZ(max_X*TO_METERS , max_Y*TO_METERS , max_Z*TO_METERS);
		Initial_BBox.ptB.SetXYZ(max_X*TO_METERS , max_Y*TO_METERS , min_Z*TO_METERS);
		Initial_BBox.ptC.SetXYZ(min_X*TO_METERS , max_Y*TO_METERS , min_Z*TO_METERS);
		Initial_BBox.ptD.SetXYZ(min_X*TO_METERS , max_Y*TO_METERS , max_Z*TO_METERS);
		Initial_BBox.ptE.SetXYZ(max_X*TO_METERS , min_Y*TO_METERS , max_Z*TO_METERS);
		Initial_BBox.ptF.SetXYZ(max_X*TO_METERS , min_Y*TO_METERS , min_Z*TO_METERS);
		Initial_BBox.ptG.SetXYZ(min_X*TO_METERS , min_Y*TO_METERS , min_Z*TO_METERS);
		Initial_BBox.ptH.SetXYZ(min_X*TO_METERS , min_Y*TO_METERS , max_Z*TO_METERS);
	}

	bool Check_BSPCollision (Matrix3D ObjectTransformation, BSP_Tree *BSP)
	{
		Vertex Old_Box [8];

		/* Compute the Old Global Box */
		Old_Box[0] = ObjectTransformation * Initial_BBox.ptA;
		Old_Box[1] = ObjectTransformation * Initial_BBox.ptB;
		Old_Box[2] = ObjectTransformation * Initial_BBox.ptC;
		Old_Box[3] = ObjectTransformation * Initial_BBox.ptD;
		Old_Box[4] = ObjectTransformation * Initial_BBox.ptE;
		Old_Box[5] = ObjectTransformation * Initial_BBox.ptF;
		Old_Box[6] = ObjectTransformation * Initial_BBox.ptG;
		Old_Box[7] = ObjectTransformation * Initial_BBox.ptH;

		/* Check each Old Global box Pt with each new Global box Pt for segment intercept */
		if(!BSP->SegmentIntercept(Old_Box[0],World_BBox.ptA,BSP->GetRoot()))
			return true;
		if(!BSP->SegmentIntercept(Old_Box[1],World_BBox.ptB,BSP->GetRoot()))
			return true;
		if(!BSP->SegmentIntercept(Old_Box[2],World_BBox.ptC,BSP->GetRoot()))
			return true;
		if(!BSP->SegmentIntercept(Old_Box[3],World_BBox.ptD,BSP->GetRoot()))
			return true;
		if(!BSP->SegmentIntercept(Old_Box[4],World_BBox.ptE,BSP->GetRoot()))
			return true;
		if(!BSP->SegmentIntercept(Old_Box[5],World_BBox.ptF,BSP->GetRoot()))
			return true;
		if(!BSP->SegmentIntercept(Old_Box[6],World_BBox.ptG,BSP->GetRoot()))
			return true;
		if(!BSP->SegmentIntercept(Old_Box[7],World_BBox.ptH,BSP->GetRoot()))
			return true;
		
		/* No collision */
		return false;
	}

};
/* Computing the bounding box*/

/* Function to classify a point to a plane */
CLASSIFY Classify_Pt_Plane(Vertex point,Plane3D splitter);
CLASSIFY Custom_Classify_PtToPlane (Plane3D Pl, Point3D Pt);
CLASSIFY Custom_ClassifyToBox (Box3D & box, Point3D P);
float Compute_Angle_AccordingTo_Plane(Vector3D Obj_Dir , Plane3D Pln);
Vector3D Compute_PrjectedOn_Plane (Vector3D Source, Plane3D Pln);

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

inline float Abs (float V)
{
	return (V<0)?-V:V;
}

inline Vector3D operator- (Vector3D V)
{
	return Vector3D(-V.X, -V.Y, -V.Z);
}

#endif