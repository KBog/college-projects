#pragma once
#include "RigidBody.h"
#include "Bsp.h"
#include "Support_Functions.h"

enum FP { Left, Right, Bottom, Top, Back, Front };
enum CamType {Cam_Type_View, Cam_Type_Follow };
//enum VectorSide {Vector_Side_Left , Vector_Side_Right};

class Camera
{
	public:
			Renderer * renderer;

			BoundingBox* CamBox;
			Object* TargetBox;

			CamType Cam_Type;
			BodyCoordinates *Body_Coordinates;
			Point3D Position, OldPosition;
			float Speed;
			
			/*Interpolation Variables*/
			Object *Target;
			float Rotation_Factor_x;
			float Rotation_Factor_y;
			float Rotation_Factor_z;

			float Rotation_Angle_x;
			float Rotation_Angle_y;
			float Rotation_Angle_z;

			float Rotation_Delay;
			float Translation_Factor;
			float Translation_Delay;
			float Distance;

			Vector3D Translation_Vector;
			Point3D Target_Position;
			BodyCoordinates* Target_Direction;

			/* Camera Frustrum Plane Normals */
			Plane3D FrustrumPlanes[6];
			Matrix3D Current_ViewingMatrix, Old_ViewingMatrix;
			bool Collided;


			/*Default Constructor that takes the Direction of the camera*/
			Camera(BodyCoordinates *body_coordinates, Point3D position, float speed)
			{
				Speed=speed;
				Collided=false;
				Position= OldPosition= position;
				Old_ViewingMatrix = app.GetRenderer()->GetViewingTransformation();
				renderer = app.GetRenderer();
				Body_Coordinates = body_coordinates;
				TargetBox= new Object(app.GetDirectory() + "Resources\\Tank_Bottom.3DS","NO_3DS",Point3D(0,0,0),new BodyCoordinates,Vector3D(0,0,0),Vector3D(0,0,0),Point3D(1,1,1));
			}

			void Set_Interpolation(Object *target);
			void Set_Interpolation(BodyCoordinates* destination_direction, Point3D destination_position);
			void Interpolation (void);
			void Update (void);
			void Update_CameraFrustrum (Matrix3D M);
			void Update_CameraCollision (BSP_Tree *BSP);
			Vector3D Get_3DRotationAxis(Vector3D *s, Vector3D TargetDirection);
			void Angles_UpdateXYZ(void);
			void MainCharacterBox(Object *Character);
			void Draw_BoundingBox (void);
};
