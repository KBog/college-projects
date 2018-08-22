#include "Tank.h"


void Tank_TopPart::Update_AI ()
{
	if(Keyboard::IsPressed(KeyboardKeyU))
	{
		Body_Coordinates->BodyCoordinates_Orientation = Matrix3D::GetRotateArbitraryAxis(Body_Coordinates->y,0.35f) * Body_Coordinates->BodyCoordinates_Orientation;
	}
	else if(Keyboard::IsPressed(KeyboardKeyO))
	{
		Body_Coordinates->BodyCoordinates_Orientation = Matrix3D::GetRotateArbitraryAxis(Body_Coordinates->y,-0.35f) * Body_Coordinates->BodyCoordinates_Orientation;
	}
	

	Update_ObjectBodyCoordinates();
	Update_ObjectMatrix(Local_ObjectTransformation);
	Update_ObjectBoundingBox(Local_ObjectTransformation,Parent->Parent_ObjectMatrix);
}
void Tank_TopPart::Update_DynamicCollision_WithResponse(BSP_Tree *BSP)
{
	/*bool Collided=false;
	for(int i=0; i<Collision_Loader->numberOfVertices; i++)
	{
		if(!BSP->SegmentIntercept(Old_Vertices[i],New_Vertices[i],BSP->GetRoot()))
			Collided=true;
	}
	if(Collided)
		Position += Body_Coordinates->z;*/
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Tank_FrontPart::Update_AI ()
{
	if(Keyboard::IsPressed(KeyboardKeyK))
	{
		Body_Coordinates->BodyCoordinates_Orientation = Matrix3D::GetRotateArbitraryAxis(Body_Coordinates->x,0.5f) * Body_Coordinates->BodyCoordinates_Orientation;
	}
	else if(Keyboard::IsPressed(KeyboardKeyI))
	{
		Body_Coordinates->BodyCoordinates_Orientation = Matrix3D::GetRotateArbitraryAxis(Body_Coordinates->x,-0.5f) * Body_Coordinates->BodyCoordinates_Orientation;
	}
		
	Update_ObjectBodyCoordinates();
	Update_ObjectMatrix(Local_ObjectTransformation);
	Update_ObjectBoundingBox(Local_ObjectTransformation,Parent->Parent_ObjectMatrix);
}
void Tank_FrontPart::Update_DynamicCollision_WithResponse(BSP_Tree *BSP)
{
	/*bool Collided=false;
	for(int i=0; i<Collision_Loader->numberOfVertices; i++)
	{
		if(!BSP->SegmentIntercept(Old_Vertices[i],New_Vertices[i],BSP->GetRoot()))
			Collided=true;
	}
	if(Collided)
		Position += Body_Coordinates->z;*/
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Tank::Tank (string Render_Poly, string Collision_Poly, Point3D position, BodyCoordinates *bodycoordinates, Vector3D orientation, Vector3D rotational_speed, Point3D scale,
float mass, float fluid_density, Vector3D velocity, bool Apply_Gravity, Constraint *_constraint) :
RigidBody(Render_Poly,Collision_Poly,position,bodycoordinates,orientation,rotational_speed,scale,mass,fluid_density,velocity,Apply_Gravity,_constraint)
{
	//Gravity_ON=OFF;
	//RotationalDrag_ON=ON;
	Attach_Child(new Tank_TopPart(app.GetDirectory() + "Resources\\Tank_Top.3DS",app.GetDirectory() + "Resources\\Tank_Top.3DS",Point3D(0,0.455f,0),new BodyCoordinates,Vector3D(0,0,0),Vector3D(0,0,0),Point3D(1,1,1)));
	Attach_Child(new Tank_TopPart(app.GetDirectory() + "Resources\\Tank_Top.3DS",app.GetDirectory() + "Resources\\Tank_Top.3DS",Point3D(0,-0.455f,0.5f),new BodyCoordinates,Vector3D(0,0,0),Vector3D(0,0,0),Point3D(1,1,1)));
	Object_Children->Attach_Child(new Tank_FrontPart(app.GetDirectory() + "Resources\\Tank_Front.3DS",app.GetDirectory() + "Resources\\Tank_Front.3DS",Point3D(0,0.15f,-0.10f),new BodyCoordinates,Vector3D(0,0,0),Vector3D(0,0,0),Point3D(1,1,1)));

	Compute_GlobalBoundingBox();
	Object_Children->Compute_GlobalBoundingBox();
}

void Tank::Custom_Loads (void)
{
	if(Keyboard::IsPressed(KeyboardKeyI))
	{
		this->Custom_Forces_Sum += Body_Coordinates->world_z*2;
		//Apply_Force(Force(World_Gravity_Center,Body_Coordinates->world_z*2));
	}
	else if(Keyboard::IsPressed(KeyboardKeyK))
	{
		this->Custom_Forces_Sum -= Body_Coordinates->world_z*2;
	}
	if(Keyboard::IsPressed(KeyboardKeyJ))
	{
		//Apply_Torque(Force(World_Gravity_Center + Body_Coordinates->world_z,Body_Coordinates->world_x*2));
	}
	else if(Keyboard::IsPressed(KeyboardKeyL))
	{
		//Apply_Torque(Force(World_Gravity_Center + Body_Coordinates->world_z,-Body_Coordinates->world_x*2));
	}
}

/* Function to update the object physics properties */
void Tank::Update_Physics (void)
{
	/* Update the Position, orientation */
	Update_RigidBody();
	/* Get the Final_WorldTransformation & the Parent_ObjectMatrix (without the scale) after getting the position ... */
	Update_ObjectMatrix(Local_ObjectTransformation);
	/* Update the body coordinates */
	Update_ObjectBodyCoordinates();
	/* Update the children */
	//Update_PhysicsBoundings(Local_ObjectTransformation);
	/* Update the character's bounding box */
	if(Parent)
		Update_ObjectBoundingBox(Local_ObjectTransformation,Parent->Parent_ObjectMatrix);
}

void Tank::Update_AI ()
{
	//if(Keyboard::IsPressed(KeyboardKeyJ))
	//{
	//	Body_Coordinates->BodyCoordinates_Orientation = Matrix3D::GetRotateArbitraryAxis(Body_Coordinates->y,1) * Body_Coordinates->BodyCoordinates_Orientation;
	//}
	//if(Keyboard::IsPressed(KeyboardKeyL))
	//{
	//	Body_Coordinates->BodyCoordinates_Orientation = Matrix3D::GetRotateArbitraryAxis(Body_Coordinates->y,-1) * Body_Coordinates->BodyCoordinates_Orientation;
	//}
	//if(Keyboard::IsPressed(KeyboardKeyU))
	//{
	//	Position += Body_Coordinates->y*0.003f;
	//}
	//if(Keyboard::IsPressed(KeyboardKeyO))
	//{
	//	Position -= Body_Coordinates->y*0.003f;
	//}
	////if(Keyboard::IsTriggered(KeyboardKeyH))
	////{
	////	//Detach_Child(1,&Game_Engine->Current_Level->Current_Area->Object_List);
	////	Detach_Child(1,&Object_Children->Object_Children);
	////}

	//if(Keyboard::IsPressed(KeyboardKeyI))
	//{
	//	Position += Body_Coordinates->z*0.003f;
	//}
	//else if(Keyboard::IsPressed(KeyboardKeyK))
	//{
	//	Position -= Body_Coordinates->z*0.003f;
	//}

	//Update_ObjectBodyCoordinates();
	//Update_ObjectMatrix(Local_ObjectTransformation);
	//if(Parent)
	//	Update_ObjectBoundingBox(Local_ObjectTransformation,Parent->Parent_ObjectMatrix);
}

//void Tank::Update_DynamicCollision_WithResponse(BSP_Tree *BSP)
//{
//	vector<Point3D> CollidedPoints;
//	Point3D Average_Point;
//
//	if(Get_CollisionPoints(BSP,CollidedPoints))
//		Apply_ImpactMomentum(Vector3D(0,1,0));
//	/*if(Collided)
//		Custom_Forces_Sum += Vector3D(1,0,0)*10;*/
//}























