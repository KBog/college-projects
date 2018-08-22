#ifndef _PHYSICS_SUPPORT_H_
#define _PHYSICS_SUPPORT_H_


#include "TestRenderApp.h"

enum Physics_Bounding_Type { PhysicsBox_Type, PhysicsCylinder_Type, PhysicsSphere_Type };

/* Material properties class */
class Material_Properties
{
	public:
			/* material's density */
			float Density;
			/* material's Static friction */
			float Static_Friction;
			/* material's Kinetic friction */
			float Kinetic_Friction;
			/* Volume */
			float Volume;
			
			/* Default Constructor */
			Material_Properties ();

			/* Material properties constructor (Volume=-1 to know whether we must compute it or send it by paramter) */
			Material_Properties(float density, float sfriction=1.0f, float dfriction=1.0f, float volume=-1.0f);
};

/* Force class */
class Force
{
	public:
			/* Position of the force */
			Point3D Position;
			/* The force itself */
			Vector3D force;
			/* Force Constructor */
			Force (Point3D position, Vector3D force_) : Position(position), force(force_) {}
			/* Force Default Constructor */
			Force () : Position(Vector3D(0,0,0)), force(Vector3D(0,0,0)) {}
};

/* Class global physics */
class Physics_Bounding
{
	public:	
			/* Center */
			Point3D Center, World_Center;
			/* Mass Properties */
			float Mass;
			/* Material of the physics bounding */
			Material_Properties MaterialProperties;
			/* Inertia and Inv Inertia tensor */
			Matrix3D Inertia_Tensor, Inv_Inertia_Tensor;
			/* Max length between the dimensions */
			float Chord;
			
			/* Default constructor */
			Physics_Bounding ()
			{
				Mass=1;
				Inertia_Tensor.SetIdentity();
				Inv_Inertia_Tensor.SetIdentity();
			}
			/* Physics Bounding Constructor */
			Physics_Bounding (Point3D center, Material_Properties materialproperties):
			Center(center),MaterialProperties(materialproperties)
			{
			}
			/* Update the vertices to the world space */
			virtual void Update (Matrix3D WorldTransformation)
			{
			}
			/* Draw the bounding type */
			virtual void Draw (void)
			{
			}
			
};

/* Class Physics Box */
class Physics_Box : public Physics_Bounding
{
	public:
			/* Box dimensions */
			float Width, Height, Depth;
			/* Box 8 Points */
			/* Priority: Y (Top/Bottom), Z (Front/Back), X (Right/Left) */
			Point3D Pt_TFR, Pt_TBR, Pt_TBL, Pt_TFL, 
					Pt_BFR, Pt_BBR, Pt_BBL, Pt_BFL;
			Vertex V[8];
			float Area_FrontBack, Area_LeftRight, Area_TopBottom;

			/* Default constructor */
			Physics_Box():Physics_Bounding()
			{
				Width=Height=Depth=0;
			}
			/* Box Constructor */
			Physics_Box (Point3D center, Material_Properties materialproperties, float width, float height, float depth);

			/* Function to update the vertices to the world space */
			void Update (Matrix3D WorldTransformation);

			/* Draw the shape according to the vertices in the space */
			void Draw (void);

			/* Function to get the projected area of the box according to the current velocity vector */
			float Get_ProjectedArea (Vector3D Direction);
};

/* Class Physics Cylinder */
class Physics_Cylinder : public Physics_Bounding
{
	public:
			/* Cylinder dimensions */
			float Radius, Height;
			/* Cyliner 8 Points for rendering */
			Point3D Pt_FT, Pt_FMR, Pt_FB, Pt_FML,
					Pt_BT, Pt_BMR, Pt_BB, Pt_BML;
			Vertex V[8];

			/* Cylinder constructor */
			Physics_Cylinder (Point3D center, Material_Properties materialproperties, float radius, float height, Point3D Orientation);

			/* Function to update the vertices to the world space */
			void Update (Matrix3D WorldTransformation);

			/* Draw the shape according to the vertices in the space */
			void Draw (void);
};

class Physics_Sphere : public Physics_Bounding
{
	public:
			/* Sphere Dimension */
			float Radius;
			/* Sphere 6 points */
			Point3D Pt_Top, Pt_Bottom, Pt_Front, Pt_Back, Pt_Left, Pt_Right;
			Vertex V[6];

			/* Sphere constructor */
			Physics_Sphere (Point3D center, Material_Properties materialproperties, float radius);

			/* Function to update the vertices to the world space */
			void Update (Matrix3D WorldTransformation);

			/* Draw the shape according to the vertices in the space */
			void Draw (void);
};

#endif