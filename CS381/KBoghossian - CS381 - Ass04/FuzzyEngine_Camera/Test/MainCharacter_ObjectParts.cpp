#include "MainCharacter.h"
#include "MainCharacter_ObjectParts.h"
#include "Physics_Support.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* Front Wheel constructor */
MainCharacter_FrontWheel::MainCharacter_FrontWheel(string Render_Poly, string Collision_Poly, Point3D position, BodyCoordinates *bodycoordinates, Vector3D orientation, Vector3D rotational_speed, Point3D scale, float radius):
Radius(radius),
Object(Render_Poly,Collision_Poly,position,bodycoordinates,orientation,rotational_speed,scale)
{
}
void MainCharacter_FrontWheel::Update_AI (void) 
{
	if(!Keyboard::IsPressed(KeyboardKeyK) && ((MainCharacter *)Parent)->GliCycleState==Bicycle_State)
	{
		/* Getting the Rotation Angle according to distance traveled */
		/* d = r.teta --> teta = d/r (here d is the distance traveled)
									r is the radius of the wheel) */
		Rotational_Speed.X = ((MainCharacter *)Parent)->Delta_Distance/Radius;
		/* Converting the Angle from radians to degree */
		Rotational_Speed.X = (Rotational_Speed.X*ONEEIGHTY_OVER_PI);
		/* Rotating the wheel */
		Body_Coordinates->BodyCoordinates_Orientation = Matrix3D::GetRotateArbitraryAxis(Body_Coordinates->x,Rotational_Speed.X) * Body_Coordinates->BodyCoordinates_Orientation;					
	}
	//Body_Coordinates->BodyCoordinates_Orientation = Matrix3D::GetRotateArbitraryAxis(Body_Coordinates->x,2) * Body_Coordinates->BodyCoordinates_Orientation;					

	Update_ObjectBodyCoordinates();
	Update_ObjectMatrix(Local_ObjectTransformation);
	Update_ObjectBoundingBox(Local_ObjectTransformation,Parent->Parent_ObjectMatrix);
	/* Compute the world Position of the child */
	World_Position = (Parent->Parent_ObjectMatrix) *Position;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* Back Wheel constructor */
MainCharacter_BackWheel::MainCharacter_BackWheel(string Render_Poly, string Collision_Poly, Point3D position, BodyCoordinates *bodycoordinates, Vector3D orientation, Vector3D rotational_speed, Point3D scale, float radius):
Radius(radius),
Object(Render_Poly,Collision_Poly,position,bodycoordinates,orientation,rotational_speed,scale)
{
}
void MainCharacter_BackWheel::Update_AI (void) 
{
	if(!Keyboard::IsPressed(KeyboardKeyK) && ((MainCharacter *)Parent)->GliCycleState==Bicycle_State)
	{
		/* Getting the Rotation Angle according to distance traveled */
		/* d = r.teta --> teta = d/r (here d is the distance traveled)
									r is the radius of the wheel) */
		Rotational_Speed.X = ((MainCharacter *)Parent)->Delta_Distance/Radius;
		/* Converting the Angle from radians to degree */
		Rotational_Speed.X = (Rotational_Speed.X*ONEEIGHTY_OVER_PI);
		/* Rotating the wheel */
		Body_Coordinates->BodyCoordinates_Orientation = Matrix3D::GetRotateArbitraryAxis(Body_Coordinates->x,Rotational_Speed.X) * Body_Coordinates->BodyCoordinates_Orientation;					
	}

	Update_ObjectBodyCoordinates();
	Update_ObjectMatrix(Local_ObjectTransformation);
	Update_ObjectBoundingBox(Local_ObjectTransformation,Parent->Parent_ObjectMatrix);
	/* Compute the world Position of the child */
	World_Position = (Parent->Parent_ObjectMatrix)*Position;
}

MainCharacter_GliderTop::MainCharacter_GliderTop(string Render_Poly, string Collision_Poly, Point3D position, BodyCoordinates *bodycoordinates, Vector3D orientation, Vector3D rotational_speed, Point3D scale, float upward_gliding_ratio , float frontal_gliding_ratio):
Upward_Gliding_Ratio(upward_gliding_ratio),Frontal_Gliding_Ratio(frontal_gliding_ratio),
Object(Render_Poly,Collision_Poly,position,bodycoordinates,orientation,rotational_speed,scale)
{
}

//Vector3D MainCharacter_GliderTop::Synchronize_ChildParent (Vector3D Input)
//{
//	// MUST DO: Choose the lift factor to be proportional to all the children
//	//			Let the lift factor includes the area of the glider
//	//			Put the Lift computation in the glider synchronize but don't apply a force, just translation
//	//			Do the glider as seperate object
//	//			Adjust the I Key ???!!!
//	((MainCharacter *)Parent)->Custom_Forces_Sum += /*((MainCharacter *)Parent)->Body_Coordinates->y*/Vector3D(0,1,0)*(((MainCharacter *)Parent)->Mass*GRAVITY_VALUE*Upward_Gliding_Ratio);
//	((MainCharacter *)Parent)->Custom_Forces_Sum += ((MainCharacter *)Parent)->Body_Coordinates->world_z*(Frontal_Gliding_Ratio);
//	return Vector3D(0,0,0);
//}
void MainCharacter_GliderTop::Update_AI (void) 
{
	Update_ObjectBodyCoordinates();
	Update_ObjectMatrix(Local_ObjectTransformation);
	Update_ObjectBoundingBox(Local_ObjectTransformation,Parent->Parent_ObjectMatrix);
	/* Compute the world Position of the child */
	World_Position = (Parent->Parent_ObjectMatrix)*Position;
}