/* Start Header ------------------------------------------------------------

Copyright (C) 2004 DigiPen Institute of Technology.
		Reproduction or disclosure of this file or its contents without the
		Prior written consent of DigiPen Institute of Technology is prohibited.

File Name:	Line3D.h

Purpose:	<using 3d lines functionnality, and their classification along other shapes>

Language:	C++ compiler of Visual C++ 7.x, Intel

Platform:	Windows

Project:	Assignment 1 CS300: Utility Library

Author:	AKE development team

Creation date: <11/10/2004>

- End Header --------------------------------------------------------------*/

#ifndef _LINE3D_H_
#define _LINE3D_H_

//#include "Ray3D.h"
//class Ray3D;
//class Sphere3D;
//class LineSegment3D;
//class Plane3D;
//#include "Point3D.h"
//class Point3D;

class Line3D
{
public:
    Line3D();
    Line3D(const Point3D & p,const Point3D & v);
	Line3D(const Line3D & lin);
	~Line3D();

	/// A point on the line.
    Point3D point;
    /// A Vector parallel to the line.
    Point3D direction;

	//Set
//	inline void SetPoint(Point3D poi) {point = poi;};
//	inline void SetVector(Point3D vec) {direction = vec;};
	//Get
//	inline Point3D point {return point;};
//	inline Point3D direction {return direction;};
    
	CLASSIFY ClassifyToLine(Line3D & lin);
	CLASSIFY ClassifyToLine(Line3D & lin,Point3D & pinter);
	bool Perpendicular(Line3D & lin);
	bool Parallel(Line3D & lin);
	bool Coplanar(Line3D & lin);
	float AngleBetween(Line3D & lin);
	CLASSIFY ClassifyToRay(Ray3D & ray);
	CLASSIFY ClassifyToRay(Ray3D & ray,Point3D & pinter);
	CLASSIFY ClassifyToLineSegment(LineSegment3D & lns);
	CLASSIFY ClassifyToLineSegment(LineSegment3D & lns,Point3D & pinter);
	CLASSIFY ClassifyToPlane(Plane3D & pln);
	CLASSIFY ClassifyToPlane(Plane3D & pln,Point3D & pinter);
	CLASSIFY ClassifyToSphere(Sphere3D & sph,Point3D pinter[2]);

protected:
private:	
};

#endif