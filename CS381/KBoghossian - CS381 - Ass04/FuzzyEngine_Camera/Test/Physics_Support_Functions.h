#ifndef _PHYSICS_SUPPORT_FUNCTIONS_H_
#define _PHYSICS_SUPPORT_FUNCTIONS_H_


#define GRAVITY_VALUE 9.8f
//1 Unit --> 0.025m
#define TO_UNIT 40	
#define TO_METERS 0.025f

#include "TestRenderApp.h"

enum Physics_Bounding_Type { PhysicsBox_Type, PhysicsCylinder_Type, PhysicsSphere_Type };

/* Class global physics */
class Physics_Bounding
{
	public:	
			/* Center */
			Point3D Center, World_Center;
			/* Mass Properties */
			float Mass, Volume, Density;
			/* Inertia and Inv Inertia tensor */
			Matrix3D Inertia_Tensor, Inv_Inertia_Tensor;
			
			/* Default constructor */
			Physics_Bounding ()
			{
				Mass=Volume=Density=1;
				Inertia_Tensor.SetIdentity();
				Inv_Inertia_Tensor.SetIdentity();
			}
			/* Physics Bounding Constructor */
			Physics_Bounding (Point3D center, float density):
			Center(center),Density(density)
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

			/* Default constructor */
			Physics_Box():Physics_Bounding()
			{
				Width=Height=Depth=0;
			}
			/* Box Constructor */
			Physics_Box (Point3D center, float density, float width, float height, float depth) :
			Width(width),Height(height),Depth(depth),
			Physics_Bounding(center,density)
			{
				/* Compute the half dimensions */
				float HalfWidth=Width/2, HalfHeight=Height/2, HalfDepth=Depth/2;
				/* Top Rectangle */
				Pt_TFR.SetXYZ(Center.X+HalfWidth, Center.Y+HalfHeight, Center.Z+HalfDepth);  
				Pt_TBR.SetXYZ(Center.X+HalfWidth, Center.Y+HalfHeight, Center.Z-HalfDepth);  
				Pt_TBL.SetXYZ(Center.X-HalfWidth, Center.Y+HalfHeight, Center.Z-HalfDepth);  
				Pt_TFL.SetXYZ(Center.X-HalfWidth, Center.Y+HalfHeight, Center.Z+HalfDepth);  
				/* Bottom Rectangle */
				Pt_BFR.SetXYZ(Center.X+HalfWidth, Center.Y-HalfHeight, Center.Z+HalfDepth);  
				Pt_BBR.SetXYZ(Center.X+HalfWidth, Center.Y-HalfHeight, Center.Z-HalfDepth);  
				Pt_BBL.SetXYZ(Center.X-HalfWidth, Center.Y-HalfHeight, Center.Z-HalfDepth);  
				Pt_BFL.SetXYZ(Center.X-HalfWidth, Center.Y-HalfHeight, Center.Z+HalfDepth);  

				/* Compute Mass */
				Volume = Width*Height*Depth;
				Mass = Volume*Density;
				/* Compute Inertia Tensor */
				Inertia_Tensor.SetIdentity();
				Inv_Inertia_Tensor.SetIdentity();
				float Height_Sq=Height*Height, Width_Sq=Width*Width, Depth_Sq=Depth*Depth;
				Inertia_Tensor.Ma00 = 0.083333f*Mass*(Height_Sq + Width_Sq);
				Inertia_Tensor.Ma11 = 0.083333f*Mass*(Width_Sq + Depth_Sq);
				Inertia_Tensor.Ma22 = 0.083333f*Mass*(Depth_Sq + Height_Sq);
				/* Get the inverse of the inertia tensor */
				Matrix3D::GetInverse4x4(Inertia_Tensor,Inv_Inertia_Tensor);
			}

			void Update (Matrix3D WorldTransformation)
			{
				V[0]=WorldTransformation*Pt_TFR;
				V[1]=WorldTransformation*Pt_TBR;
				V[2]=WorldTransformation*Pt_TBL;
				V[3]=WorldTransformation*Pt_TFL;

				V[4]=WorldTransformation*Pt_BFR;
				V[5]=WorldTransformation*Pt_BBR;
				V[6]=WorldTransformation*Pt_BBL;
				V[7]=WorldTransformation*Pt_BFL;		
			}

			void Draw (void)
			{
				Renderer *renderer=app.GetRenderer();
				renderer->SetTextureCoordBuffer(0);
				renderer->SetActiveTextureUnit(0);

				unsigned short Indices [] = {0,1, 1,2, 2,3, 3,0,  0,4, 1,5, 2,6, 3,7,  4,5, 5,6, 6,7, 7,4};
				renderer->SetIndexBuffer(Indices, 24);

				/* Transform to Units for rendering */
				Matrix3D S = Matrix3D::GetScale(TO_UNIT,TO_UNIT,TO_UNIT);
				Vertex Vertices[8];
				Vertices[0] = S*V[0];	Vertices[4] = S*V[4];
				Vertices[1] = S*V[1];	Vertices[5] = S*V[5];
				Vertices[2] = S*V[2];	Vertices[6] = S*V[6];
				Vertices[3] = S*V[3];	Vertices[7] = S*V[7];

				renderer->SetVertexBuffer(Vertices);	
				
				renderer->SetWorldTransformation(Matrix3D::GetIdentity());
				renderer->SetNormalBuffer(0);
	
				renderer->SetPrimitivesType(PrimitiveTypeLineList);
				renderer->Draw();
			}
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
			Physics_Cylinder (Point3D center, float density, float radius, float height):
			Radius(radius),Height(height),
			Physics_Bounding(center,density)
			{
				/* Compute the half height */
				float HalfHeight=Height/2;
				/* Front Cross */
				Pt_FT.SetXYZ(Center.X			,Center.Y+Radius	,Center.Z+HalfHeight);
				Pt_FMR.SetXYZ(Center.X+Radius	,Center.Y			,Center.Z+HalfHeight);
				Pt_FB.SetXYZ(Center.X			,Center.Y-Radius	,Center.Z+HalfHeight);
				Pt_FML.SetXYZ(Center.X-Radius	,Center.Y			,Center.Z+HalfHeight);
				/* Back Cross */
				Pt_BT.SetXYZ(Center.X			,Center.Y+Radius	,Center.Z-HalfHeight);
				Pt_BMR.SetXYZ(Center.X+Radius	,Center.Y			,Center.Z-HalfHeight);
				Pt_BB.SetXYZ(Center.X			,Center.Y-Radius	,Center.Z-HalfHeight);
				Pt_BML.SetXYZ(Center.X-Radius	,Center.Y			,Center.Z-HalfHeight);

				/* Compute Mass */
				float Radius_Sq = Radius*Radius;
				Volume = ((float)PI*Radius_Sq)*Height;
				Mass = Volume*Density;
				/* Compute Inertia Tensor */
				Inertia_Tensor.SetIdentity();
				Inv_Inertia_Tensor.SetIdentity();
				Inertia_Tensor.Ma00 = 0.5f*Mass*Radius_Sq;
				Inertia_Tensor.Ma11 = Inertia_Tensor.Ma22 = 0.083333f*Mass*(3*Radius_Sq + Height*Height); /************************* ??????????????????????????????????????????????? */
				/* Get the inverse of the inertia tensor */
				Matrix3D::GetInverse4x4(Inertia_Tensor,Inv_Inertia_Tensor);
			}

			void Update (Matrix3D WorldTransformation)
			{
				V[0]=WorldTransformation*Pt_FT;
				V[1]=WorldTransformation*Pt_FMR;
				V[2]=WorldTransformation*Pt_FB;
				V[3]=WorldTransformation*Pt_FML;

				V[4]=WorldTransformation*Pt_BT;
				V[5]=WorldTransformation*Pt_BMR;
				V[6]=WorldTransformation*Pt_BB;
				V[7]=WorldTransformation*Pt_BML;		
			}

			void Draw (void)
			{
				Renderer *renderer=app.GetRenderer();
				renderer->SetTextureCoordBuffer(0);
				renderer->SetActiveTextureUnit(0);

				unsigned short Indices [] = {0,2, 1,3,	4,6, 5,7,  0,4, 1,5, 2,6, 3,7, 8,9};
				renderer->SetIndexBuffer(Indices, 18);

				/* Transform to Units for rendering */
				Matrix3D S = Matrix3D::GetScale(TO_UNIT,TO_UNIT,TO_UNIT);
				Vertex Vertices[10];
				Vertices[0] = S*V[0];	Vertices[4] = S*V[4];
				Vertices[1] = S*V[1];	Vertices[5] = S*V[5];
				Vertices[2] = S*V[2];	Vertices[6] = S*V[6];
				Vertices[3] = S*V[3];	Vertices[7] = S*V[7];
				Vertices[8] = S*(Center+Vector3D(0,0,1)*Height/2);
				Vertices[9] = S*(Center+Vector3D(0,0,-1)*Height/2);


				renderer->SetVertexBuffer(Vertices);	
				
				renderer->SetWorldTransformation(Matrix3D::GetIdentity());
				renderer->SetNormalBuffer(0);
	
				renderer->SetPrimitivesType(PrimitiveTypeLineList);
				renderer->Draw();
			}
};

class Physics_Sphere : public Physics_Bounding
{
	public:
			/* Sphere Dimension */
			float Radius;
			/* Sphere 6 points */
			Point3D Pt_Top, Pt_Bottom, Pt_Front, Pt_Back, Pt_Left, Pt_Right;

			/* Sphere constructor */
			Physics_Sphere (Point3D center, float density, float radius):
			Radius(radius),
			Physics_Bounding(center,density)
			{
				/* Computing the Sphere 6 points */
				Pt_Top.SetXYZ	(Center.X			,Center.Y+Radius	,Center.Z);
				Pt_Bottom.SetXYZ(Center.X			,Center.Y-Radius	,Center.Z);
				Pt_Front.SetXYZ	(Center.X			,Center.Y			,Center.Z+Radius);
				Pt_Back.SetXYZ	(Center.X			,Center.Y			,Center.Z-Radius);
				Pt_Left.SetXYZ	(Center.X-Radius	,Center.Y			,Center.Z);
				Pt_Right.SetXYZ	(Center.X+Radius	,Center.Y			,Center.Z);

				/* Compute Mass */
				float Radius_Sq = Radius*Radius;
				Volume = 1.333333f*((float)PI*Radius_Sq*Radius);
				Mass = Volume*Density;
				/* Compute Inertia Tensor */
				Inertia_Tensor.SetIdentity();
				Inv_Inertia_Tensor.SetIdentity();
				Inertia_Tensor.Ma00 = Inertia_Tensor.Ma11 = Inertia_Tensor.Ma22 = 0.4f*Mass*Radius_Sq;
				/* Get the inverse of the inertia tensor */
				Matrix3D::GetInverse4x4(Inertia_Tensor,Inv_Inertia_Tensor);
			}

			void Draw (void)
			{
				Renderer *renderer=app.GetRenderer();
				renderer->SetTextureCoordBuffer(0);
				renderer->SetActiveTextureUnit(0);

				unsigned short Indices [] = {0,1, 2,3, 4,5};
				renderer->SetIndexBuffer(Indices, 6);

				/* Transform to Units for rendering */
				Matrix3D S = Matrix3D::GetScale(TO_UNIT,TO_UNIT,TO_UNIT);
				Vertex Vertices[6];
				Vertices[0] = S*Pt_Top;		
				Vertices[1] = S*Pt_Bottom;	
				Vertices[2] = S*Pt_Front;	
				Vertices[3] = S*Pt_Back;	
				Vertices[4] = S*Pt_Left;
				Vertices[5] = S*Pt_Right;

				renderer->SetVertexBuffer(Vertices);	
				
				renderer->SetWorldTransformation(Matrix3D::GetIdentity());
				renderer->SetNormalBuffer(0);
	
				renderer->SetPrimitivesType(PrimitiveTypeLineList);
				renderer->Draw();
			}
};

#endif