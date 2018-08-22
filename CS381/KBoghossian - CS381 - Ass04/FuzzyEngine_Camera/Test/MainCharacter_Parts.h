#ifndef _MAINCHARACTER_PARTS_H_
#define _MAINCHARACTER_PARTS_H_

#include "RigidBody.h"

enum Children_Names { Front_Wheel , Back_Wheel };

/* MainCharacter_Part Class */
class MainCharacter_Part : public RigidBody
{
	public:
			/* A pointer to hold its parent */
			Object *Parent;

			/* Object_Part constructor */
			MainCharacter_Part (string Render_Poly, string Collision_Poly, Point3D position, BodyCoordinates *bodycoordinates, Vector3D orientation, Vector3D rotational_speed, Point3D scale,
								float mass, float fluid_density, Vector3D velocity, bool Apply_Gravity=true, Constraint *_constraint=NULL) :
			RigidBody(Render_Poly,Collision_Poly,position,bodycoordinates,orientation,rotational_speed,scale,mass,fluid_density,velocity,Apply_Gravity,_constraint)
			{
			}

			virtual void Update_AI (void) 
			{
			}
			virtual void Update_Physics (void) 
			{
			}
			virtual void Update_Effect (void)
			{
			}
};

///* Front Wheel class */
//class MainCharacter_FrontWheel : public MainCharacter_Part
//{
//	public:
//			bool Right_WasPressed;
//			bool Left_WasPressed;
//
//			/* Front Wheel constructor */
//			MainCharacter_FrontWheel(string s, Point3D position, BodyCoordinates *bodycoordinates, Vector3D orientation, Vector3D rotational_speed, Point3D scale,
//									 float fluid_density, Vector3D velocity, Vector3D angular_velocity, Physics_Bounding *physicsbounding, Physics_Bounding_Type physicsbounding_type, int physicsbounding_partitionnumber=1, int Solver_Class=Runge_Kutta_N4_Method, bool Apply_Gravity=ON, int accuracy=1, Constraint *_constraint=NULL):
//			MainCharacter_Part(s,position,bodycoordinates,orientation,rotational_speed,scale,fluid_density,velocity,angular_velocity,physicsbounding,physicsbounding_type,physicsbounding_partitionnumber,Solver_Class,Apply_Gravity,accuracy,_constraint)
//			{
//			}
//
//			/* Function to update the behavior of the character with its children */
//			void Custom_Loads (void);
//
//			/* Function to update the object physics properties */
//			void Update_Physics (void);
//
//			void Update_DiagramForces (void) 
//			{
//				Apply_BodyDiagramForce(Drag_Type, ON);
//			}
//};

///* Back Wheel class */
//class MainCharacter_BackWheel : public MainCharacter_Part
//{
//	public:
//			/* Wheel's Radius in meters */
//			float Radius;
//
//			/* Front Wheel constructor */
//			MainCharacter_BackWheel(float radius, string s, Point3D position, BodyCoordinates *bodycoordinates, Vector3D orientation, Vector3D rotational_speed, Point3D scale,
//									float fluid_density, Vector3D velocity, Vector3D angular_velocity, Physics_Bounding *physicsbounding, Physics_Bounding_Type physicsbounding_type, int physicsbounding_partitionnumber=1, int Solver_Class=Runge_Kutta_N4_Method, bool Apply_Gravity=true, int accuracy=1):
//			Radius(radius),
//			MainCharacter_Part(s,position,bodycoordinates,orientation,rotational_speed,scale,fluid_density,velocity,angular_velocity,physicsbounding,physicsbounding_type,physicsbounding_partitionnumber,Solver_Class,Apply_Gravity,accuracy)
//			{
//			}
//
//			/* Behavior of the front wheel */
//			void Update_Physics (void);
//};

#endif