/* Start Header ------------------------------------------------------------

Copyright (C) 2004 DigiPen Institute of Technology.
		Reproduction or disclosure of this file or its contents without the
		Prior written consent of DigiPen Institute of Technology is prohibited.

File Name:	Quad3D.h

Purpose:	<using 3d quads functionnality, and their classification along other shapes>

Language:	C++ compiler of Visual C++ 7.x, Intel

Platform:	Windows

Project:	Assignment 1 CS300: Utility Library

Author:	AKE development team

Creation date: <11/10/2004>

- End Header --------------------------------------------------------------*/

#ifndef _QUAD3D_H_
#define _QUAD3D_H_

class Quad3D
{
public:
	Quad3D();
    Quad3D(const Point3D & pt0,const Point3D & pt1,const Point3D & pt2,const Point3D & pt3);//counter clock wise
	Quad3D(const Quad3D & qua);
	~Quad3D();

	void Init(const Point3D & pt0,const Point3D & pt1,const Point3D & pt2,const Point3D & pt3);

	Point3D p0;
	Point3D p1;
	Point3D p2;
	Point3D p3;
	Point3D normal;

	//Set
/*	inline void SetP0(Point3D pt0);
	inline void SetP1(Point3D pt1);
	inline void SetP2(Point3D pt2);
	inline void SetP3(Point3D pt3);
	inline void SetNormal(Point3D nor);
	//Get
	inline Point3D p0 {return p0;};
	inline Point3D p1 {return p1;};
	inline Point3D p2 {return p2;};
	inline Point3D GetP3() {return p3;};
	inline Point3D normal {return normal;};*/

	Point3D & operator[](int i);
	const Point3D & operator[](int i) const;

	//CLASSIFY ClassifyToQuad(Quad3D & qua);
	//CLASSIFY ClassifyToTriangle(Quad3D & qua,Point3D pinter[8],LineSegment3D segmentoverlap[4],int & numpoints,int & numsegments);
	//float AngleBetween(Quad3D & qua);//in degree
	//bool Perpendicular(Quad3D & qua);
	//bool Parallel(Quad3D & qua);
	//bool Coplanar(Quad3D & qua);	

protected:
private:
	//four points are coplanar
	
};
/*
inline void Quad3D::SetP0(Point3D pt0)
{
	p0 = pt0;
	normal = Point3D((p1.Y - p0.Y)*(p2.Z - p0.Z) - (p1.Z - p0.Z)*(p2.Y - p0.Y),(p1.Z - p0.Z)*(p2.X - p0.X) - (p1.X - p0.X)*(p2.Z - p0.Z),(p1.X - p0.X)*(p2.Y - p0.Y) - (p1.Y - p0.Y)*(p2.X - p0.X));
	normal.SetNormalize();
}

inline void Quad3D::SetP1(Point3D pt1)
{
	p1 = pt1;
	normal = Point3D((p1.Y - p0.Y)*(p2.Z - p0.Z) - (p1.Z - p0.Z)*(p2.Y - p0.Y),(p1.Z - p0.Z)*(p2.X - p0.X) - (p1.X - p0.X)*(p2.Z - p0.Z),(p1.X - p0.X)*(p2.Y - p0.Y) - (p1.Y - p0.Y)*(p2.X - p0.X));
	normal.SetNormalize();
}

inline void Quad3D::SetP2(Point3D pt2)
{
	p2 = pt2;
	normal = Point3D((p1.Y - p0.Y)*(p2.Z - p0.Z) - (p1.Z - p0.Z)*(p2.Y - p0.Y),(p1.Z - p0.Z)*(p2.X - p0.X) - (p1.X - p0.X)*(p2.Z - p0.Z),(p1.X - p0.X)*(p2.Y - p0.Y) - (p1.Y - p0.Y)*(p2.X - p0.X));
	normal.SetNormalize();
}

inline void Quad3D::SetP3(Point3D pt3)
{
	p3 = pt3;
	normal = Point3D((p1.Y - p0.Y)*(p2.Z - p0.Z) - (p1.Z - p0.Z)*(p2.Y - p0.Y),(p1.Z - p0.Z)*(p2.X - p0.X) - (p1.X - p0.X)*(p2.Z - p0.Z),(p1.X - p0.X)*(p2.Y - p0.Y) - (p1.Y - p0.Y)*(p2.X - p0.X));
	normal.SetNormalize();
}

inline void Quad3D::SetNormal(Point3D nor)
{
	normal = nor;
	normal.SetNormalize();
}*/

#endif