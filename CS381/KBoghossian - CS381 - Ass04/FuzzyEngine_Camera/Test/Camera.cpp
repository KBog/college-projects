#include ".\camera.h"

#include "TestRenderApp.h"
#include "Game.h"
extern MyApplication app;

bool Free_Cam=false;
extern Camera *Main_Camera;
extern Game *Game_Engine;

extern Vector3D Reflection;
extern Point3D InterPoint;

//#define CAMSPEED 0.025f

void Camera::Update()
{
	//Current_ViewingMatrix = renderer->GetViewingTransformation();
	Old_ViewingMatrix = Current_ViewingMatrix;
	OldPosition= Position;
    		
	if(Free_Cam)
	{
		if(Keyboard::IsPressed(KeyboardKeyUp))
		{
			//Current_ViewingMatrix = Matrix3D::GetTranslate(0, 0, Speed) * Current_ViewingMatrix;
			Position += Speed*Body_Coordinates->z;
		}

		if(Keyboard::IsPressed(KeyboardKeyDown))
		{
			//Current_ViewingMatrix = Matrix3D::GetTranslate(0, 0, -Speed) * Current_ViewingMatrix;
			Position -= Speed*Body_Coordinates->z;
		}

		if(Keyboard::IsPressed(KeyboardKeyQ))
		{
			//Current_ViewingMatrix = Matrix3D::GetTranslate(0, -Speed, 0) * Current_ViewingMatrix;
			Position += Speed*Body_Coordinates->y;
		}

		if(Keyboard::IsPressed(KeyboardKeyA))
		{
			//Current_ViewingMatrix = Matrix3D::GetTranslate(0, Speed, 0) * Current_ViewingMatrix;
			Position -= Speed*Body_Coordinates->y;
		}

		if(Keyboard::IsPressed(KeyboardKeyZ))
		{
			//Current_ViewingMatrix = Matrix3D::GetTranslate(Speed, 0, 0) * Current_ViewingMatrix;
			Position += Speed*Body_Coordinates->x;
		}

		if(Keyboard::IsPressed(KeyboardKeyX))
		{
			//Current_ViewingMatrix = Matrix3D::GetTranslate(-Speed, 0, 0) * Current_ViewingMatrix;
			Position -= Speed*Body_Coordinates->x;
			
		}
		if(!Collided)
		{
			if(Keyboard::IsPressed(KeyboardKeyW))
			{
				//Current_ViewingMatrix = Matrix3D::GetRotateOnX(-2) * Current_ViewingMatrix;
				Body_Coordinates->BodyCoordinates_Orientation= Matrix3D::GetRotateArbitraryAxis(Body_Coordinates->x,-2) * Body_Coordinates->BodyCoordinates_Orientation;
			}

			if(Keyboard::IsPressed(KeyboardKeyS))
			{
				//Current_ViewingMatrix = Matrix3D::GetRotateOnX(2) * Current_ViewingMatrix;
				Body_Coordinates->BodyCoordinates_Orientation= Matrix3D::GetRotateArbitraryAxis(Body_Coordinates->x,2) * Body_Coordinates->BodyCoordinates_Orientation;
			}

			if(Keyboard::IsPressed(KeyboardKeyRight))
			{
				//Current_ViewingMatrix = Matrix3D::GetRotateArbitraryAxis(Body_Coordinates->y,2) * Current_ViewingMatrix;
				Body_Coordinates->BodyCoordinates_Orientation= Matrix3D::GetRotateArbitraryAxis(Body_Coordinates->y,-4) * Body_Coordinates->BodyCoordinates_Orientation;
			}

			if(Keyboard::IsPressed(KeyboardKeyLeft))
			{
				//Current_ViewingMatrix = Matrix3D::GetRotateArbitraryAxis(Body_Coordinates->y,-2) * Current_ViewingMatrix;
				Body_Coordinates->BodyCoordinates_Orientation= Matrix3D::GetRotateArbitraryAxis(Body_Coordinates->y,4) * Body_Coordinates->BodyCoordinates_Orientation;
			}

			if(Keyboard::IsPressed(KeyboardKeyC))
			{
				//Current_ViewingMatrix = Matrix3D::GetRotateOnZ(-2) * Current_ViewingMatrix;
				Body_Coordinates->BodyCoordinates_Orientation= Matrix3D::GetRotateArbitraryAxis(Body_Coordinates->z,-2) * Body_Coordinates->BodyCoordinates_Orientation;
			}

			if(Keyboard::IsPressed(KeyboardKeyV))
			{
				//Current_ViewingMatrix = Matrix3D::GetRotateOnZ(2) * Current_ViewingMatrix;
				Body_Coordinates->BodyCoordinates_Orientation= Matrix3D::GetRotateArbitraryAxis(Body_Coordinates->z,2) * Body_Coordinates->BodyCoordinates_Orientation;
			}
		}

		//if(Keyboard::IsTriggered(KeyboardKeyR))
		//	Current_ViewingMatrix = Current_ViewingMatrix.GetIdentity() * Matrix3D::GetRotateOnY(90) * Matrix3D::GetTranslate(800,-100,0);
	
		/*Matrix3D m,j;
		Matrix3D v=m*view;
		renderer->SetViewingTransformation(v);
		Matrix3D inv = Matrix3D::GetInverse4x4(v);
		Point3D pt = Point3D(0,0,0), pt1=Point3D(0,0,-20);
		pt = inv*pt;
		pt1=inv*pt1;

		if(!Game_Engine->Current_Level->Current_Area->AreaBSP->SegmentIntercept(pt,pt1,Game_Engine->Current_Level->Current_Area->AreaBSP->GetRoot()))
			renderer->SetViewingTransformation(currentViewingMatrix);*/
		/*Matrix3D v=m*view;
		renderer->SetViewingTransformation(v);
		Matrix3D inv = Matrix3D::GetInverse4x4(v);
		Point3D pt = Point3D(0,0,0), pt1=Point3D(0,0,-20);
		pt = inv*pt;
		pt1=inv*pt1;

		Position = pt;*/
	}
	else
	{	
		Current_ViewingMatrix = renderer->GetViewingTransformation();
		Old_ViewingMatrix = Current_ViewingMatrix;
		
		//Point3D TarPosUnits = Target->Position*TO_UNIT;
		//Position = TarPosUnits - Direction*Camera_Object_Distance_Z;
		//
		//Plane3D P(TarPosUnits, Vector3D(0,1,0));
		//Point3D ProjPt;
		//ProjPt = TarPosUnits + Target->Body_Coordinates->z;
		//ProjPt.ProjectionOnPlane(P,ProjPt);
		//
		//Vector3D Proj_Direction(ProjPt.X-TarPosUnits.X, ProjPt.Y-TarPosUnits.Y, ProjPt.Z-TarPosUnits.Z);
		//Proj_Direction.SetNormalize();
		//Direction += Proj_Direction;
		//Direction.SetNormalize();
		//
		//Point3D Interp_Target = Position - Direction*Camera_Object_Distance_Z*150 + Vector3D(0,1,0)*Camera_Object_Distance_Y;

		//Matrix3D Cam_Orientation;
		//Cam_Orientation.SetLookAtAsOGL(Position,Interp_Target,Vector3D(0,1,0));
		//renderer->SetViewingTransformation(Cam_Orientation);
		//Current_ViewingMatrix=Cam_Orientation;

		//Interpolation();


	}
	/* Matrix Inv_Orientation will hold the position of the object */
	Matrix3D Inv_Orientation;
	Matrix3D::GetInverse4x4(Body_Coordinates->BodyCoordinates_Orientation,Inv_Orientation);
	/* Updating the current view matrix by translating it to the Position already updated from before */
	Current_ViewingMatrix = Inv_Orientation*Matrix3D::GetTranslate(-Position.X*TO_UNIT,-Position.Y*TO_UNIT,-Position.Z*TO_UNIT);
	
	/* Update the collision of the camera VS the BSP */
	Update_CameraCollision(Game_Engine->Current_Level->Current_Area->BSP);
	/* Update the camera frustrum */
	Update_CameraFrustrum(renderer->GetViewingTransformation());
	Body_Coordinates->Update();
}

/* Function to check for the collision of the camera with the BSP */
void Camera::Update_CameraCollision (BSP_Tree *BSP)
{
	Matrix3D inv = Matrix3D::GetInverse4x4(Current_ViewingMatrix);
	Point3D pt = Point3D(0,0,0), pt1=Point3D(0,0,-5);
	pt = inv*pt;
	pt1= inv*pt1;

	//if(BSP->BSP_Collision(/*OldPosition*TO_UNIT,Position*TO_UNIT,*/pt,pt1,InterPoint,Reflection))
	//{
	//	//THE CAM MUST BE AN OBJECT SO WE OVERWRITE THE Update_CollisionResponse that will slide
	//	//Update_CollisionResponse()
	//	renderer->SetViewingTransformation(Old_ViewingMatrix);
	//	Position= OldPosition;
	//	Collided=true;
	//}
	//else
	//{
	//	InterPoint.SetXYZ(0,0,0);
	//	Reflection.SetXYZ(0,0,0);
	//	Collided=false;
		renderer->SetViewingTransformation(Current_ViewingMatrix);
	//}
}

/* Function to update the Frustrum Plane Normals according to the Viewing and Perspective Matrices */
void Camera::Update_CameraFrustrum (Matrix3D M)
{
	float Magnitude=0;
	/* Computing the new Left Plane equation then normalizing the normal */
	FrustrumPlanes[Left].A = M.Ma00 + M.Ma30;
	FrustrumPlanes[Left].B = M.Ma01 + M.Ma31;
	FrustrumPlanes[Left].C = M.Ma02 + M.Ma32;
	FrustrumPlanes[Left].D = M.Ma03 + M.Ma33;
	Magnitude=sqrtf(FrustrumPlanes[Left].A*FrustrumPlanes[Left].A + FrustrumPlanes[Left].B*FrustrumPlanes[Left].B + FrustrumPlanes[Left].C*FrustrumPlanes[Left].C);
	FrustrumPlanes[Left].A/=Magnitude;	FrustrumPlanes[Left].B/=Magnitude;	FrustrumPlanes[Left].C/=Magnitude;	FrustrumPlanes[Left].D/=Magnitude;
	/* Computing the new Right Plane equation then normalizing the normal */
	FrustrumPlanes[Right].A = -M.Ma00 + M.Ma30;
	FrustrumPlanes[Right].B = -M.Ma01 + M.Ma31;
	FrustrumPlanes[Right].C = -M.Ma02 + M.Ma32;
	FrustrumPlanes[Right].D = -M.Ma03 + M.Ma33;
	Magnitude=sqrtf(FrustrumPlanes[Right].A*FrustrumPlanes[Right].A + FrustrumPlanes[Right].B*FrustrumPlanes[Right].B + FrustrumPlanes[Right].C*FrustrumPlanes[Right].C);
	FrustrumPlanes[Right].A/=Magnitude;	FrustrumPlanes[Right].B/=Magnitude;	FrustrumPlanes[Right].C/=Magnitude;	FrustrumPlanes[Right].D/=Magnitude;
	/* Computing the new Bottom Plane equation then normalizing the normal */
	FrustrumPlanes[Bottom].A = M.Ma10 + M.Ma30;
	FrustrumPlanes[Bottom].B = M.Ma11 + M.Ma31;
	FrustrumPlanes[Bottom].C = M.Ma12 + M.Ma32;
	FrustrumPlanes[Bottom].D = M.Ma13 + M.Ma33;
	Magnitude=sqrtf(FrustrumPlanes[Bottom].A*FrustrumPlanes[Bottom].A + FrustrumPlanes[Bottom].B*FrustrumPlanes[Bottom].B + FrustrumPlanes[Bottom].C*FrustrumPlanes[Bottom].C);
	FrustrumPlanes[Bottom].A/=Magnitude;	FrustrumPlanes[Bottom].B/=Magnitude;	FrustrumPlanes[Bottom].C/=Magnitude;	FrustrumPlanes[Bottom].D/=Magnitude;
	/* Computing the new Top Plane equation then normalizing the normal */
	FrustrumPlanes[Top].A = -M.Ma10 + M.Ma30;
	FrustrumPlanes[Top].B = -M.Ma11 + M.Ma31;
	FrustrumPlanes[Top].C = -M.Ma12 + M.Ma32;
	FrustrumPlanes[Top].D = -M.Ma13 + M.Ma33;
	Magnitude=sqrtf(FrustrumPlanes[Top].A*FrustrumPlanes[Top].A + FrustrumPlanes[Top].B*FrustrumPlanes[Top].B + FrustrumPlanes[Top].C*FrustrumPlanes[Top].C);
	FrustrumPlanes[Top].A/=Magnitude;	FrustrumPlanes[Top].B/=Magnitude;	FrustrumPlanes[Top].C/=Magnitude;	FrustrumPlanes[Top].D/=Magnitude;
	/* Computing the new Back Plane equation then normalizing the normal */
	FrustrumPlanes[Back].A = M.Ma20 + M.Ma30;
	FrustrumPlanes[Back].B = M.Ma21 + M.Ma31;
	FrustrumPlanes[Back].C = M.Ma22 + M.Ma32;
	FrustrumPlanes[Back].D = M.Ma23 + M.Ma33;
	Magnitude=sqrtf(FrustrumPlanes[Back].A*FrustrumPlanes[Back].A + FrustrumPlanes[Back].B*FrustrumPlanes[Back].B + FrustrumPlanes[Back].C*FrustrumPlanes[Back].C);
	FrustrumPlanes[Back].A/=Magnitude;	FrustrumPlanes[Back].B/=Magnitude;	FrustrumPlanes[Back].C/=Magnitude;	FrustrumPlanes[Back].D/=Magnitude;
	/* Computing the new Front Plane equation then normalizing the normal */
	FrustrumPlanes[Front].A = -M.Ma20 + M.Ma30;
	FrustrumPlanes[Front].B = -M.Ma21 + M.Ma31;
	FrustrumPlanes[Front].C = -M.Ma22 + M.Ma32;
	FrustrumPlanes[Front].D = -M.Ma23 + M.Ma33;
	Magnitude=sqrtf(FrustrumPlanes[Front].A*FrustrumPlanes[Front].A + FrustrumPlanes[Front].B*FrustrumPlanes[Front].B + FrustrumPlanes[Front].C*FrustrumPlanes[Front].C);
	FrustrumPlanes[Front].A/=Magnitude;	FrustrumPlanes[Front].B/=Magnitude;	FrustrumPlanes[Front].C/=Magnitude;	FrustrumPlanes[Front].D/=Magnitude;
}

/* This function will set all the variables for the Camera Interpolation */
void Camera::Set_Interpolation(BodyCoordinates *destination_direction, Point3D destination_position)
{
	/* Setting the Camera Type to be Cam_Type_View */
	Cam_Type= Cam_Type_View;

	/* Setting the Target Position  */
	Target_Position= destination_position;
	/* Setting the Target Direction */
	Target_Direction= destination_direction;

	/* Getting the real distance from source to destination */
	Distance= Position.Distance(Target_Position);

	/* Getting the Rotation Angle between the source and destination */
	Rotation_Angle_x = ONEEIGHTY_OVER_PI*acosf(Body_Coordinates->x.GetDotProduct(Target_Direction->x));
	Rotation_Angle_y = ONEEIGHTY_OVER_PI*acosf(Body_Coordinates->y.GetDotProduct(Target_Direction->y));
	Rotation_Angle_z = ONEEIGHTY_OVER_PI*acosf(Body_Coordinates->z.GetDotProduct(Target_Direction->z));



	/*getting the cross product of the Camera with the Destination Position then the dot Product with j (World Up_Direction) */
	//float Side_Check=(Body_Coordinates->z.GetCrossProduct(Target_Direction)).GetDotProduct(Vector3D (0,1,0));
	//
	///*if the cheking is positive then the camera is to the right of the target_Position */ 
	//if (Side_Check > 0)
	//	Vector_Side= Vector_Side_Right;
	///*else the camera is to the left of the target_Position*/
	//else
	//	Vector_Side= Vector_Side_Left;

	/* Getting the small angle factor that we need to rotate the current camera direction
	to reach the destination direction */
	Rotation_Factor_x= Rotation_Angle_x/(Distance);
	Rotation_Factor_y= Rotation_Angle_y/(Distance);
	Rotation_Factor_z= Rotation_Angle_z/(Distance);

	/* Translation Vector */
	Translation_Vector = destination_position - Position;
	//Translation_Vector.SetXYZ(Target_Position.X-Position.X,Target_Position.Y-Position.Y,-(Target_Position.Z-Position.Z));
	Translation_Vector.SetNormalize();
}

/* This funciton will set all the variables for the Camera Interpolation */
void Camera::Set_Interpolation(Object *target)
{
	/* Setting the Camera Type to be Cam_Type_Follow */
	Cam_Type= Cam_Type_Follow;

	/* Setting the Target Position  */
	Target_Position= target->Position;

	/* Getting the real distance from source to destination */
	Distance= Position.Distance(Target_Position);

	/* Getting the Rotation Angle between the source and destination */
	Rotation_Angle_z= acosf(Body_Coordinates->y.GetDotProduct(target->Body_Coordinates->y));
	
	/* Translation Vector */
	Translation_Vector= target->Position - Position;
	Translation_Vector.SetNormalize();
}

/* Getting the real side of camera vector according to the 3D position in space */
Vector3D Camera::Get_3DRotationAxis(Vector3D *s, Vector3D TargetDirection)
{
	/* It's the CROSS product btw Cam & Obj */
	Vector3D *N= NULL;
	N= new Vector3D;

	if(*s!= TargetDirection)
	{
		N= &s->GetCrossProduct(TargetDirection);
		N->SetNormalize();
	}
	return *N;
}

/* Updating all angles */
void Camera::Angles_UpdateXYZ(void)
{
	/* Getting the Rotation Angle between the source and destination */
	Rotation_Angle_x = ONEEIGHTY_OVER_PI*acosf(Body_Coordinates->x.GetDotProduct(Target_Direction->x));
	Rotation_Angle_y = ONEEIGHTY_OVER_PI*acosf(Body_Coordinates->y.GetDotProduct(Target_Direction->y));
	Rotation_Angle_z = ONEEIGHTY_OVER_PI*acosf(Body_Coordinates->z.GetDotProduct(Target_Direction->z));
	
	/* Getting the new Angle Rotation Factor */
	Rotation_Factor_x= Rotation_Angle_x/(Distance);
	Rotation_Factor_y= Rotation_Angle_y/(Distance);
	Rotation_Factor_z= Rotation_Angle_z/(Distance);
}

/* Depending on the camera type we interpolate to a certain target or a position */
void Camera::Interpolation(void)
{
	switch(Cam_Type)
	{
	case Cam_Type_Follow:
		{
			//Set
			Distance= Position.Distance(Target_Position);
			//MainCharacterBox(GliCycle);
			TargetBox->Position = Position;
			//TargetBox->Position.Y -= 50;
//			TargetBox->Position.Z += TargetBox->Position.Z*Body_Coordinates->z;
			TargetBox->Position += TargetBox->Body_Coordinates->z*2 - TargetBox->Body_Coordinates->y*0.25f;
			TargetBox->Body_Coordinates = Body_Coordinates;
			TargetBox->Update_Object();
			TargetBox->Render(app.GetRenderer());
			if(Distance >= Speed)
				Position +=Translation_Vector * Speed;
			else
			{
			}
		}
		break;
	case Cam_Type_View:
		{
			//Reach certain position!
			Distance= Position.Distance(Target_Position);
			if(Distance >= Speed)
				Position +=Translation_Vector * Speed;
				
			if(Rotation_Angle_x >= Rotation_Factor_x)
			{
				/* Getting the Arbitrary 3D Axis in world Space and rotate X-axis of the camera by the shortest angle */
				Body_Coordinates->BodyCoordinates_Orientation= Matrix3D::GetRotateArbitraryAxis(Get_3DRotationAxis(&Body_Coordinates->x, Target_Direction->x),Rotation_Factor_x) * Body_Coordinates->BodyCoordinates_Orientation;
				/* Updating all Camera Angles */
				Angles_UpdateXYZ();
				/* Decrementing the X-Angle by its factor */
				Rotation_Angle_x -= Rotation_Factor_x;
			}
			if(Rotation_Angle_y >= Rotation_Factor_y)
			{
				/* Getting the Arbitrary 3D Axis in world Space and rotate Y-axis of the camera by the shortest angle */
				Body_Coordinates->BodyCoordinates_Orientation= Matrix3D::GetRotateArbitraryAxis(Get_3DRotationAxis(&Body_Coordinates->y, Target_Direction->y),Rotation_Factor_y) * Body_Coordinates->BodyCoordinates_Orientation;
				/* Updating all Camera Angles */
				Angles_UpdateXYZ();
				/* Decrementing the Y-Angle by its factor */
				Rotation_Angle_y -= Rotation_Factor_y;
			}
			if(Rotation_Angle_z >= Rotation_Factor_z)
			{
				/* Getting the Arbitrary 3D Axis in world Space and rotate Z-axis of the camera by the shortest angle */
				Body_Coordinates->BodyCoordinates_Orientation= Matrix3D::GetRotateArbitraryAxis(Get_3DRotationAxis(&Body_Coordinates->z, Target_Direction->z),Rotation_Factor_z) * Body_Coordinates->BodyCoordinates_Orientation;
				/* Updating all Camera Angles */
				Angles_UpdateXYZ();
				/* Decrementing the Z-Angle by its factor */
				Rotation_Angle_z -= Rotation_Factor_z;
			}
		}
		break;
	}
}
