#include "MainCharacter.h"
#include "Item_Ammo.h"
#include "Item_Health.h"
#include "Item_SlowMotion.h"
#include "Item_Crate.h"
#include "Alarm_Door.h"
#include "Area.h"
#include "Tank.h"
#include "Camera.h"

extern bool NetIsOn;
extern bool Free_Cam;
extern Camera *Main_Camera;

/* ------------->>>>>>>>>>>>>>>>>>>>>>> MODIFY THE COPY CONSTRUCTOR <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< ----------------------- */

/* Area constructor */
Area::Area (string Render_Poly, string Collision_Poly, int name, Point3D position, BodyCoordinates *bodycoordinates, Vector3D orientation, Vector3D rotational_speed, Point3D scale, int partitionwidth_nbr, int partitiondepth_nbr, Object *object_list, Item *item_list, Alarm *alarm_list) : 
Name((Area_Name)name), Object_List(object_list), Item_List(item_list), Alarm_List(alarm_list),
Object(Render_Poly,Collision_Poly,position,bodycoordinates,orientation,rotational_speed,scale)
{
	next=prev=NULL;
	//BSP = new BSP_Tree(&Collision_Loader);	//DELETE THE BSP
	//BSP->SetRenderer(app.GetRenderer());
	/* ------------------------------------------ OCTREE -------------------------------------*/
	//AreaOctree = new Octree(Loader,app.GetRenderer());
	//float Largest=0;
	//if(BBox_Depth>BBox_Height)
	//{
	//	if(BBox_Depth>BBox_Width)
	//		Largest=BBox_Depth;
	//	else
	//		Largest=BBox_Width;
	//}
	//else
	//{
	//	if(BBox_Height>BBox_Width)
	//		Largest=BBox_Height;
	//	else
	//		Largest=BBox_Width;
	//}
	/*AreaOctree->GetWorldDimensions(Largest*TO_UNIT);
	unsigned short *pObjectIndex = new unsigned short[Loader->numberOfObjects];
	for(int i=0;i<Loader->numberOfObjects;i++)
			pObjectIndex[i]=i;
	AreaOctree->CreateOctant(Loader->vertexIndices,Loader->numberOfFaces*3, Loader->objectFaceNumber, pObjectIndex, Loader->numberOfObjects,AreaOctree->GetCenter(), AreaOctree->GetWidth());
	AreaOctree->Compute_Octant8Points(AreaOctree);*/
	/* ------------------------------------------ ENDOCTREE -------------------------------------*/
	AreaPartition.Set_AreaPartition(BBox.BBox_Width,BBox.BBox_Depth,partitionwidth_nbr,partitiondepth_nbr);
	/* Setting the parent of the object in the area just for the first time, later on the objects won't follow the area */
	for(Object *tmp_object=Object_List; tmp_object; tmp_object=tmp_object->next)
		tmp_object->Parent=this;
	for(Item *tmp_item=Item_List; tmp_item; tmp_item=tmp_item->next)
		tmp_item->Parent=this;
}

/* Area copy constructor */
Area::Area (Area *a)
{
	Name=a->Name;
	/* Copying the Object class from the area */
	Position=a->Position;
	Body_Coordinates=a->Body_Coordinates;
	Orientation=a->Orientation;
	Rotational_Speed=a->Rotational_Speed;
	Scale=a->Scale;
	Local_ObjectTransformation=a->Local_ObjectTransformation;
	Final_ObjectTransformation=a->Final_ObjectTransformation;
	Parent_ObjectMatrix=a->Parent_ObjectMatrix;
	Loader=a->Loader;
	Body_Coordinates=a->Body_Coordinates;
	/* Copying the content of the area */
	Object_List=a->Object_List;
	Item_List=a->Item_List;
	Alarm_List=a->Alarm_List;
	BSP=a->BSP;
	AreaPartition=a->AreaPartition;
	BBox=a->BBox;
	Global_BBox=a->Global_BBox;
	next=prev=NULL;
}

/* Destructor to unload all the things in the current area */
Area::~Area ()
{
	/* **** Delete Object List **** */
	Delete_AllList(&Object_List);
	/* **** Delete Item List **** */
	Delete_AllList(&Item_List);
	/* **** Delete Alarm List **** */
	Delete_AllList(&Alarm_List);

	//delete AreaBSP;
}

/* Function to update all the things in the current area */
void Area::Update (void)
{
	/*if(Keyboard::IsPressed(KeyboardKeyI))
	{
		this->Position.X++;
	}*/

	Renderer *renderer = app.GetRenderer();

	//Getting  the camera position
	//Matrix3D currentViewingMatrix,m,j;
	//m=Main_Camera->Current_ViewingMatrix;
	//currentViewingMatrix=Main_Camera->Old_ViewingMatrix;
	//Matrix3D v=m*currentViewingMatrix;
	//renderer->SetViewingTransformation(v);
	//Matrix3D inv = Matrix3D::GetInverse4x4(v);
	//Point3D pt = Point3D(0,0,0), pt1=Point3D(0,0,-20);
	//pt = inv*pt;
	//pt1=inv*pt1;

	//if(!AreaBSP->SegmentIntercept(pt,pt1,AreaBSP->GetRoot()))
	//{
	//	//Main_Camera->CameraViewing = currentViewingMatrix;
	//	//Main_Camera->Collided=true;
	//}

	Update_ObjectMatrix(Final_ObjectTransformation);
	Update_ObjectBoundingBox(Final_ObjectTransformation);
	Update_ObjectBodyCoordinates();
	//BSP->Update_BSP_Vertices(Final_ObjectTransformation);

	//GliCycle->Update_Collision(Object_List,AreaBSP);

	/* Looping through the list of objects in the current area of the current level */
	for(Object *tmp_object=Object_List; tmp_object; tmp_object=tmp_object->next)
	{
		/* Set the parent of each area's object to the area (if u want the objects to follow the area each GL) */
		//tmp_object->Parent=this;
		//if(AreaBSP->SegmentIntercept(tmp_object->Position*TO_UNIT,Point3D(tmp_object->Position.X*TO_UNIT,tmp_object->Position.Y*TO_UNIT+10,tmp_object->Position.Z*TO_UNIT),AreaBSP->GetRoot()))
		
		tmp_object->Update_Object();
		tmp_object->Update_Partition(AreaPartition.Cell_Width,AreaPartition.Cell_Depth);
		//tmp_object->Update_Collision(Object_List, this);
	}

	/* Looping through the list of items in the current area of the current level */
	Item* Next_Item=NULL;
	for(Item *tmp_item=Item_List; tmp_item; tmp_item=Next_Item)
	{
		Next_Item=tmp_item->next;
		/* Set the parent of each area's object to the area (if u want the objects to follow the area each GL) */
		tmp_item->Parent=this;
		tmp_item->Update_Object();
		if(GliCycle->Collision_WithItem(tmp_item))
		{
			tmp_item->BehaviorWhenTaken();
			delete Remove_FromList(&Item_List,tmp_item);
		}
	}
	/* Looping through the list of alarms in the current area of the current level */
	for(Alarm *tmp_alarm=Alarm_List; tmp_alarm; tmp_alarm=tmp_alarm->next)
	{
	}
}

void Area::Render_Area (void)
{
	/* Getting the renderer */
	Renderer *renderer = app.GetRenderer();
	Render(renderer);

	AreaPartition.Draw(BBox,BBox.BBox_Width*TO_UNIT,BBox.BBox_Depth*TO_UNIT);
	/*Matrix3D Rendering_Matrix = Matrix3D::GetTranslate(World_Position.X*TO_UNIT, World_Position.Y*TO_UNIT, World_Position.Z*TO_UNIT)*
									(Matrix3D::GetScale(Scale.X, Scale.Y, Scale.Z)*
									Body_Coordinates->BodyCoordinates_WorldOrientation);

	AreaOctree->RenderOctree(AreaOctree,Rendering_Matrix);
	unsigned short Indices [] = {0,1, 1,2, 2,3, 3,0,  0,4, 1,5, 2,6, 3,7,  4,5, 5,6, 6,7, 7,4};
	app.GetRenderer()->SetIndexBuffer(Indices, 24);
	AreaOctree->DrawOctantBoundaries(AreaOctree,Rendering_Matrix);*/

	Matrix3D currentViewingMatrix,m,j;
	currentViewingMatrix=renderer->GetViewingTransformation();
	Matrix3D v=m*currentViewingMatrix;
	renderer->SetViewingTransformation(v);
	Matrix3D inv = Matrix3D::GetInverse4x4(v);
	Point3D pt = Point3D(0,0,0), pt1=Point3D(0,0,-20);
	pt = inv*pt;
	pt1=inv*pt1;
	//BSP->RenderNormalsBSP_Tree(pt);
	//BSP->RenderBSP_Tree(pt);

	/* Rendering the objects */
	for(Object *tmp_object=Object_List; tmp_object; tmp_object=tmp_object->next)
	{
		tmp_object->Render(renderer);
	}
	
	/* Looping through the list of items in the current area of the current level */
	for(Item *tmp_item=Item_List; tmp_item; tmp_item=tmp_item->next)
		tmp_item->Render(renderer);
}

void Area::Render_Test (void)
{
	Draw_BodyCoordinates();
	Draw_BoundingBox();
	/* Rendering the objects */
	for(Object *tmp_object=Object_List; tmp_object; tmp_object=tmp_object->next)
	{
		tmp_object->Draw_BodyCoordinates();
		tmp_object->Draw_BoundingBox();
	}
	/* Looping through the list of items in the current area of the current level */
	for(Item *tmp_item=Item_List; tmp_item; tmp_item=tmp_item->next)
	{
		tmp_item->Draw_BodyCoordinates();
		tmp_item->Draw_BoundingBox();
	}
}

/* Add all the objects in the area */
Object *Init_Objects_L1_A1 (void)
{
	/*Character List is a Two-Way Linked list*/			
	Object *head=NULL;			

	//Add_ToList(&head, (Object *)new MainCharacter(app.GetDirectory() + "Resources\\BoxTest.3DS",2,100,Point3D(0,0,0),10,NULL,NULL,Point3D(0,0,0), Vector3D(0,0,-1), Vector3D(0,0,0), Point3D(0.1f,0.1f,0.1f)));
	Add_ToList(&head, (Object *)new Tank(app.GetDirectory() + "Resources\\Tank_Bottom.3DS",app.GetDirectory() + "Resources\\Tank_Bottom.3DS",Point3D(0,0,0),new BodyCoordinates,Vector3D(0,90,0),Vector3D(0,0,0),Point3D(0.2f,0.2f,0.2f),0.5f,ROH_AIR,Vector3D(0,0,0),OFF,NULL));
	
	return head;
}
/* Add all the items in the area */
Item *Init_Items_L1_A1 (void)
{
	/*Character List is a Two-Way Linked list*/			
	Item *head=NULL;			

	/*Add_ToList(&head, (Item *)new Item_MainCharacter_Ammo(app.GetDirectory() + "Resources\\Item_MachineGun.3DS",Bullet_Type,50,0.625f,0.0125f,Point3D(0,2.5f,0)));
	Add_ToList(&head, (Item *)new Item_MainCharacter_Ammo(app.GetDirectory() + "Resources\\Item_RocketLauncher.3DS",Rocket_Type,15,0.625f,0.0125f,Point3D(-2.5f,2.5f,0)));
	Add_ToList(&head, (Item *)new Item_MainCharacter_Ammo(app.GetDirectory() + "Resources\\Item_AtomBomb.3DS",AtomBomb_Type,15,0.625f,0.0125f,Point3D(2.5f,2.5f,-1.25f)));
	Add_ToList(&head, (Item *)new Item_MainCharacter_Health(app.GetDirectory() + "Resources\\Item_HealthSmall.3DS",25,0.375f,0.0025f,Point3D(0,2.5f,1.25f)));
	Add_ToList(&head, (Item *)new Item_MainCharacter_Health(app.GetDirectory() + "Resources\\Item_Health.3DS",25,0.625f,0.0125f,Point3D(0,2.5f,-1.25f)));
	Add_ToList(&head, (Item *)new Item_MainCharacter_SlowMotion(app.GetDirectory() + "Resources\\Item_SlowMotion.3DS",360,Point3D(2.5f,2.5f,1.25f),new BodyCoordinates, Vector3D(0,0,0),Vector3D(3,2,1)));*/
	
	return head;
}
/* Add all the items in the area */
Alarm *Init_Alarms_L1_A1 (void)
{
	/*Character List is a Two-Way Linked list*/			
	Alarm *head=NULL;			

	//Add_ToList(&head, (Alarm *)new Alarm_Door(NULL,1,0,Vector3D(0,0,1)));
	
	return head;
}


/* Add all the objects in the area */
Object *Init_Objects_L1_A2 (void)
{
	/*Character List is a Two-Way Linked list*/			
	Object *head=NULL;			

	//Add_ToList(&head, (Object *)new MainCharacter(2,100,Point3D(0,0,0),10,NULL,NULL,Point3D(0,0,0), Vector3D(0,0,-1), Vector3D(0,0,0), Point3D(1,1,1)));
	//Add_ToList(&head, new Object(Point3D(0,0,0), Vector3D(1,1,1), Vector3D(0,0,0), Point3D(1,1,1)));
	
	return head;
}

