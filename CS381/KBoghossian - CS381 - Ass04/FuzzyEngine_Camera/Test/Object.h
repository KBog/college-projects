#ifndef _OBJECT_H_
#define _OBJECT_H_

#include <Application.h>
#include "TestRenderApp.h"
#include "3dsfile.h"
#include "Support_Functions.h"
#include "Bsp.h"
#include <GL/gl.h>
#include <GL/glu.h>

extern float Slow_Motion;

/* ObjectID class to specify the current object position in the partition */
struct PartitionID
{
	int i, j;
	
	/* Partition ID constructor */
	void SetID(int _i, int _j)
	{
		i=_i;
		j=_j;
	}
};

class Object
{
	public:
			/* Object Local Position & Object World Position (if it is a child) - (both are in meters) */
			Point3D Position, World_Position;		
			/* Vector Direction of the object */
			BodyCoordinates *Body_Coordinates;			
			/* Orientation of the object (angles) */
			Vector3D Orientation;
			/* Speed of the Rotation about the axis */
			Vector3D Rotational_Speed;
			/* Sx, Sy and Sz of the object */
			Point3D Scale;				
			/* Contains all the transformations required
			   for the final object's rendering */
			Matrix3D Final_ObjectTransformation;	
			/* Contains all the transformations required
			   to compute the hierarchy */
			Matrix3D Local_ObjectTransformation;
			/* Matrix to hold the object transformation of the old game loop 
			   (to do the segment intercept collision) in UNITS */
			Matrix3D Old_ObjectTransformation;
			Matrix3D New_ObjectTransformation;
			/* Contains the ObjectMatrix without the scale 
			   (To be used for the heirarchy) */
			Matrix3D Parent_ObjectMatrix, Old_Parent_ObjectMatrix;
			/* A box to envelop the object */
			BoundingBox BBox, Global_BBox;
			/* The object that belongs to which partition */
			PartitionID partitionID;
			/* To hold the model of the object */
			_3DSLoader *Loader, *Collision_Loader;
			/* The 2 arrays of vertices to check collision using the segment intercept */
			Vertex *Old_Vertices, *New_Vertices;
			/* A pointer to the Object's Parent */
			Object *Parent;
			/* A linklist of Objects to store the children */
			Object *Object_Children;
			/* The object's BSP */
			BSP_Tree *BSP;

			/* Pointer to the next and prev object in a list */
			Object *next, *prev;

			/* Object Default Constructor */
			Object ();
			/* Object Constructor */
			Object (string Render_Poly, string Collision_Poly, Point3D position, BodyCoordinates *bodycoordinates, Vector3D orientation, Vector3D rotational_speed, Point3D scale);
			/* Object Constructor */
			Object (_3DSLoader *loader, Point3D position, BodyCoordinates *bodycoordinates, Vector3D orientation, Vector3D rotational_speed, Point3D scale);

			virtual void Update_AI (void) {}		//Update the Local Matrix & Finalize the Final Matrix
			virtual void Update_Physics (void) {}
			virtual void Update_Effect (void) {}
			virtual void Update_DynamicCollision_WithResponse (Object *object) {}

			/* Update the Final Object Matrix relative to the Variables Pos, scale ... */
			void Update_ObjectMatrix (Matrix3D &ObjectMatrix);

			/* Function to update the object's body coordinates */
			void Update_ObjectBodyCoordinates ();

			/* A function  that updates the globa bounding box of the parent and its children */
			void Compute_GlobalBoundingBox(void);
			/* Function to update the bounding box of the object 
				N.B: The objectMatrix parameter is either the FinalMatrix or the LocalMatrix depending
					if "this" is an object or a child of an object
			*/
			void Update_ObjectBoundingBox(Matrix3D ObjectMatrix, Matrix3D Parent=Matrix3D::GetIdentity());
			/* Drawing the body coordinates of the object */
			void Draw_BodyCoordinates (Matrix3D Camera_Matrix = Matrix3D::GetIdentity());
			/* Function to draw the bounding box of the object */
			void Draw_BoundingBox (Matrix3D Camera_Matrix=Matrix3D::GetIdentity());
			/* Render the object according to the default rendering matrix = T*S*R */
			void Render (Renderer *renderer);
			/* Render the object according to the sent paramter rendering matrix (used by physics & hierarchy ...) */
			void Render (Renderer *renderer, Matrix3D Rendering_Matrix);
			/* Render the object according to the default rendering matrix = T*S*R */
			void RenderAsHUD (Renderer *renderer);
			/* Function to attach a child to its parent (The area must not attach a child to it, it must add_toList to the LinkList member) */
			void Attach_Child (Object *Child);
			/* Function to detach a child at a specific index from the Object_Children LinkList and re-attaches it to another linklist 
			   only if the NewParent is not NULL, else it will delete the detached child */
			void Detach_Child (int ChildIndex, Object **NewParent=NULL);
			/* Function to update the object + its children */
			void Update_Object (void);
			/* Function to update the old & new vertices to be used for collision (segment intercept) */
			void Update_OldNewVertices (void);
			/* Function to update the POSITION ID in the Partition */
			void Update_Partition (float cell_width, float cell_depth);
			/* Function to check if the object collided with any object (including the Area) */
			void Update_Collision (Object *Obj_List, Object *Area);
			/* Function to know the current object ID if it is with the second object ID */
			bool Check_ID_With (Object *Obj);
			/* Check each vertex in the Low Poly with the BSP */
			bool Check_GlobalBox_AreaBSP_Collision (BSP_Tree *BSP);
			//
			void Check_CharacterBSP_Collision (Object *object);
			/* Function to get the collision points */
			bool Get_CollisionPoints (BSP_Tree *BSP, vector<Point3D> &CollisionPoints, vector<Point3D> &CollisonFacesPoints, vector<Vector3D> &CollisionFacesNormals);
			/* Object destructor */
			virtual ~Object ();
};

#endif