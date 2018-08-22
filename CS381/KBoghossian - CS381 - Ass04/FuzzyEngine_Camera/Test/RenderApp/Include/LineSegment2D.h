/* Start Header ------------------------------------------------------------

Copyright (C) 2004 DigiPen Institute of Technology.
		Reproduction or disclosure of this file or its contents without the
		Prior written consent of DigiPen Institute of Technology is prohibited.

File Name:	LineSegment2D.h

Purpose:	<using 2d line segments functionnality, and their classification along other shapes>

Language:	C++ compiler of Visual C++ 7.x, Intel

Platform:	Windows

Project:	Assignment 1 CS300: Utility Library

Author:	AKE development team

Creation date: <11/10/2004>

- End Header --------------------------------------------------------------*/

#ifndef _LINESEGMENT2D_H_
#define _LINESEGMENT2D_H_

class LineSegment2D
{
public:
    LineSegment2D();
    LineSegment2D(const Point2D & pA,const Point2D & pB);
	LineSegment2D(const LineSegment2D & lns);
	~LineSegment2D();

	Point2D & operator[](int i);
	const Point2D & operator[](int i) const;

	LineSegment2D & operator= (LineSegment2D & lns);

	/// One of the endpoints of the line segment.
    Point2D pointA;
    /// The other endpoint of the line segment.
    Point2D pointB;

	//Set
//	inline void SetPointA(Point2D pA) {pointA = pA;};
//	inline void SetPointB(Point2D pB) {pointB = pB;};
	//Get
//	inline Point2D pointA {return pointA;};
//	inline Point2D pointB {return pointB;};
	
	CLASSIFY ClassifyToLineSegment(LineSegment2D & lns);
	CLASSIFY ClassifyToLineSegment(LineSegment2D & lns,Point2D & pinter,LineSegment2D & segmentoverlap);
	bool Perpendicular(LineSegment2D & lns);
	bool Parallel(LineSegment2D & lns);
	bool Alligned(LineSegment2D & lns);
	float AngleBetween(LineSegment2D & lns);
	CLASSIFY ClassifyToCircle(Circle2D & cir);
	CLASSIFY ClassifyToCircle(Circle2D & cir,Point2D pinter[2]);
	bool Tangent(Circle2D & cir);
	bool Tangent(Circle2D & cir,Point2D & pinter);
	CLASSIFY ClassifyToRay(Ray2D & ray);
	CLASSIFY ClassifyToRay(Ray2D & ray,Point2D & pinter,LineSegment2D & segmentoverlap);
	CLASSIFY ClassifyToLine(Line2D & lin);
	CLASSIFY ClassifyToLine(Line2D & lin,Point2D & pinter);

protected:
private:	
};

#endif