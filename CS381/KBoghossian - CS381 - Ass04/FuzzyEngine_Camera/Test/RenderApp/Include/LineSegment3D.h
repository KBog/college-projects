/* Start Header ------------------------------------------------------------

Copyright (C) 2004 DigiPen Institute of Technology.
		Reproduction or disclosure of this file or its contents without the
		Prior written consent of DigiPen Institute of Technology is prohibited.

File Name:	LineSegment3D.h

Purpose:	<using 3d line segments functionnality, and their classification along other shapes>

Language:	C++ compiler of Visual C++ 7.x, Intel

Platform:	Windows

Project:	Assignment 1 CS300: Utility Library

Author:	AKE development team

Creation date: <11/10/2004>

- End Header --------------------------------------------------------------*/

#ifndef _LINESEGMENT3D_H_
#define _LINESEGMENT3D_H_

//class Quad3D;
//class Line3D;
//class Plane3D;
//class Ray3D;
//class Sphere3D;

class LineSegment3D
{
public:
    LineSegment3D();
    LineSegment3D(const Point3D & pA,const Point3D & pB);
	LineSegment3D(const LineSegment3D & lns);
	~LineSegment3D();

	Point3D & operator[](int i);
	const Point3D & operator[](int i) const;

	LineSegment3D & operator= (LineSegment3D & lns);

	/// One of the endpoints of the line segment.
    Point3D pointA;
    /// The other endpoint of the line segment.
    Point3D pointB;

	//Set
//	inline void SetPointA(Point3D pA) {pointA = pA;};
//	inline void SetPointB(Point3D pB) {pointB = pB;};
	//Get
//	inline Point3D pointA {return pointA;};
//	inline Point3D pointB {return pointB;};

	CLASSIFY ClassifyToLineSegment(LineSegment3D & lns);
	CLASSIFY ClassifyToLineSegmentCoplanar(LineSegment3D & lns);
	CLASSIFY ClassifyToLineSegment(LineSegment3D & lns,Point3D & pinter,LineSegment3D & segmentoverlap);
	bool Perpendicular(LineSegment3D & lns);
	bool Parallel(LineSegment3D & lns);
	bool Alligned(LineSegment3D & lns);
	bool Coplanar(LineSegment3D & lns);
	float AngleBetween(LineSegment3D & lns);//in degree
	CLASSIFY ClassifyToRay(Ray3D & ray);
	CLASSIFY ClassifyToRay(Ray3D & ray,Point3D & pinter,LineSegment3D & segmentoverlap);
	CLASSIFY ClassifyToLine(Line3D & lin);
	CLASSIFY ClassifyToLine(Line3D & lin,Point3D & pinter);
	CLASSIFY ClassifyToPlane(Plane3D & pln);
	CLASSIFY ClassifyToPlane(Plane3D & pln,Point3D & pinter);
	CLASSIFY ClassifyToQuad(Quad3D & qua);
	CLASSIFY ClassifyToQuadSimplified(Quad3D & qua);
	CLASSIFY ClassifyToQuad(Quad3D & qua,LineSegment3D segmentintersection[2],Point3D pointintersection[2]);
	CLASSIFY ClassifyToSphere(Sphere3D & sph,Point3D pinter[2]);

protected:
private:	
};

#endif