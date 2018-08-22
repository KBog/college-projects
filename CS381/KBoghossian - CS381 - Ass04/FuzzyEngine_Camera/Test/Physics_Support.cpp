#include "Physics_Support.h"
#include "Object.h"

/* Default Constructor */
Material_Properties::Material_Properties ()
{
	Density=0;
	Static_Friction=0;
	Kinetic_Friction=0;
	Volume=0;
}
/* Material properties constructor (Volume=-1 to know whether we must compute it or send it by paramter) */
Material_Properties::Material_Properties(float density, float sfriction, float dfriction, float volume)
{
	Density=density;
	Volume=volume;
	Static_Friction=sfriction;
	Kinetic_Friction=dfriction;
}

////////////////////////////////////////////////////////////////////////////////

/* Box Constructor */
Physics_Box::Physics_Box (Point3D center, Material_Properties materialproperties, float width, float height, float depth) :
Width(width),Height(height),Depth(depth),
Physics_Bounding(center,materialproperties)
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

	/* Compute the 3 areas of the box */
	Area_FrontBack = Width*Height;
	Area_LeftRight = Depth*Height;
	Area_TopBottom = Width*Depth;

	if(Width > Height)
	{
		if(Width > Depth)
		{
			Chord = Width;
		}
		else
		{
			Chord = Depth;
		}
	}
	else
	{
		if(Height > Depth)
		{
			Chord = Height;
		}
		else
		{
			Chord = Depth;
		}
	}

	/* Compute Mass */
	MaterialProperties.Volume = Width*Height*Depth;
	Mass = MaterialProperties.Volume*MaterialProperties.Density;
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

/* Function to update the vertices to the world space */
void Physics_Box::Update (Matrix3D WorldTransformation)
{
	World_Center = WorldTransformation*Center;
	V[0]=WorldTransformation*Pt_TFR;
	V[1]=WorldTransformation*Pt_TBR;
	V[2]=WorldTransformation*Pt_TBL;
	V[3]=WorldTransformation*Pt_TFL;

	V[4]=WorldTransformation*Pt_BFR;
	V[5]=WorldTransformation*Pt_BBR;
	V[6]=WorldTransformation*Pt_BBL;
	V[7]=WorldTransformation*Pt_BFL;		
}

/* Draw the shape according to the vertices in the space */
void Physics_Box::Draw (void)
{
	glDisable(GL_TEXTURE_2D);
	glColor4f(1,0,1,1);

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

	glColor4f(1,1,1,1);
	glEnable(GL_TEXTURE_2D);
}

/* Function to get the projected area of the box according to the current velocity vector */
float Physics_Box::Get_ProjectedArea (Vector3D Direction)
{
	float X=Direction.X, Y=Direction.Y, Z=Direction.Z;

	if(X>EPSILON)	//X>0
	{
		if(Y>EPSILON)	//Y>0
		{
			if(Z>EPSILON)		//Z>0
				return Area_TopBottom+Area_FrontBack+Area_LeftRight;
			else if(Z<-EPSILON)		//Z<0
				return Area_TopBottom+Area_FrontBack+Area_LeftRight;
			else		//Z=0
				return Area_TopBottom+Area_LeftRight;
		}
		else if(Y<-EPSILON)	//Y<0
		{
			if(Z>EPSILON)		//Z>0
				return Area_TopBottom+Area_FrontBack+Area_LeftRight;
			else if(Z<-EPSILON)	//Z<0
				return Area_TopBottom+Area_FrontBack+Area_LeftRight;
			else		//Z=0
				return Area_TopBottom+Area_LeftRight;
		}
		else		//Y=0
		{
			if(Z>EPSILON)		//Z>0
				return Area_FrontBack+Area_LeftRight;
			else if(Z<-EPSILON)	//Z<0
				return Area_FrontBack+Area_LeftRight;
			else		//Z=0
				return Area_LeftRight;
		}
	}
	else if(X<-EPSILON)	//X<0
	{
		if(Y>EPSILON)	//Y>0
		{
			if(Z>EPSILON)		//Z>0
				return Area_TopBottom+Area_FrontBack+Area_LeftRight;
			else if(Z<-EPSILON)		//Z<0
				return Area_TopBottom+Area_FrontBack+Area_LeftRight;
			else		//Z=0
				return Area_TopBottom+Area_LeftRight;
		}
		else if(Y<-EPSILON)	//Y<0
		{
			if(Z>EPSILON)		//Z>0
				return Area_TopBottom+Area_FrontBack+Area_LeftRight;
			else if(Z<-EPSILON)	//Z<0
				return Area_TopBottom+Area_FrontBack+Area_LeftRight;
			else		//Z=0
				return Area_TopBottom+Area_LeftRight;
		}
		else		//Y=0
		{
			if(Z>EPSILON)		//Z>0
				return Area_FrontBack+Area_LeftRight;
			else if(Z<-EPSILON)	//Z<0
				return Area_FrontBack+Area_LeftRight;
			else		//Z=0
				return Area_LeftRight;
		}
	}
	else		//X=0
	{
		if(Y>EPSILON)	//Y>0
		{
			if(Z>EPSILON)		//Z>0
				return Area_TopBottom+Area_FrontBack+Area_LeftRight;
			else if(Z<-EPSILON)		//Z<0
				return Area_TopBottom+Area_FrontBack;
			else		//Z=0
				return Area_TopBottom;
		}
		else if(Y<-EPSILON)	//Y<0
		{
			if(Z>EPSILON)		//Z>0
				return Area_TopBottom+Area_FrontBack;
			else if(Z<-EPSILON)	//Z<0
				return Area_TopBottom+Area_FrontBack;
			else		//Z=0
				return Area_TopBottom;
		}
		else		//Y=0
		{
			if(Z>EPSILON)		//Z>0
				return Area_FrontBack;
			else if(Z<-EPSILON)	//Z<0
				return Area_FrontBack;
			else		//Z=0
				return 0;
		}
	}
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* Cylinder constructor */
Physics_Cylinder::Physics_Cylinder (Point3D center, Material_Properties materialproperties, float radius, float height, Point3D Orientation):
Radius(radius),Height(height),
Physics_Bounding(center,materialproperties)
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

	/* Get the rotation matrix to specify the orientation of the cylinder */
	Matrix3D Rotation = Matrix3D::GetRotateOnZ(Orientation.Z)*Matrix3D::GetRotateOnY(Orientation.Y)*Matrix3D::GetRotateOnX(Orientation.X);
	/* Transform the initial points */
	Pt_FT = Rotation*Pt_FT;
	Pt_FMR = Rotation*Pt_FMR;
	Pt_FB = Rotation*Pt_FB;
	Pt_FML = Rotation*Pt_FML;
	Pt_BT = Rotation*Pt_BT;
	Pt_BMR = Rotation*Pt_BMR;
	Pt_BB = Rotation*Pt_BB;
	Pt_BML = Rotation*Pt_BML;


	/* Compute Mass */
	float Radius_Sq = Radius*Radius;
	MaterialProperties.Volume = ((float)PI*Radius_Sq)*Height;
	Mass = MaterialProperties.Volume*MaterialProperties.Density;
	/* Compute Inertia Tensor */
	Inertia_Tensor.SetIdentity();
	Inv_Inertia_Tensor.SetIdentity();
	Inertia_Tensor.Ma00 = 0.5f*Mass*Radius_Sq;
	Inertia_Tensor.Ma11 = Inertia_Tensor.Ma22 = 0.083333f*Mass*(3*Radius_Sq + Height*Height); /************************* ??????????????????????????????????????????????? */
	/* Get the inverse of the inertia tensor */
	Matrix3D::GetInverse4x4(Inertia_Tensor,Inv_Inertia_Tensor);
}

/* Function to update the vertices to the world space */
void Physics_Cylinder::Update (Matrix3D WorldTransformation)
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

/* Draw the shape according to the vertices in the space */
void Physics_Cylinder::Draw (void)
{
	glDisable(GL_TEXTURE_2D);
	glColor4f(1,0,1,1);

	Renderer *renderer=app.GetRenderer();
	renderer->SetTextureCoordBuffer(0);
	renderer->SetActiveTextureUnit(0);

	unsigned short Indices [] = {0,2,1,3,	4,6,5,7,  0,4, 1,5, 2,6, 3,7};
	renderer->SetIndexBuffer(Indices, 16);

	/* Transform to Units for rendering */
	Matrix3D S = Matrix3D::GetScale(TO_UNIT,TO_UNIT,TO_UNIT);
	Vertex Vertices[10];
	Vertices[0] = S*V[0];	Vertices[4] = S*V[4];
	Vertices[1] = S*V[1];	Vertices[5] = S*V[5];
	Vertices[2] = S*V[2];	Vertices[6] = S*V[6];
	Vertices[3] = S*V[3];	Vertices[7] = S*V[7];
	
	renderer->SetVertexBuffer(Vertices);	
	
	renderer->SetWorldTransformation(Matrix3D::GetIdentity());
	renderer->SetNormalBuffer(0);

	renderer->SetPrimitivesType(PrimitiveTypeLineList);
	renderer->Draw();

	glColor4f(1,1,1,1);
	glEnable(GL_TEXTURE_2D);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* Sphere constructor */
Physics_Sphere::Physics_Sphere (Point3D center, Material_Properties materialproperties, float radius):
Radius(radius),
Physics_Bounding(center,materialproperties)
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
	MaterialProperties.Volume = 1.333333f*((float)PI*Radius_Sq*Radius);
	Mass = MaterialProperties.Volume*MaterialProperties.Density;
	/* Compute Inertia Tensor */
	Inertia_Tensor.SetIdentity();
	Inv_Inertia_Tensor.SetIdentity();
	Inertia_Tensor.Ma00 = Inertia_Tensor.Ma11 = Inertia_Tensor.Ma22 = 0.4f*Mass*Radius_Sq;
	/* Get the inverse of the inertia tensor */
	Matrix3D::GetInverse4x4(Inertia_Tensor,Inv_Inertia_Tensor);
}

/* Function to update the vertices to the world space */
void Physics_Sphere::Update (Matrix3D WorldTransformation)
{
	V[0]=WorldTransformation*Pt_Top;
	V[1]=WorldTransformation*Pt_Bottom;
	V[2]=WorldTransformation*Pt_Front;
	V[3]=WorldTransformation*Pt_Back;
	V[4]=WorldTransformation*Pt_Left;
	V[5]=WorldTransformation*Pt_Right;
}

/* Draw the shape according to the vertices in the space */
void Physics_Sphere::Draw (void)
{
	glDisable(GL_TEXTURE_2D);
	glColor4f(1,0,1,1);

	Renderer *renderer=app.GetRenderer();
	renderer->SetTextureCoordBuffer(0);
	renderer->SetActiveTextureUnit(0);

	unsigned short Indices [] = {0,1, 2,3, 4,5};
	renderer->SetIndexBuffer(Indices, 6);

	/* Transform to Units for rendering */
	Matrix3D S = Matrix3D::GetScale(TO_UNIT,TO_UNIT,TO_UNIT);
	Vertex Vertices[6];
	Vertices[0] = S*V[0];		
	Vertices[1] = S*V[1];	
	Vertices[2] = S*V[2];	
	Vertices[3] = S*V[3];	
	Vertices[4] = S*V[4];
	Vertices[5] = S*V[5];

	renderer->SetVertexBuffer(Vertices);	
	
	renderer->SetWorldTransformation(Matrix3D::GetIdentity());
	renderer->SetNormalBuffer(0);

	renderer->SetPrimitivesType(PrimitiveTypeLineList);
	renderer->Draw();

	glColor4f(1,1,1,1);
	glEnable(GL_TEXTURE_2D);
}