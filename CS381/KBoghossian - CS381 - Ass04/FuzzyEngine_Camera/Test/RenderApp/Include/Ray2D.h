/* Start Header ------------------------------------------------------------

Copyright (C) 2004 DigiPen Institute of Technology.
		Reproduction or disclosure of this file or its contents without the
		Prior written consent of DigiPen Institute of Technology is prohibited.

File Name:	Ray2D.h

Purpose:	<using 2d rays functionnality, and their classification along other shapes>

Language:	C++ compiler of Visual C++ 7.x, Intel

Platform:	Windows

Project:	Assignment 1 CS300: Utility Library

Author:	AKE development team

Creation date: <11/10/2004>

- End Header --------------------------------------------------------------*/

#ifndef _RAY2D_H_
#define _RAY2D_H_

class Ray2D
{
public:
    Ray2D();
    Ray2D(const Point2D & o,const Vector2D & d);
	Ray2D(const Ray2D & ray);
	~Ray2D();

	/// The origin of the ray.
    Point2D origin;
    /// The direction of the ray.
    Vector2D direction;

	//Set
//	inline void SetOrigin(Point2D ori) {origin = ori;};
//	inline void SetDirection(Vector2D dir) {direction = dir;};
	//Get
//	inline Point2D origin {return origin;};
//	inline Vector2D direction {return direction;};

    CLASSIFY ClassifyToRay(Ray2D & ray);
	CLASSIFY ClassifyToRay(Ray2D & ray,Point2D & pinter,LineSegment2D & segmentoverlap,Ray2D & rayoverlap);
	bool Perpendicular(Ray2D & ray);
	bool Parallel(Ray2D & ray);
	float AngleBetween(Ray2D & ray);//in degree
	CLASSIFY ClassifyToCircle(Circle2D & cir);
	CLASSIFY ClassifyToCircle(Circle2D & cir,Point2D pinter[2]);
	bool Tangent(Circle2D & cir);
	bool Tangent(Circle2D & cir,Point2D & pinter);
	CLASSIFY ClassifyToLineSegment(LineSegment2D & lns);
	CLASSIFY ClassifyToLineSegment(LineSegment2D & lns,Point2D & pinter,LineSegment2D & segmentoverlap);
	CLASSIFY ClassifyToLine(Line2D & lin);
	CLASSIFY ClassifyToLine(Line2D & lin,Point2D & pinter);

protected:
private:
	
};

#endif