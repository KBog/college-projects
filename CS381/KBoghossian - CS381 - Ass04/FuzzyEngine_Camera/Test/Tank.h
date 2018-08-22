#ifndef _TANK_H_
#define _TANK_H_

#include "RigidBody.h"

class Tank_TopPart : public Object
{
	public:
			Tank_TopPart(string Render_Poly, string Collision_Poly, Point3D position, BodyCoordinates *bodycoordinates, Vector3D orientation, Vector3D rotational_speed, Point3D scale):
			Object(Render_Poly,Collision_Poly,position,bodycoordinates,orientation,rotational_speed,scale)
			{
			}

			void Update_AI ();
			void Update_DynamicCollision_WithResponse(BSP_Tree *BSP);
};

class Tank_FrontPart : public Object
{
	public:
			Tank_FrontPart(string Render_Poly, string Collision_Poly, Point3D position, BodyCoordinates *bodycoordinates, Vector3D orientation, Vector3D rotational_speed, Point3D scale):
			Object(Render_Poly,Collision_Poly,position,bodycoordinates,orientation,rotational_speed,scale)
			{
			}
			void Update_AI ();
			void Update_DynamicCollision_WithResponse(BSP_Tree *BSP);
};

class Tank : public RigidBody
{
	public:
			Tank (string Render_Poly, string Collision_Poly, Point3D position, BodyCoordinates *bodycoordinates, Vector3D orientation, Vector3D rotational_speed, Point3D scale, float mass, float fluid_density, Vector3D velocity, bool Apply_Gravity, Constraint *_constraint);
			void Update_Physics (void);
			void Update_AI ();
			void Update_DiagramForces (void);
			void Custom_Loads();
			void Update_DynamicCollision_WithResponse(BSP_Tree *BSP);
};

#endif