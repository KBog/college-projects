/* Start Header ------------------------------------------------------------

Copyright (C) 2004 DigiPen Institute of Technology.
		Reproduction or disclosure of this file or its contents without the
		Prior written consent of DigiPen Institute of Technology is prohibited.

File Name:	Triangle3D.h

Purpose:	<using 3d triangles functionnality, and their classification along other shapes>

Language:	C++ compiler of Visual C++ 7.x, Intel

Platform:	Windows

Project:	Assignment 1 CS300: Utility Library

Author:	AKE development team

Creation date: <11/10/2004>

- End Header --------------------------------------------------------------*/

#ifndef _TRIANGLE3D_H_
#define _TRIANGLE3D_H_

class Triangle3D
{
public:
	Triangle3D();
    Triangle3D(const Point3D & pt0,const Point3D & pt1,const Point3D & pt2);
	Triangle3D(const Triangle3D & tri);
	~Triangle3D();

	Point3D p0;
	Point3D p1;
	Point3D p2;
	Point3D normal;

	//Set
//	inline void SetP0(Point3D pt0);
//	inline void SetP1(Point3D pt1);
//	inline void SetP2(Point3D pt2);
//	inline void SetNormal(Point3D nor);
	//Get
//	inline Point3D p0 {return p0;};
//	inline Point3D p1 {return p1;};
//	inline Point3D p2 {return p2;};
//	inline Point3D normal {return normal;};

	Point3D & operator[](int i);
	const Point3D & operator[](int i) const;

	CLASSIFY ClassifyToTriangle(Triangle3D & tri);
	CLASSIFY ClassifyToTriangle(Triangle3D & tri,Point3D pinter[6],LineSegment3D segmentoverlap[3],int & numpoints,int & numsegments);//we need the intersection here to divide triangles for ex
	float AngleBetween(Triangle3D & tri);//in degree
	bool Perpendicular(Triangle3D & tri);
	bool Parallel(Triangle3D & tri);
	bool Coplanar(Triangle3D & tri);

protected:
private:	
};
/*
inline void Triangle3D::SetP0(Point3D pt0)
{
	p0 = pt0;
	normal = Point3D((p1.Y - p0.Y)*(p2.Z - p0.Z) - (p1.Z - p0.Z)*(p2.Y - p0.Y),(p1.Z - p0.Z)*(p2.X - p0.X) - (p1.X - p0.X)*(p2.Z - p0.Z),(p1.X - p0.X)*(p2.Y - p0.Y) - (p1.Y - p0.Y)*(p2.X - p0.X));
	normal.SetNormalize();
}

inline void Triangle3D::SetP1(Point3D pt1)
{
	p1 = pt1;
	normal = Point3D((p1.Y - p0.Y)*(p2.Z - p0.Z) - (p1.Z - p0.Z)*(p2.Y - p0.Y),(p1.Z - p0.Z)*(p2.X - p0.X) - (p1.X - p0.X)*(p2.Z - p0.Z),(p1.X - p0.X)*(p2.Y - p0.Y) - (p1.Y - p0.Y)*(p2.X - p0.X));
	normal.SetNormalize();
}

inline void Triangle3D::SetP2(Point3D pt2)
{
	p2 = pt2;
	normal = Point3D((p1.Y - p0.Y)*(p2.Z - p0.Z) - (p1.Z - p0.Z)*(p2.Y - p0.Y),(p1.Z - p0.Z)*(p2.X - p0.X) - (p1.X - p0.X)*(p2.Z - p0.Z),(p1.X - p0.X)*(p2.Y - p0.Y) - (p1.Y - p0.Y)*(p2.X - p0.X));
	normal.SetNormalize();
}

inline void Triangle3D::SetNormal(Point3D nor)
{
	normal = nor;
	normal.SetNormalize();
}
*/
#endif