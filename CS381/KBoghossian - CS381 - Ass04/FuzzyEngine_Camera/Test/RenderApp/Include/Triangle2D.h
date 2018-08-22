/* Start Header ------------------------------------------------------------

Copyright (C) 2004 DigiPen Institute of Technology.
		Reproduction or disclosure of this file or its contents without the
		Prior written consent of DigiPen Institute of Technology is prohibited.

File Name:	Triangle2D.h

Purpose:	<using 2d triangles functionnality, and their classification along other shapes>

Language:	C++ compiler of Visual C++ 7.x, Intel

Platform:	Windows

Project:	Assignment 1 CS300: Utility Library

Author:	AKE development team

Creation date: <11/10/2004>

- End Header --------------------------------------------------------------*/

#ifndef _TRIANGLE2D_H_
#define _TRIANGLE2D_H_

class Triangle2D
{
public:
	Triangle2D();
	Triangle2D(const Triangle2D & tri);
    Triangle2D(const Point2D & pt0,const Point2D & pt1,const Point2D & pt2);
	~Triangle2D();

	Point2D p0;
	Point2D p1;
	Point2D p2;

	//Set
/*	inline void SetP0(Point2D pt0) {p0 = pt0;};
	inline void SetP1(Point2D pt1) {p1 = pt1;};
	inline void SetP2(Point2D pt2) {p2 = pt2;};
	//Get
	inline Point2D p0 {return p0;};
	inline Point2D p1 {return p1;};
	inline Point2D p2 {return p2;};*/

	Point2D & operator[](int i);
	const Point2D & operator[](int i) const;

	CLASSIFY ClassifyToTriangle(Triangle2D & tri);
	CLASSIFY ClassifyToTriangle(Triangle2D & tri,Point2D pinter[6],LineSegment2D segmentoverlap[3],int & numpoints,int & numsegments);

protected:
private:	
};

#endif