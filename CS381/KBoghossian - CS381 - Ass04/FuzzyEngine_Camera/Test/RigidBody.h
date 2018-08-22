#ifndef _RIGIDBODY_H_
#define _RIGIDBODY_H_

#include "Object.h"
#include "Physics_Support.h"
#include "Quaternion.h"
#include "ODESolver.h"
#include "Defines.h"
#include "Constraint.h"
#include <vector>

enum BodyDiagramForce_Type { Gravity_Type , Drag_Type };

#define MAX_BODYDIAGRAM_FORCES 2

extern Vector3D TEST_AOA;

/* RigidBody class */
class RigidBody : public Object
{
	public:
			/* The mass of the object according to its mesh */
			float Mass;
			/* The weight of the object */
			float Weight;
			/* Linear Velocity */
			Vector3D Velocity;
			/* this is the magnitude of the velocity vector */
			float Speed;
			/* The sum of forces */
			Vector3D Forces_Sum;
			/* The Custom Forces */
			Vector3D Custom_Forces_Sum;
			/* Array of diagram forces */
			Vector3D BodyDiagram_Forces [MAX_BODYDIAGRAM_FORCES];
			/* Vector of impulse forces applied at runtime on the Gravity Center of the object */
			vector<Force> Impulse_Forces;
			/* Time Step */
			float Time_Step;
			/* Density of the fluid */
			float Fluid_Density;	//Air: 1.29 kg/m^3
			/* Coefficient of aerodynamic of the object */
			float Cd;
			/* Dimensions */
			float Length, Width, Height;
			/* Constraint */
			Constraint *constraint;

			float CoefficientOfRestitution;
			bool Gravity_ON, Drag_ON;
			
			/* Default Constructor */
			RigidBody():Object()
			{
			}

			/* RigidBody Constructor */
			RigidBody (string Render_Poly, string Collision_Poly, Point3D position, BodyCoordinates *bodycoordinates, Vector3D orientation, Vector3D rotational_speed, Point3D scale, 
					   float mass, float fluid_density, Vector3D velocity, bool Apply_Gravity=true, Constraint *_constraint=NULL, float coefficientofrestitution=0.3f) :
			Mass(mass), Fluid_Density(fluid_density), Velocity(velocity), constraint(_constraint), CoefficientOfRestitution(coefficientofrestitution), Gravity_ON(ON), Drag_ON(ON),
			Object(Render_Poly,Collision_Poly,position,bodycoordinates,orientation,rotational_speed,scale)
			{
				/* Setting the TimeStep = 1/60 game loops */
				Time_Step=0.016667f;

				/* Computing the weight in N */
				Weight = Mass*GRAVITY_VALUE;

				Speed=0;
				
				/* Apply the gravity if true */
				Apply_BodyDiagramForce(Gravity_Type , Apply_Gravity);

				/* initializing the forces */
				Forces_Sum.SetXYZ(0,0,0);

				/* Compute the height and width according to the initial bounding box dimensions */
				Height = Vector3D(BBox.Initial_BBox.ptE.X - BBox.Initial_BBox.ptA.X , BBox.Initial_BBox.ptE.Y - BBox.Initial_BBox.ptA.Y , BBox.Initial_BBox.ptE.Z - BBox.Initial_BBox.ptA.Z ).GetLength() * Scale.Y;
				Width  = Vector3D(BBox.Initial_BBox.ptB.X - BBox.Initial_BBox.ptA.X , BBox.Initial_BBox.ptB.Y - BBox.Initial_BBox.ptA.Y , BBox.Initial_BBox.ptB.Z - BBox.Initial_BBox.ptA.Z ).GetLength() * Scale.X;
			}
			/* A function to turn on and off the Exterior Forces */
			void Apply_BodyDiagramForce(int Force_Type, bool On_Off)
			{	                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        
				switch(Force_Type)
				{
					/* Apply the gravity at the center of mass */
					case Gravity_Type:
						BodyDiagram_Forces[Gravity_Type] = (On_Off)?(Vector3D(0,-1,0)*Weight):Vector3D(0,0,0);
						break;
					/* Apply the drag at the center of mass */
					case Drag_Type:
						{
							if(On_Off)
							{
								/* Compute the direction-normalized of the velocity vector */
								Vector3D Drag_Dir;
								float Surface_Area=0;
								if(Speed)
								{
									Drag_Dir.SetXYZ(-Velocity.X/Speed,-Velocity.Y/Speed,-Velocity.Z/Speed);
									//Cd = 0.5f;//(0.0742f)/(float)pow((1/(1.5f*(float)pow(10,-5)))*Speed*Length,0.2f);
									Surface_Area = BBox.BBox_Width*BBox.BBox_Height;
								}
								
								/* Applying the translational Drag */
								BodyDiagram_Forces[Drag_Type] = Drag_Dir*(0.5f*Fluid_Density*Cd*Speed*Speed*Surface_Area);
							}
							else
								BodyDiagram_Forces[Drag_Type] = Vector3D(0,0,0);
						}
						break;
				}
			}

			/* A function to apply a force only once (in the world coordinates) */
			void Apply_Force (Force F)
			{
				/* Adding the force to the list */
				Impulse_Forces.push_back(F);
			}

			/* Function to compute loads on an object like MainCharacter and Enemy */
			virtual void Custom_Loads (void){}
			
			void Update_DiagramForces (void) 
			{
				Apply_BodyDiagramForce(Drag_Type, Drag_ON);
				//if(Keyboard::IsTriggered(KeyboardKeyY))
				Apply_BodyDiagramForce(Gravity_Type, Gravity_ON);
			}

			void Compute_AllForces (void)
			{
				/* Adding all the exterior forces to the forces sum */
				for(int i=0; i<MAX_BODYDIAGRAM_FORCES; i++)
					Forces_Sum += BodyDiagram_Forces[i];
				/* Adding the impulse forces to the linear and angular forces */
				for(unsigned int j=0; j<Impulse_Forces.size(); j++)
					Forces_Sum += Impulse_Forces[j].force;
				
				/* Adding the Forces Sum and Torques Sum to the Custom Forces
				   and Torques respectively */
				Forces_Sum+=Custom_Forces_Sum;

				/* Apply the constraint on the forces if there exist a constraint for the body */
				/*if(constraint)
				{
					float angle_x = Compute_Angle_AccordingTo_Plane(Body_Coordinates->z,Plane3D(World_Position,Vector3D(0,1,0)));
					float angle_y = Compute_Angle_AccordingTo_Plane(Body_Coordinates->y,Plane3D(World_Position,Vector3D(1,0,0)));
					float angle_z = Compute_Angle_AccordingTo_Plane(Body_Coordinates->x,Plane3D(World_Position,Vector3D(0,1,0)));
					
					constraint->Apply_Constraint(Forces_Sum,Torques_Sum);
					Torques_Sum += constraint->Apply_AnglesConstraint(angle_x , angle_y , angle_z ,
																	  Body_Coordinates->x,Body_Coordinates->y,Body_Coordinates->z,
																	  Angular_Velocity,
																	  Inv_Inertia_Tensor.Ma00,Inv_Inertia_Tensor.Ma11,Inv_Inertia_Tensor.Ma22,
																	  Torques_Sum); 

				}*/

			}
			
			/* Update the Rigid Body position and orientation */
			void Update_RigidBody (void)
			{ 
				/* Computing the custom forces accroding to each object */
				Custom_Loads();
				/* Update the diagram forces */
				Update_DiagramForces();
				
					/* Re-Initializing all the Forces */
					Forces_Sum.SetXYZ(0,0,0);
					
					/* Getting the Sum of forces and Torques */
					Compute_AllForces();

					/* Computing the acceleration vector */
					Vector3D Acceleration = Forces_Sum/Mass;
					/* Computing the Velocity vector */
					Velocity = Acceleration*Time_Step + Velocity;
					/* Computing the new position */
					Position = 0.5f*Acceleration*Time_Step*Time_Step + Velocity*Time_Step + Position;

				/* Updating the body speed according to the new velocity */
				Speed = Velocity.GetLength();
				/* Clearing all the Impulse forces */
				Impulse_Forces.clear();
				/* Re-Initializing all the Custom Forces */
				Custom_Forces_Sum.SetXYZ(0,0,0);
				World_Position = Position;
			}

			/* Update the dynamic collision */
			//void Update_DynamicCollision_WithResponse(Object *object)
			//{
			//	vector<Point3D> CollidedPoints, CollidedFacesPoints;
			//	vector<Vector3D> CollidedFacesNormals;
			//	Point3D Body1_Average_Point, Body2_Average_Point;
			//	Vector3D Average_Normal;
			//	Vector3D Body1_Velocity, Body2_Velocity;

			//	/* Take the 2nd rigid body */
			//	RigidBody *Body2 = (RigidBody *)object;

			//	/* Check if there is collision, then store the collided points in the vector */
			//	if(Get_CollisionPoints(object->BSP,CollidedPoints,CollidedFacesPoints,CollidedFacesNormals))
			//	{
			//		/* Loop through all the collided points to compute the average point */
			//		for(unsigned int p=0; p<CollidedPoints.size(); p++)
			//			Body1_Average_Point += CollidedPoints[p];
			//		Body1_Average_Point = (Body1_Average_Point/(float)CollidedPoints.size())*TO_METERS;

			//		/* Loop through all the collided points to compute the average point */
			//		for(unsigned int p=0; p<CollidedFacesPoints.size(); p++)
			//			Body2_Average_Point += CollidedFacesPoints[p];
			//		Body2_Average_Point = (Body2_Average_Point/(float)CollidedFacesPoints.size())*TO_METERS;

			//		/* Loop through all the face normals to compute the average */
			//		for(unsigned int n=0; n<CollidedFacesNormals.size(); n++)
			//			Average_Normal += CollidedFacesNormals[n];
			//		Average_Normal = Average_Normal/(float)CollidedFacesNormals.size();
			//		Average_Normal.SetNormalize();

			//		//TESTING
			//		Average_Normal = Vector3D(0,1,0);

			//		/* Compute the vertex velocity of body1 */
			//		Body1_Velocity = Compute_RelativeVelocity(Body1_Average_Point);

			//		if(Body2->Mass)
			//		{
			//			/* Compute the vertex velocity of body2 */
			//			Body2_Velocity = Compute_RelativeVelocity(Body2_Average_Point);

			//			/* Compute the relative velocity */
			//			float Relative_Velocity = (Body1_Velocity - Body2_Velocity).GetDotProduct(Average_Normal);

			//			/* Compute the magnitude of the impact force */
			//			float Impact_Mag = -((1.0f+CoefficientOfRestitution)*Relative_Velocity)/(1/Mass + 1/Body2->Mass);

			//			/* Apply a force to Body1 using the av normal */
			//			Apply_Force(Force(Body1_Average_Point,Impact_Mag*Average_Normal));
			//			/* Apply a force to Body2 using the av normal but with an opposite direction */
			//			Body2->Apply_Force(Force(Body2_Average_Point,-Impact_Mag*Average_Normal));
			//		}
			//		else
			//		{
			//			/* Compute the vertex velocity of body2 */
			//			Body2_Velocity = Vector3D(0,0,0);

			//			/* Compute the relative velocity */
			//			float Relative_Velocity = (Body1_Velocity - Body2_Velocity).GetDotProduct(Average_Normal);

			//			/* Compute the magnitude of the impact force */
			//			float Impact_Mag = -((1.0f+CoefficientOfRestitution)*Relative_Velocity)/(1/Mass);

			//			/* Apply a force to Body1 using the av normal */
			//			Apply_Force(Force(Body1_Average_Point,Impact_Mag*Average_Normal));
			//			//Apply_Force(Force(World_Gravity_Center,Impact_Mag*Average_Normal));
			//		}
			//	}
			//}
};

#endif














