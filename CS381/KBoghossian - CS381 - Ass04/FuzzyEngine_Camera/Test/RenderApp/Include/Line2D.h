/* Start Header ------------------------------------------------------------

Copyright (C) 2004 DigiPen Institute of Technology.
		Reproduction or disclosure of this file or its contents without the
		Prior written consent of DigiPen Institute of Technology is prohibited.

File Name:	Line2D.h

Purpose:	<using 2d lines functionnality, and their classification along other shapes>

Language:	C++ compiler of Visual C++ 7.x, Intel

Platform:	Windows

Project:	Assignment 1 CS300: Utility Library

Author:	AKE development team

Creation date: <11/10/2004>

- End Header --------------------------------------------------------------*/

#ifndef _LINE2D_H_
#define _LINE2D_H_

class Line2D
{
public:
    Line2D();
    Line2D(const Point2D & p,const Vector2D & v);
	Line2D(const Line2D & lin);
	~Line2D();

	/// A point on the line.
    Point2D point;
    /// A Vector parallel to the line.
    Vector2D direction;

	//Set
//	inline void SetPoint(Point2D poi) {point = poi;};
//	inline void SetVector(Vector2D vec) {direction = vec;};
	//Get
//	inline Point2D point {return point;};
//	inline Vector2D direction {return direction;};

    CLASSIFY ClassifyToLine(Line2D & lin);
	CLASSIFY ClassifyToLine(Line2D & lin,Point2D & pinter);
	bool Perpendicular(Line2D & lin);
	bool Parallel(Line2D & lin);
	float AngleBetween(Line2D & lin);//in degree
	CLASSIFY ClassifyToCircle(Circle2D & cir);
	CLASSIFY ClassifyToCircle(Circle2D & cir,Point2D pinter[2]);
	bool Tangent(Circle2D & cir);
	bool Tangent(Circle2D & cir,Point2D & pinter);
	CLASSIFY ClassifyToRay(Ray2D & ray);
	CLASSIFY ClassifyToRay(Ray2D & ray,Point2D & pinter);
	CLASSIFY ClassifyToLineSegment(LineSegment2D & lns);
	CLASSIFY ClassifyToLineSegment(LineSegment2D & lns,Point2D & pinter);

protected:
private:	
};

#endif