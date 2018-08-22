/* Start Header ------------------------------------------------------------

Copyright (C) 2004 DigiPen Institute of Technology.
		Reproduction or disclosure of this file or its contents without the
		Prior written consent of DigiPen Institute of Technology is prohibited.

File Name:	Point2D.h

Purpose:	<using 2d points functionnality, and their classification along other shapes>

Language:	C++ compiler of Visual C++ 7.x, Intel

Platform:	Windows

Project:	Assignment 1 CS300: Utility Library

Author:	AKE development team

Creation date: <11/10/2004>

- End Header --------------------------------------------------------------*/

#ifndef _POINT2D_H_
#define _POINT2D_H_

class Circle2D;
class Line2D;
class Ray2D;
class LineSegment2D;
class Triangle2D;

class Point2D
{
public:
	float X;
	float Y;

	Point2D();
	Point2D(float x,float y);
	Point2D(const Point2D & pc);
	~Point2D();

	//Set
//	inline void X = float x) {X = x;};
//	inline void Y = float y) {Y = y;};
	inline void SetXY(float x,float y) {X = x;Y = y;};
	//Get
//	inline float X {return X;};
//	inline float Y {return Y;};
	inline float GetCoordinate(int i);

	float & operator[](int i);
	const float & operator[](int i) const;

	Point2D & operator= (const Point2D & p);
	Point2D & operator= (const Vector2D & v);
	Point2D operator+ (const Vector2D & v);
	Point2D operator- (const Vector2D & v);
	Point2D operator* (float f);
	Point2D operator/ (float f);
	Point2D & operator+= (const Vector2D & v);
	Point2D & operator-= (const Vector2D & v);
	Point2D & operator*= (float f);
	Point2D & operator/= (float f);
	bool operator== (const Point2D & p);
	bool operator!= (const Point2D & p);

	float Distance(Point2D & pnt);
	CLASSIFY ClassifyToCircle(Circle2D & cir);
	bool PointOnCircle(Circle2D & cir);
	bool PointInsideCircle(Circle2D & cir);
	bool PointOutsideCircle(Circle2D & cir);
	float ProjectionOnCircle(Circle2D & cir,Point2D & pro);
	CLASSIFY ClassifyToLine(Line2D & lin);
	bool PointOnLine(Line2D & lin);
	bool PointOutsideLine(Line2D & lin);
	float ProjectionOnLine(Line2D & lin,Point2D & pro);
	CLASSIFY ClassifyToRay(Ray2D & ray);
	bool PointOnRay(Ray2D & ray);
	float ProjectionOnRay(Ray2D & ray,Point2D & pro,bool & onray);
	CLASSIFY ClassifyToLineSegment(LineSegment2D & lns);
	bool PointOnLineSegment(LineSegment2D & lns);
	float ProjectionOnLineSegment(LineSegment2D & lns,Point2D & pro,bool & onlinesegment);
	CLASSIFY ClassifyToTriangle(Triangle2D & tri);
	CLASSIFY ClassifyToTriangle(Triangle2D & tri,LineSegment2D & segmentintersect,Point2D & pointintersection);
	bool OnTriangle(Triangle2D & tri);
	bool InsideTriangle(Triangle2D & tri);
	bool OutsideTriangle(Triangle2D & tri);
	
	//static

	//friends
	friend bool operator==(const Point2D & p1,const Point2D & p2);
    friend bool operator!=(const Point2D & p1,const Point2D & p2);
    friend Point2D operator+(const Point2D & p,const Vector2D & v);
    friend Point2D operator+(const Vector2D & v,const Point2D & p);
	friend Point2D operator-(const Point2D & p,const Vector2D & v);
    friend Vector2D operator-(const Point2D & p1,const Point2D & p2);
    friend Point2D operator*(const Point2D & p,float f);
    friend Point2D operator*(float f,const Point2D & p);
	
protected:
private:	
};

inline float Point2D::GetCoordinate(int i)
{
	switch(i)
	{
	case 0:
		return X;
		break;
	case 1:
		return Y;
		break;
	}
	return X;
}

#endif