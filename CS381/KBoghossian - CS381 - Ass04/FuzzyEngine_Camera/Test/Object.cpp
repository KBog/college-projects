#include "Object.h"

Vector3D StoredOrient;

/* Object Default Constructor */
Object::Object () 
{
	/* Initializing all its members */
	Position.SetXYZ(0,0,0);
	Body_Coordinates = new BodyCoordinates;
	Orientation.SetXYZ(0,0,0);
	Rotational_Speed.SetXYZ(0,0,0);
	Scale.SetXYZ(1,1,1);
	Local_ObjectTransformation.SetIdentity();
	Final_ObjectTransformation.SetIdentity();
	next=prev=NULL;
	Parent=NULL;
	Object_Children=NULL;
	Collision_Loader=Loader=NULL;
}
/* Object Constructor */
Object::Object (string Render_Poly, string Collision_Poly, Point3D position, BodyCoordinates *bodycoordinates, Vector3D orientation, Vector3D rotational_speed, Point3D scale) :
Position(position), World_Position(position), Body_Coordinates(bodycoordinates), Orientation(orientation), Rotational_Speed(rotational_speed), Scale(scale)
{
	/* Taking the shape of the object */
	if(!(Render_Poly=="NO_3DS"))
	{
		Loader = new _3DSLoader;
		/* Load the model */
		_3DSModel model;
		strcpy(model.fileName, Render_Poly.c_str());
		Loader->Load3DSFiles(&model,1);

		/* Computing the bounding box according to the loader */
		BBox.Compute_BoundingBox(Loader->vertices,Loader->numberOfVertices);
	}
	else
		Loader=NULL;
		
	if(!(Collision_Poly=="NO_3DS"))
	{
		Collision_Loader=new _3DSLoader;
		/* Load the model */
		_3DSModel model;
		strcpy(model.fileName, Collision_Poly.c_str());
		Collision_Loader->Load3DSFiles(&model,1);
		/* Allocating the array of vertices */
		Old_Vertices = new Vertex [Collision_Loader->numberOfVertices];
		New_Vertices = new Vertex [Collision_Loader->numberOfVertices];
	}
	else
		Collision_Loader=NULL;

	/* Compute the initial orientation matrix */
	Body_Coordinates->BodyCoordinates_Orientation = (Matrix3D::GetRotateOnY(Orientation.Y)*
													Matrix3D::GetRotateOnX(Orientation.X)*
													Matrix3D::GetRotateOnZ(Orientation.Z));
	Body_Coordinates->BodyCoordinates_WorldOrientation=Body_Coordinates->BodyCoordinates_Orientation;

	/* Compute the initial object transformation */
	Final_ObjectTransformation = Matrix3D::GetTranslate(Position.X, Position.Y, Position.Z)*
							(Matrix3D::GetScale(Scale.X, Scale.Y, Scale.Z)*
							Body_Coordinates->BodyCoordinates_Orientation);
	New_ObjectTransformation = Matrix3D::GetTranslate(Position.X*TO_UNIT, Position.Y*TO_UNIT, Position.Z*TO_UNIT)*
							(Matrix3D::GetScale(Scale.X, Scale.Y, Scale.Z)*
							Body_Coordinates->BodyCoordinates_Orientation);
	Local_ObjectTransformation = Final_ObjectTransformation;
	/* Compute the object matrix without the scale (in case the object is a parent */
	Parent_ObjectMatrix = Matrix3D::GetTranslate(Position.X, Position.Y, Position.Z)*
									Body_Coordinates->BodyCoordinates_Orientation;

	/* Compute the BoundingBox in meters (scaled) */
	Matrix3D Scale_Matrix = Matrix3D::GetScale(Scale.X,Scale.Y,Scale.Z);
	BBox.World_BBox.ptA = Scale_Matrix*BBox.Initial_BBox.ptA;
	BBox.World_BBox.ptB = Scale_Matrix*BBox.Initial_BBox.ptB;
	BBox.World_BBox.ptC = Scale_Matrix*BBox.Initial_BBox.ptC;
	BBox.World_BBox.ptD = Scale_Matrix*BBox.Initial_BBox.ptD;
	BBox.World_BBox.ptE = Scale_Matrix*BBox.Initial_BBox.ptE;
	BBox.World_BBox.ptF = Scale_Matrix*BBox.Initial_BBox.ptF;
	BBox.World_BBox.ptG = Scale_Matrix*BBox.Initial_BBox.ptG;
	BBox.World_BBox.ptH = Scale_Matrix*BBox.Initial_BBox.ptH;

	/* Compute the bounding box dimensions (in meters) */
	/* Width */
	BBox.BBox_Width = fabsf(BBox.World_BBox.ptA.X-BBox.World_BBox.ptD.X);
	/* Height */
	BBox.BBox_Height = fabsf(BBox.World_BBox.ptA.Y-BBox.World_BBox.ptE.Y);
	/* Depth */
	BBox.BBox_Depth = fabsf(BBox.World_BBox.ptA.Z-BBox.World_BBox.ptB.Z);

	/* Set the next and previous pointers to NULL */
	next=prev=NULL;
	Parent=NULL;
	Object_Children=NULL;
}

/* Object Constructor */
Object::Object (_3DSLoader *loader, Point3D position, BodyCoordinates *bodycoordinates, Vector3D orientation, Vector3D rotational_speed, Point3D scale) :
Position(position), World_Position(position), Body_Coordinates(bodycoordinates), Orientation(orientation), Rotational_Speed(rotational_speed), Scale(scale)
{
	Loader = loader;
	/* Allocating the array of vertices */
	Old_Vertices = new Vertex [Loader->numberOfVertices];
	New_Vertices = new Vertex [Loader->numberOfVertices];
	/* Computing the bounding box according to the loader */
	BBox.Compute_BoundingBox(Loader->vertices,Loader->numberOfVertices);

	/* Compute the initial orientation matrix */
	Body_Coordinates->BodyCoordinates_Orientation = (Matrix3D::GetRotateOnY(Orientation.Y)*
													Matrix3D::GetRotateOnX(Orientation.X)*
													Matrix3D::GetRotateOnZ(Orientation.Z));
	Body_Coordinates->BodyCoordinates_WorldOrientation=Body_Coordinates->BodyCoordinates_Orientation;

	/* Compute the initial object transformation */
	Final_ObjectTransformation = Matrix3D::GetTranslate(Position.X, Position.Y, Position.Z)*
							(Matrix3D::GetScale(Scale.X, Scale.Y, Scale.Z)*
							Body_Coordinates->BodyCoordinates_Orientation);
	New_ObjectTransformation = Matrix3D::GetTranslate(Position.X*TO_UNIT, Position.Y*TO_UNIT, Position.Z*TO_UNIT)*
							(Matrix3D::GetScale(Scale.X, Scale.Y, Scale.Z)*
							Body_Coordinates->BodyCoordinates_Orientation);
	Local_ObjectTransformation = Final_ObjectTransformation;
	/* Compute the object matrix without the scale (in case the object is a parent */
	Parent_ObjectMatrix = Matrix3D::GetTranslate(Position.X, Position.Y, Position.Z)*
									Body_Coordinates->BodyCoordinates_Orientation;

	/* Compute the BoundingBox in meters (scaled) */
	Matrix3D Scale_Matrix = Matrix3D::GetScale(Scale.X,Scale.Y,Scale.Z);
	BBox.World_BBox.ptA = Scale_Matrix*BBox.Initial_BBox.ptA;
	BBox.World_BBox.ptB = Scale_Matrix*BBox.Initial_BBox.ptB;
	BBox.World_BBox.ptC = Scale_Matrix*BBox.Initial_BBox.ptC;
	BBox.World_BBox.ptD = Scale_Matrix*BBox.Initial_BBox.ptD;
	BBox.World_BBox.ptE = Scale_Matrix*BBox.Initial_BBox.ptE;
	BBox.World_BBox.ptF = Scale_Matrix*BBox.Initial_BBox.ptF;
	BBox.World_BBox.ptG = Scale_Matrix*BBox.Initial_BBox.ptG;
	BBox.World_BBox.ptH = Scale_Matrix*BBox.Initial_BBox.ptH;

	/* Compute the bounding box dimensions (in meters) */
	/* Width */
	BBox.BBox_Width = fabsf(BBox.World_BBox.ptA.X-BBox.World_BBox.ptD.X);
	/* Height */
	BBox.BBox_Height = fabsf(BBox.World_BBox.ptA.Y-BBox.World_BBox.ptE.Y);
	/* Depth */
	BBox.BBox_Depth = fabsf(BBox.World_BBox.ptA.Z-BBox.World_BBox.ptB.Z);

	/* Set the next and previous pointers to NULL */
	next=prev=NULL;
	Parent=NULL;
	Object_Children=NULL;
}

/* Object destructor */
Object::~Object () 
{
	next = prev = NULL;
	Delete_AllList(&Object_Children);
	/* Delete the loader pointer */
	delete Loader;
	delete Collision_Loader;
	//delete BSP;
	/* Delete the body coordinates */
	delete Body_Coordinates;
}

/* Update the Final Object Matrix relative to the Variables Pos, scale ... */
void Object::Update_ObjectMatrix (Matrix3D &ObjectMatrix)
{
	/* Get the Old matrix to be used for collision (IN UNITS) */
	Old_ObjectTransformation = New_ObjectTransformation;

	/* Transforming the object to its initial status */
	ObjectMatrix = Matrix3D::GetTranslate(Position.X, Position.Y, Position.Z)*
							(Matrix3D::GetScale(Scale.X, Scale.Y, Scale.Z)*
							Body_Coordinates->BodyCoordinates_Orientation);
	
	/* Compute the current object transformation (IN UNITS) */
	New_ObjectTransformation = Matrix3D::GetTranslate(Position.X*TO_UNIT, Position.Y*TO_UNIT, Position.Z*TO_UNIT)*
							(Matrix3D::GetScale(Scale.X, Scale.Y, Scale.Z)*
							Body_Coordinates->BodyCoordinates_Orientation);

	
	/* If the object has a parent */
	if(Parent)
	{
		/* Compute the child's Final Object Transformation as Parent*Local */
		Final_ObjectTransformation = Parent->Parent_ObjectMatrix*ObjectMatrix;
		/* Compute the child's New Object Transformation as Parent*Local (IN UNITS) */
		New_ObjectTransformation =  (Matrix3D::GetTranslate(Parent->World_Position.X*TO_UNIT, Parent->World_Position.Y*TO_UNIT, Parent->World_Position.Z*TO_UNIT)*
									Parent->Body_Coordinates->BodyCoordinates_WorldOrientation)
									* New_ObjectTransformation;
		/* Compute the world position and the world orientation of the object according to its parent (if it has a parent) */
		World_Position = Parent->Parent_ObjectMatrix*Position;
		Body_Coordinates->BodyCoordinates_WorldOrientation = Parent->Body_Coordinates->BodyCoordinates_WorldOrientation * 
																Body_Coordinates->BodyCoordinates_Orientation;
	}
	else
	{
		/* Set the world position and orientation to the local position and orientation */
		World_Position = Position;
		Body_Coordinates->BodyCoordinates_WorldOrientation = Body_Coordinates->BodyCoordinates_Orientation;
	}
	
	/* Store the old parent matrix (to be used 4 collision) */
	Old_Parent_ObjectMatrix = Parent_ObjectMatrix;
	/* Compute the object matrix without the scale (in case the object is a parent */
	Parent_ObjectMatrix = Matrix3D::GetTranslate(World_Position.X, World_Position.Y, World_Position.Z)*
							Body_Coordinates->BodyCoordinates_WorldOrientation;
}

/* Function to update the object's body coordinates */
void Object::Update_ObjectBodyCoordinates ()
{
	Body_Coordinates->Update();
}

/* Function to update the bounding box of the object 
	N.B: The objectMatrix parameter is either the FinalMatrix or the LocalMatrix depending
		if "this" is an object or a child of an object
*/
void Object::Update_ObjectBoundingBox(Matrix3D ObjectMatrix, Matrix3D Parent)
{
	Matrix3D tmp = Parent*ObjectMatrix;
	BBox.World_BBox.ptA = tmp*BBox.Initial_BBox.ptA;
	BBox.World_BBox.ptB = tmp*BBox.Initial_BBox.ptB;
	BBox.World_BBox.ptC = tmp*BBox.Initial_BBox.ptC;
	BBox.World_BBox.ptD = tmp*BBox.Initial_BBox.ptD;
	BBox.World_BBox.ptE = tmp*BBox.Initial_BBox.ptE;
	BBox.World_BBox.ptF = tmp*BBox.Initial_BBox.ptF;
	BBox.World_BBox.ptG = tmp*BBox.Initial_BBox.ptG;
	BBox.World_BBox.ptH = tmp*BBox.Initial_BBox.ptH;

	Matrix3D tmp2 = Matrix3D::GetTranslate(World_Position.X,World_Position.Y,World_Position.Z)*Body_Coordinates->BodyCoordinates_WorldOrientation;
	Global_BBox.World_BBox.ptA = tmp2*Global_BBox.Initial_BBox.ptA;
	Global_BBox.World_BBox.ptB = tmp2*Global_BBox.Initial_BBox.ptB;
	Global_BBox.World_BBox.ptC = tmp2*Global_BBox.Initial_BBox.ptC;
	Global_BBox.World_BBox.ptD = tmp2*Global_BBox.Initial_BBox.ptD;
	Global_BBox.World_BBox.ptE = tmp2*Global_BBox.Initial_BBox.ptE;
	Global_BBox.World_BBox.ptF = tmp2*Global_BBox.Initial_BBox.ptF;
	Global_BBox.World_BBox.ptG = tmp2*Global_BBox.Initial_BBox.ptG;
	Global_BBox.World_BBox.ptH = tmp2*Global_BBox.Initial_BBox.ptH;
}

/* Drawing the body coordinates of the object */
void Object::Draw_BodyCoordinates (Matrix3D Camera_Matrix)
{
	glDisable(GL_TEXTURE_2D);
	glColor4f(1,1,1,1);
	
	Renderer *renderer = app.GetRenderer();
	renderer->SetTextureCoordBuffer(0);
	renderer->SetActiveTextureUnit(0);

	unsigned short Indices [] = {0,1, 0,2, 0,3};
	renderer->SetIndexBuffer(Indices, 6);

	Vertex Vertices[4];
	Vertices[0] = World_Position*TO_UNIT;
	Vertices[1] = Vertices[0] + (Body_Coordinates->world_x)*Scale.X*TO_UNIT;	
	Vertices[2] = Vertices[0] + (Body_Coordinates->world_y)*Scale.X*TO_UNIT;	
	Vertices[3] = Vertices[0] + (Body_Coordinates->world_z)*Scale.X*TO_UNIT;	

	Vertices[0] = Camera_Matrix*Vertices[0];
	Vertices[1] = Camera_Matrix*Vertices[1];
	Vertices[2] = Camera_Matrix*Vertices[2];
	Vertices[3] = Camera_Matrix*Vertices[3];

	renderer->SetVertexBuffer(Vertices);	
	
	renderer->SetWorldTransformation(Matrix3D::GetIdentity());
	renderer->SetNormalBuffer(0);

	renderer->SetPrimitivesType(PrimitiveTypeLineList);
	renderer->Draw();

	glEnable(GL_TEXTURE_2D);
}

/* Function to draw the bounding box of the object */
void Object::Draw_BoundingBox (Matrix3D Camera_Matrix)
{
	glDisable(GL_TEXTURE_2D);
	glColor4f(0,1,0,1);

	Renderer *renderer = app.GetRenderer();
	renderer->SetTextureCoordBuffer(0);
	renderer->SetActiveTextureUnit(0);

	unsigned short Indices [] = {0,1, 1,2, 2,3, 3,0,  0,4, 1,5, 2,6, 3,7,  4,5, 5,6, 6,7, 7,4};
	renderer->SetIndexBuffer(Indices, 24);

	Vertex Vertices[8];
	Vertices[0] = Camera_Matrix*(BBox.World_BBox.ptA*TO_UNIT);	Vertices[4] = Camera_Matrix*(BBox.World_BBox.ptE*TO_UNIT);
	Vertices[1] = Camera_Matrix*(BBox.World_BBox.ptB*TO_UNIT);	Vertices[5] = Camera_Matrix*(BBox.World_BBox.ptF*TO_UNIT);
	Vertices[2] = Camera_Matrix*(BBox.World_BBox.ptC*TO_UNIT);	Vertices[6] = Camera_Matrix*(BBox.World_BBox.ptG*TO_UNIT);
	Vertices[3] = Camera_Matrix*(BBox.World_BBox.ptD*TO_UNIT);	Vertices[7] = Camera_Matrix*(BBox.World_BBox.ptH*TO_UNIT);

	renderer->SetVertexBuffer(Vertices);	
	
	renderer->SetWorldTransformation(Matrix3D::GetIdentity());
	renderer->SetNormalBuffer(0);

	renderer->SetPrimitivesType(PrimitiveTypeLineList);
	renderer->Draw();

	////
	glColor4f(0,0,1,1);

	Vertices[0] = Camera_Matrix*(Global_BBox.World_BBox.ptA*TO_UNIT);	Vertices[4] = Camera_Matrix*(Global_BBox.World_BBox.ptE*TO_UNIT);
	Vertices[1] = Camera_Matrix*(Global_BBox.World_BBox.ptB*TO_UNIT);	Vertices[5] = Camera_Matrix*(Global_BBox.World_BBox.ptF*TO_UNIT);
	Vertices[2] = Camera_Matrix*(Global_BBox.World_BBox.ptC*TO_UNIT);	Vertices[6] = Camera_Matrix*(Global_BBox.World_BBox.ptG*TO_UNIT);
	Vertices[3] = Camera_Matrix*(Global_BBox.World_BBox.ptD*TO_UNIT);	Vertices[7] = Camera_Matrix*(Global_BBox.World_BBox.ptH*TO_UNIT);

	renderer->SetVertexBuffer(Vertices);	
	
	renderer->SetWorldTransformation(Matrix3D::GetIdentity());
	renderer->SetNormalBuffer(0);

	renderer->SetPrimitivesType(PrimitiveTypeLineList);
	renderer->Draw();
	//

	glColor4f(1,1,1,1);
	glEnable(GL_TEXTURE_2D);
}

/* Render the object according to the default rendering matrix = T*S*R */
void Object::Render (Renderer *renderer)
{
	if(Loader)
	{
		Matrix3D Rendering_Matrix = Matrix3D::GetTranslate(World_Position.X*TO_UNIT, World_Position.Y*TO_UNIT, World_Position.Z*TO_UNIT)*
									(Matrix3D::GetScale(Scale.X, Scale.Y, Scale.Z)*
									Body_Coordinates->BodyCoordinates_WorldOrientation);
		renderer->SetWorldTransformation(Rendering_Matrix);
		Loader->Render(renderer);
		if(TEST_ENGINE)
		{
			Draw_BodyCoordinates();
			Draw_BoundingBox();
		}
	}

	for(Object *tmp_child=Object_Children; tmp_child; tmp_child=tmp_child->next)
		tmp_child->Render(renderer);
}

/* Render the object according to the sent paramter rendering matrix (used by physics & hierarchy ...) */
void Object::Render (Renderer *renderer, Matrix3D Rendering_Matrix)
{
	if(Loader)
	{
		renderer->SetWorldTransformation(Rendering_Matrix);
		Loader->Render(renderer);
		if(TEST_ENGINE)
		{
			Draw_BodyCoordinates();
			Draw_BoundingBox();
		}
	}
	for(Object *tmp_child=Object_Children; tmp_child; tmp_child=tmp_child->next)
		tmp_child->Render(renderer);
}

/* Render the object according to the default rendering matrix = T*S*R */
void Object::RenderAsHUD (Renderer *renderer)
{
	if(Loader)
	{
		Matrix3D Rendering_Matrix = Matrix3D::GetTranslate(World_Position.X*TO_UNIT, World_Position.Y*TO_UNIT, World_Position.Z*TO_UNIT)*
									(Matrix3D::GetScale(Scale.X, Scale.Y, Scale.Z)*
									Body_Coordinates->BodyCoordinates_WorldOrientation);
		Matrix3D Camera_Matrix = renderer->GetViewingTransformation();
		Matrix3D::GetInverse4x4(Camera_Matrix,Camera_Matrix);
		renderer->SetWorldTransformation(Camera_Matrix*Rendering_Matrix);
		Loader->Render(renderer);
		if(TEST_ENGINE)
		{
			Draw_BodyCoordinates(Camera_Matrix);
			Draw_BoundingBox(Camera_Matrix);
		}
	}

	for(Object *tmp_child=Object_Children; tmp_child; tmp_child=tmp_child->next)
		tmp_child->RenderAsHUD(renderer);
}

/* Function to attach a child to its parent (The area must not attach a child to it, it must add_toList to the LinkList member) */
void Object::Attach_Child (Object *Child)
{
	if(Child)
	{
		/* Update the child's parameters according to its parent */
		Child->Position.X*=Scale.X;	Child->Position.Y*=Scale.Y;	Child->Position.Z*=Scale.Z;
		Child->Scale.X*=Scale.X;	Child->Scale.Y*=Scale.Y;	Child->Scale.Z*=Scale.Z;
		/* Set the parent */
		Child->Parent=this;
		/* Add a child to the end of the list */
		Add_ToList(&Object_Children,Child);
	}
}

/* Function to detach a child at a specific index from the Object_Children LinkList and re-attaches it to another linklist 
	only if the NewParent is not NULL, else it will delete the detached child */
void Object::Detach_Child (int ChildIndex, Object **NewParent)
{
	/* If there is a new parent */
	if(*NewParent)
		(*NewParent)->Attach_Child(Remove_FromList(&Object_Children,Return_AtIndex(&Object_Children,ChildIndex)));
	else
		delete Remove_FromList(&Object_Children,Return_AtIndex(&Object_Children,ChildIndex));
}

/* Compute the BBox recursively */
void Compute_BBox_Dimensions(Object *obj, float &min_X, float &min_Y, float &min_Z, float &max_X, float &max_Y, float &max_Z)
{
	Point3D Pts[8];
	Matrix3D Trans = Matrix3D::GetTranslate(obj->Position.X,obj->Position.Y,obj->Position.Z)*Matrix3D::GetScale(obj->Scale.X,obj->Scale.Y,obj->Scale.Z);
	
	Pts[0] = Trans*obj->BBox.Initial_BBox.ptA;
	Pts[1] = Trans*obj->BBox.Initial_BBox.ptB;
	Pts[2] = Trans*obj->BBox.Initial_BBox.ptC;
	Pts[3] = Trans*obj->BBox.Initial_BBox.ptD;
	Pts[4] = Trans*obj->BBox.Initial_BBox.ptE;
	Pts[5] = Trans*obj->BBox.Initial_BBox.ptF;
	Pts[6] = Trans*obj->BBox.Initial_BBox.ptG;
	Pts[7] = Trans*obj->BBox.Initial_BBox.ptH;

	for(int i=0; i<8; i++)
	{
		if(Pts[i].X > max_X)
			max_X = Pts[i].X;
		else if(Pts[i].X < min_X)
			min_X = Pts[i].X;
		if(Pts[i].Y > max_Y)
			max_Y = Pts[i].Y;
		else if(Pts[i].Y < min_Y)
			min_Y = Pts[i].Y;
		if(Pts[i].Z > max_Z)
			max_Z = Pts[i].Z;
		else if(Pts[i].Z < min_Z)
			min_Z = Pts[i].Z;
	}

	/* Loop through the children */
	for(Object *tmp = obj->Object_Children; tmp; tmp = tmp->next)
		Compute_BBox_Dimensions(tmp, min_X, min_Y, min_Z, max_X, max_Y, max_Z);
}

/* A function  that updates the globa bounding box of the parent and its children */
void Object::Compute_GlobalBoundingBox(void)
{
	/* Call the update object to get the matrices with the hierarchy working properly */
	Update_Object();
	/* Compute the initial bbox dimensions */
	Point3D Pts[8];
	Matrix3D Trans = Matrix3D::GetScale(Scale.X,Scale.Y,Scale.Z);
	Pts[0] = Trans*BBox.Initial_BBox.ptA;
	Pts[1] = Trans*BBox.Initial_BBox.ptB;
	Pts[2] = Trans*BBox.Initial_BBox.ptC;
	Pts[3] = Trans*BBox.Initial_BBox.ptD;
	Pts[4] = Trans*BBox.Initial_BBox.ptE;
	Pts[5] = Trans*BBox.Initial_BBox.ptF;
	Pts[6] = Trans*BBox.Initial_BBox.ptG;
	Pts[7] = Trans*BBox.Initial_BBox.ptH;
	float min_X = Pts[6].X, min_Y = Pts[6].Y , min_Z = Pts[6].Z;
	float max_X = Pts[0].X, max_Y = Pts[0].Y , max_Z = Pts[0].Z;

	/* Compute the Dimensions recursively according to the children */
	for(Object *tmp = Object_Children; tmp; tmp = tmp->next)
		Compute_BBox_Dimensions(tmp, min_X, min_Y, min_Z, max_X, max_Y, max_Z);

	/* Set the Global Bounding Box */
	Global_BBox.Initial_BBox.ptA.SetXYZ(max_X , max_Y , max_Z);
	Global_BBox.Initial_BBox.ptB.SetXYZ(max_X , max_Y , min_Z);
	Global_BBox.Initial_BBox.ptC.SetXYZ(min_X , max_Y , min_Z);
	Global_BBox.Initial_BBox.ptD.SetXYZ(min_X , max_Y , max_Z);
	Global_BBox.Initial_BBox.ptE.SetXYZ(max_X , min_Y , max_Z);
	Global_BBox.Initial_BBox.ptF.SetXYZ(max_X , min_Y , min_Z);
	Global_BBox.Initial_BBox.ptG.SetXYZ(min_X , min_Y , min_Z);
	Global_BBox.Initial_BBox.ptH.SetXYZ(min_X , min_Y , max_Z);
}

/* Function to update the object + its children */
void Object::Update_Object (void)
{
	/* Update the object according to the overwritten functions */
	Update_AI();
	Update_Effect();
	Update_Physics();
	/* Loop through the children to update each one by one */
	for(Object *tmp_child=Object_Children; tmp_child; tmp_child=tmp_child->next)
		tmp_child->Update_Object();
}

/* Function to update the old & new vertices to be used for collision (segment intercept) */
void Object::Update_OldNewVertices (void)
{
	/* Loop through the number of vertices in the low poly model to compute the old and new verices */
	for(int i=0; i<Collision_Loader->numberOfVertices; i++)
	{
		Old_Vertices[i] = Old_ObjectTransformation*Collision_Loader->vertices[i];
		New_Vertices[i] = New_ObjectTransformation*Collision_Loader->vertices[i];
	}
}

/* Function to update the POSITION ID in the Partition */
void Object::Update_Partition (float cell_width, float cell_depth)
{
	int i= int(Position.X / cell_width);
	int j= int(Position.Z / cell_depth);
	partitionID.SetID(i,j);
}

/* Function to know the current object ID if it is with the second object ID */
bool Object::Check_ID_With (Object *Obj)
{
	return (partitionID.i==Obj->partitionID.i && partitionID.j==Obj->partitionID.j);
}

/* Function to update the collision of each object with each one */
void Object::Update_Collision (Object *Obj_List, Object *Area)
{
	///* Loop through the objects in the area */
	//for(Object *tmp_obj=Obj_List; tmp_obj; tmp_obj=tmp_obj->next)
	//{
	//	/* if the checking with object is not the this */
	//	if(this!=tmp_obj)
	//	{
	//		/* Check its ID with the checking one */
	//		if(Check_ID_With(tmp_obj))
	//		{
	//			/* if the 2 bounding volumes intersect */
	//			//MUST NOT CALL THE BBOX
	//			if(Global_BBox.World_BBox.ClassifyToBox(tmp_obj->Global_BBox.World_BBox)==INTERSECT)
	//			{
	//				Check_CharacterBSP_Collision(tmp_obj->BSP);
	//				//Maybe shouldn't break
	//				break;
	//			}
	//		}
	//	}
	//}

	/* Check the object with the areaBSP */
	//if(Global_BBox.Check_BSPCollision(Old_Parent_ObjectMatrix,BSP))
	//{
		Check_CharacterBSP_Collision(Area);
	//}
}

/* Check each character BBox then Old&New vertices with the BSP then recursively call the children to check for collision */
void Object::Check_CharacterBSP_Collision (Object *object)
{
	/* Check BBox with the BSP */
	//MUST NOT CALL THE BOUNDING VOLUME COLLISION
	//if(BBox.Check_BSPCollision(Final_ObjectTransformation,BSP))
	//{
		/* Update the Old & New vertices to check dynamic collision */
		Update_OldNewVertices();
		/* Virtual call to do the dynamic collision between objects (Vertices of Obj1 with BSP of Obj2) */
		Update_DynamicCollision_WithResponse(object);
	//}
	/* Loop recursively to check each child with the BSP */
	for(Object *tmp_child = Object_Children; tmp_child; tmp_child=tmp_child->next)
		tmp_child->Check_CharacterBSP_Collision(object);
}

/* Function to get the collision points */
bool Object::Get_CollisionPoints (BSP_Tree *BSP, vector<Point3D> &CollisionPoints, vector<Point3D> &CollisonFacesPoints, vector<Vector3D> &CollisionFacesNormals)
{
	bool Collided=false;
	/* Loop through all the vertices in the collision loader */
	for(int i=0; i<Collision_Loader->numberOfVertices; i++)
	{
		/* Check if the old vertex has collided */ /* ----------> SEG INTE MUST RETURN NORMAL + PT to be added to CollisionFacesPoints */
		if(!BSP->SegmentIntercept(Old_Vertices[i],New_Vertices[i],BSP->GetRoot()))
		{
			Collided=true;
			/* Add the Old vertex to the array of collided vertices */
			CollisionPoints.push_back(Old_Vertices[i]);

		}
	}
	/* return if any vertex collided or not */
	return Collided;
}








