#include "TestRenderApp.h"
#include "Physics_Support_Functions.h"
#include "Support_Functions.h"


/* Function to classify a point to a plane */
CLASSIFY Classify_Pt_Plane(Vertex point,Plane3D splitter)
{
	/* Vector going from the plane point to the vertex being classified */
	Vector3D PPt_Vtx(point.X-splitter.point.X, point.Y-splitter.point.Y, point.Z-splitter.point.Z);
	/* Compute the dot product of the Face normal with the vector previously calculated */
	float Dot_Product=splitter.normal.GetDotProduct(PPt_Vtx);
	
	/* Return the classification according to the dot product result */
	if(Dot_Product>EPSILON)
		return FRONT;
	else if(Dot_Product<-EPSILON)
		return BACK;
	return INTERSECT;
}

CLASSIFY Custom_Classify_PtToPlane (Plane3D Pl, Point3D Pt)
{
	float Result = Pl.A*Pt.X + Pl.B*Pt.Y + Pl.C*Pt.Z + Pl.D;
	if(Result<0)
		return BACK;
	else if(Result>0)
		return FRONT;
	return INTERSECT;
}

CLASSIFY Custom_ClassifyToBox (Box3D & box, Point3D P)
{
	float dot[6];
	int numofindexes=0;
	for(int i=0; i < 6; i++)
	{
        dot[i] = ( (P.X - box.planes[i].point.X)*box.planes[i].normal.X +
            (P.Y - box.planes[i].point.Y)*box.planes[i].normal.Y + 
            (P.Z - box.planes[i].point.Z)*box.planes[i].normal.Z );
		if(dot[i] < 0.0f)
			return OUTSIDE;
		if((dot[i] >= -FUN_EPSILON) && (dot[i] <=FUN_EPSILON))
			numofindexes++;		
	}
	//the point intersect the box or is inside of it
	switch(numofindexes)
	{
	case 0:
		return INSIDE;
	case 1:
		return ONE_FACE_INTERSECT;
	case 2:
        return ONE_EDGE_INTERSECT;
	case 3:
		return ONE_CORNER_INTERSECT;
	}
	//never reached
	return OUTSIDE;
}

/* Compute the angle of the object given according to a plane */
float Compute_Angle_AccordingTo_Plane(Vector3D Obj_Dir , Plane3D Pln)
{
	Point3D Origin = Pln.point;
	Point3D End = Origin + Obj_Dir*10;
	/* Projecting the velocity vector on the XZ-plane of the body */
	End.ProjectionOnPlane(Pln, End);
	/* Computing the drag vector */
	Vector3D Plane_Vector = End - Origin;
	Plane_Vector.SetNormalize();

	float Dir = 1;
	if(Obj_Dir.Y<0)
		Dir=-1;
	return acosf(Obj_Dir.GetDotProduct(Plane_Vector) / Plane_Vector.GetLength())*ONEEIGHTY_OVER_PI*Dir;

}

Vector3D Compute_PrjectedOn_Plane (Vector3D Source, Plane3D Pln)
{
	Point3D Origin = Pln.point;
	Point3D End = Origin + Source*10;
	/* Projecting the velocity vector on the XZ-plane of the body */
	End.ProjectionOnPlane(Pln, End);
	/* Computing the drag vector */
	Vector3D XZ_Vector = End - Origin;
	XZ_Vector.SetNormalize();
	return XZ_Vector;
}