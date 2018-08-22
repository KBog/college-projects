#ifndef _AREA_H_
#define _AREA_H_

#include "Object.h"
#include "Item.h"
#include "Alarm.h"
#include "Support_Functions.h"
#include "TestRenderApp.h"
#include "MainCharacter.h"
#include "BSP.h"
#include "Octree.h"

extern MainCharacter *GliCycle;

/* Names of the areas */
enum Area_Name { A1, A2, A3, A4 };


/* L1 - A1 */
Object *Init_Objects_L1_A1 (void);
Item *Init_Items_L1_A1 (void);
Alarm *Init_Alarms_L1_A1 (void);
/* L1 - A2 */
Object *Init_Objects_L1_A2 (void);

struct Area_Partition
{
	public:
		/* Partitioning the Area into Cells */
		float Cell_Width;
		float Cell_Depth;
		int PartitionsWidth_Nbr, PartitionsDepth_Nbr;
		/* 2D Space partition constructor that takes the desired number of octants built in width & depth */
		void Set_AreaPartition(float Area_BBoxWidth, float Area_BBoxDepth, int partition_width, int partition_depth)
		{
			Cell_Width= Area_BBoxWidth / (float)partition_width;
			Cell_Depth= Area_BBoxDepth / (float)partition_depth;
			PartitionsWidth_Nbr=partition_width;
			PartitionsDepth_Nbr=partition_depth;
		}
		/* Displaying all the 2D partition */
		void Draw(BoundingBox bbox, float area_width, float area_depth)
		{
			glDisable(GL_TEXTURE_2D);
			glColor4f(0.17647f,0.48235f,0.63529f,1);

			Renderer *renderer = app.GetRenderer();
			renderer->SetTextureCoordBuffer(0);
			renderer->SetActiveTextureUnit(0);
			
			unsigned short Indices [] = {0,1, 1,2, 2,3, 3,0};
			renderer->SetIndexBuffer(Indices, 8);
			/* Setting an array of 4 vertices to draw quads of the current 2DPartition */
			Vertex Vertices[4];
			/* Setting the XYZ components of ptD Top_Left_Front of the area BBOX */
			float ptD_X= bbox.World_BBox.ptD.X;
			float ptD_Z= bbox.World_BBox.ptD.Z;
			float ptD_Y= bbox.World_BBox.ptD.Y*TO_UNIT;
			float height= fabsf(bbox.World_BBox.ptA.Y-bbox.World_BBox.ptH.Y)/2*TO_UNIT;
			/* Using the method of updating the Width_Depth indices, 
			to go into depth then into width */
			for(int j=0; j<PartitionsDepth_Nbr; j++)
				for(int i=0; i<PartitionsWidth_Nbr; i++)
				{
					/* 1st Point of the Quad */
					Vertices[0].X= (ptD_X + i*Cell_Width)*TO_UNIT;
					Vertices[0].Y= ptD_Y - height;
					Vertices[0].Z= (ptD_Z + j*-Cell_Depth)*TO_UNIT;
					/* 2nd Point of the Quad */
					Vertices[1].X= (ptD_X + Cell_Width + i*Cell_Width)*TO_UNIT;
					Vertices[1].Y= ptD_Y - height;
					Vertices[1].Z= (ptD_Z + j*-Cell_Depth)*TO_UNIT;
					/* 3rd Point of the Quad */
					Vertices[2].X= (ptD_X + Cell_Width + i*Cell_Width)*TO_UNIT;
					Vertices[2].Y= ptD_Y - height;
					Vertices[2].Z= (ptD_Z - Cell_Depth + j*-Cell_Depth)*TO_UNIT;
					/* 4th Point of the Quad */
					Vertices[3].X= (ptD_X + i*Cell_Width)*TO_UNIT;
					Vertices[3].Y= ptD_Y - height;
					Vertices[3].Z= (ptD_Z - Cell_Depth + j*-Cell_Depth)*TO_UNIT;
					renderer->SetVertexBuffer(Vertices);	
				
					renderer->SetWorldTransformation(Matrix3D::GetIdentity());
					renderer->SetNormalBuffer(0);
					renderer->SetPrimitivesType(PrimitiveTypeLineList);
					renderer->Draw();
				}
			glColor4f(1,1,1,1);
			glEnable(GL_TEXTURE_2D);
		}
};

/* Area Class */
class Area : public Object
{
	public:
			/* Area name */
			Area_Name Name;
			/* A list of objects */
			Object *Object_List;
			/* A list of items */
			Item *Item_List;
			/* A list of alarms */
			Alarm *Alarm_List;
			/* A pointer to the next and previous areas */
			Area *next, *prev;
			/* Pointer to partition */
			Area_Partition AreaPartition;

			/* Area constructor */
			Area (string Render_Poly, string Collision_Poly, int name, Point3D position, BodyCoordinates *bodycoordinates, Vector3D orientation, Vector3D rotational_speed, Point3D scale, int partitionwidth_nbr=4, int partitiondepth_nbr=4, Object *object_list=NULL, Item *item_list=NULL, Alarm *alarm_list=NULL);

			/* Area copy constructor */
			Area (Area *a);

			/* Function to update all the things in the current area */
			void Update (void);

			void Render_Area (void);

			void Render_Test (void);

			/* Destructor to unload all the things in the current area */
			~Area ();

};


#endif