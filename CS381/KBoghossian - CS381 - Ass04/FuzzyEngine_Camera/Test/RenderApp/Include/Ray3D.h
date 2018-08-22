/* Start Header ------------------------------------------------------------

Copyright (C) 2004 DigiPen Institute of Technology.
		Reproduction or disclosure of this file or its contents without the
		Prior written consent of DigiPen Institute of Technology is prohibited.

File Name:	Ray3D.h

Purpose:	<using 3d rays functionnality, and their classification along other shapes>

Language:	C++ compiler of Visual C++ 7.x, Intel

Platform:	Windows

Project:	Assignment 1 CS300: Utility Library

Author:	AKE development team

Creation date: <11/10/2004>

- End Header --------------------------------------------------------------*/

#ifndef _RAY3D_H_
#define _RAY3D_H_

//#include "LineSegment3D.h"
//class Line3D;

class Ray3D
{
public:
    Ray3D();
    Ray3D(const Point3D & o,const Point3D & d);
	Ray3D(const Ray3D & ray);
	~Ray3D();

	/// The origin of the ray.
    Point3D origin;
    /// The direction of the ray.
    Point3D direction;

	//Set
//	inline void SetOrigin(Point3D ori) {origin = ori;};
//	inline void SetDirection(Point3D dir) {direction = dir;};
	//Get
//	inline Point3D origin {return origin;};
//	inline Point3D direction {return direction;};

    CLASSIFY ClassifyToRay(Ray3D & ray);
	CLASSIFY ClassifyToRay(Ray3D & ray,Point3D & pinter,LineSegment3D & segmentoverlap,Ray3D & rayoverlap);
	bool Perpendicular(Ray3D & ray);
	bool Parallel(Ray3D & ray);
	bool Coplanar(Ray3D & ray);
	float AngleBetween(Ray3D & ray);//in degree
	CLASSIFY ClassifyToLineSegment(LineSegment3D & lns);
	CLASSIFY ClassifyToLineSegment(LineSegment3D & lns,Point3D & pinter,LineSegment3D & segmentoverlap);
	CLASSIFY ClassifyToLine(Line3D & lin);
	CLASSIFY ClassifyToLine(Line3D & lin,Point3D & pinter);
	CLASSIFY ClassifyToPlane(Plane3D & pln);
	CLASSIFY ClassifyToPlane(Plane3D & pln,Point3D & pinter);
	CLASSIFY ClassifyToSphere(Sphere3D & sph,Point3D pinter[2]);

protected:
private:	
};

#endif