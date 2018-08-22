#include "MainCharacter_Parts.h"
#include "MainCharacter.h"


//void MainCharacter_FrontWheel::Update_Physics (void)
//{
//	/* Update the Position, orientation */
//	Update_RigidBody();
//	/* Get the Final_WorldTransformation after getting the position ... */
//	Update_PhysicsObjectMatrix(Local_ObjectTransformation);
//	/* Update the body coordinates */
//	Update_PhysicsBodyCoordinates();
//	/* Update the children */
//	Update_PhysicsBoundings(Local_ObjectTransformation,Parent->Parent_ObjectMatrix);
//	/* Update the character's bounding box */
//	Update_ObjectBoundingBox(Local_ObjectTransformation,Parent->Parent_ObjectMatrix);
//}
//
//void MainCharacter_FrontWheel::Custom_Loads (void) 
//{
//	if(Keyboard::IsPressed(KeyboardKeyU))
//	{
//		Vector3D Dir = (Vector3D(1,0,0));
//		Dir.SetNormalize();
//		Apply_Force(Force(World_Gravity_Center+Body_Coordinates->z*0.5f,Dir*100));
//	}
//	else if(Keyboard::IsTriggered(KeyboardKeyY))
//	{
//		Vector3D Dir = (Vector3D(-1,0,0));
//		Dir.SetNormalize();
//		Apply_Force(Force(Gravity_Center+Body_Coordinates->z*0.5f,Dir*1000));
//	}
//	//if(!Keyboard::IsPressed(KeyboardKeyK) && ((MainCharacter *)Parent)->GliCycleState==Bicycle_State)
//	//{
//	//	/* Getting the Rotation Angle according to distance traveled */
//	//	/* d = r.teta --> teta = d/r (here d is the distance traveled)
//	//								r is the radius of the wheel) */
//	//	Rotational_Speed.X = ((MainCharacter *)Parent)->Delta_Distance/((Physics_Cylinder *)PhysicsBounding)->Radius;
//	//	/* Converting the Angle from radians to degree */
//	//	Rotational_Speed.X = (Rotational_Speed.X*180/3.1415f);
//	//	/* Rotating the wheel */
//	//	Body_Coordinates->BodyCoordinates_Orientation = Matrix3D::GetRotateArbitraryAxis(Body_Coordinates->x,Rotational_Speed.X) * Body_Coordinates->BodyCoordinates_Orientation;					
//	//}
//
//	//if(Keyboard::IsPressed(KeyboardKeyL) && ((MainCharacter *)Parent)->GliCycleState==Bicycle_State)
//	//{
//	//	Right_WasPressed=true;
//	//	Left_WasPressed=false;
//
//	//	/* Limiting the Y rotation of the Wheel */
//	//	if(Orientation.Y>-MAX_FRONTWHEEL_YAW_ANGLE)
//	//	{
//	//		Orientation.Y -= INTERP_FRONTWHEEL_YAW_ANGLE_P;
//	//		Rotational_Speed.Y=INTERP_FRONTWHEEL_YAW_ANGLE_P;
//	//		/* Rotating the wheel to the right */
//	//		Body_Coordinates->BodyCoordinates_Orientation = Matrix3D::GetRotateArbitraryAxis(Parent->Body_Coordinates->y,-Rotational_Speed.Y) * Body_Coordinates->BodyCoordinates_Orientation;					
//	//	}
//	//}
//	//else if(Keyboard::IsPressed(KeyboardKeyJ) && ((MainCharacter *)Parent)->GliCycleState==Bicycle_State)
//	//{
//	//	Right_WasPressed=false;
//	//	Left_WasPressed=true;
//
//	//	/* Limiting the Y rotation of the Wheel */
//	//	if(Orientation.Y<MAX_FRONTWHEEL_YAW_ANGLE)
//	//	{
//	//		Orientation.Y += INTERP_FRONTWHEEL_YAW_ANGLE_P;
//	//		Rotational_Speed.Y=INTERP_FRONTWHEEL_YAW_ANGLE_P;
//	//		/* Rotating the wheel to the left */
//	//		Body_Coordinates->BodyCoordinates_Orientation = Matrix3D::GetRotateArbitraryAxis(Parent->Body_Coordinates->y,Rotational_Speed.Y) * Body_Coordinates->BodyCoordinates_Orientation;					
//	//	}
//	//}
//	///* Return the Wheel to the initial Position (here if the true bicycle won't be automatically returned we remove it) */
//	//else
//	//{
//	//	/* If the right key was pressed */
//	//	if(Right_WasPressed)
//	//	{
//	//		/* If the wheel was to the right */
//	//		if(Orientation.Y<0)
//	//		{
//	//			Rotational_Speed.Y=INTERP_FRONTWHEEL_YAW_ANGLE_N;
//	//			Orientation.Y += INTERP_FRONTWHEEL_YAW_ANGLE_N;
//	//			/* Return it to the left */
//	//			Body_Coordinates->BodyCoordinates_Orientation = Matrix3D::GetRotateArbitraryAxis(Parent->Body_Coordinates->y,Rotational_Speed.Y) * Body_Coordinates->BodyCoordinates_Orientation;
//	//		}
//	//		/* If the wheel was to the left */
//	//		else if(Orientation.Y>0)
//	//		{
//	//			Rotational_Speed.Y=INTERP_FRONTWHEEL_YAW_ANGLE_N;
//	//			Orientation.Y -= INTERP_FRONTWHEEL_YAW_ANGLE_N;
//	//			/* Return it to the right */
//	//			Body_Coordinates->BodyCoordinates_Orientation = Matrix3D::GetRotateArbitraryAxis(Parent->Body_Coordinates->y,-Rotational_Speed.Y) * Body_Coordinates->BodyCoordinates_Orientation;
//	//		}
//	//		/* When the wheel has reached its initial orientation --> reset the variable */
//	//		else
//	//			Right_WasPressed=false;
//	//	}
//	//	/* If the left key was pressed */
//	//	if(Left_WasPressed)
//	//	{
//	//		/* If the wheel was to the left */
//	//		if(Orientation.Y>0)
//	//		{
//	//			Rotational_Speed.Y=INTERP_FRONTWHEEL_YAW_ANGLE_N;
//	//			Orientation.Y -= INTERP_FRONTWHEEL_YAW_ANGLE_N;
//	//			/* Return it to the right */
//	//			Body_Coordinates->BodyCoordinates_Orientation = Matrix3D::GetRotateArbitraryAxis(Parent->Body_Coordinates->y,-Rotational_Speed.Y) * Body_Coordinates->BodyCoordinates_Orientation;
//	//		}
//	//		/* If the wheel was to the right */
//	//		else if(Orientation.Y<0)
//	//		{
//	//			Rotational_Speed.Y=INTERP_FRONTWHEEL_YAW_ANGLE_N;
//	//			Orientation.Y += INTERP_FRONTWHEEL_YAW_ANGLE_N;
//	//			/* Return it to the left */
//	//			Body_Coordinates->BodyCoordinates_Orientation = Matrix3D::GetRotateArbitraryAxis(Parent->Body_Coordinates->y,Rotational_Speed.Y) * Body_Coordinates->BodyCoordinates_Orientation;
//	//		}
//	//		/* When the wheel has reached its initial orientation --> reset the variable */
//	//		else
//	//			Left_WasPressed=false;
//	//	}
//	//}
//}

///* Behavior of the front wheel */
//void MainCharacter_BackWheel::Update_Physics (void) 
//{
//	/* If we are in the bicycle state --> Rotate the Back wheel according to the distance traveled */
//	if(((MainCharacter *)Parent)->GliCycleState==Bicycle_State)
//	{
//		if(!Keyboard::IsPressed(KeyboardKeyK))
//		{
//			/* Getting the Rotation Angle according to distance traveled */
//			/* d = r.teta --> teta = d/r (here d is the distance traveled)
//										r is the radius of the wheel) */
//			Rotational_Speed.X = ((MainCharacter *)Parent)->Delta_Distance/Radius;
//			/* Converting the Angle from radians to degree */
//			Rotational_Speed.X = (Rotational_Speed.X*180/3.1415f);
//			/* Rotating the wheel */
//			Body_Coordinates->BodyCoordinates_Orientation = Matrix3D::GetRotateArbitraryAxis(Body_Coordinates->x,Rotational_Speed.X) * Body_Coordinates->BodyCoordinates_Orientation;					
//		}
//	}
//	/* We are in the Glider mode */
//	else
//	{
//		/* If the user has pressed the front key --> rotate the wheel with an increasing angle */
//		if(Keyboard::IsPressed(KeyboardKeyI))
//		{
//			if(Rotational_Speed.X<GLIDER_BACKWHEEL_MAX_PITCH)
//				Rotational_Speed.X += GLIDER_BACKWHEEL_PITCH_RATIO;
//			Body_Coordinates->BodyCoordinates_Orientation = Matrix3D::GetRotateArbitraryAxis(Body_Coordinates->x,Rotational_Speed.X) * Body_Coordinates->BodyCoordinates_Orientation;					
//		}
//		/* The user has released the key --> rotate with the same direction while decreasing the rotation ratio */
//		else
//		{
//			if(Rotational_Speed.X>0)
//				Rotational_Speed.X -= GLIDER_BACKWHEEL_PITCH_RATIO;
//			Body_Coordinates->BodyCoordinates_Orientation = Matrix3D::GetRotateArbitraryAxis(Body_Coordinates->x,Rotational_Speed.X) * Body_Coordinates->BodyCoordinates_Orientation;					
//		}
//	}
//
//
//	Update_ObjectMatrix();
//	Final_ObjectTransformation = Parent->Final_ObjectTransformation*Final_ObjectTransformation;
//	Update_ObjectBodyCoordinates();
//	Update_ObjectBoundingBox();
//}