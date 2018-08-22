/* Start Header ------------------------------------------------------------

Copyright (C) 2004 DigiPen Institute of Technology.
		Reproduction or disclosure of this file or its contents without the
		Prior written consent of DigiPen Institute of Technology is prohibited.

File Name:	Circle2D.h 

Purpose:	<using 2d circles functionnality, and their classification along other shapes>

Language:	C++ compiler of Visual C++ 7.x, Intel

Platform:	Windows

Project:	Assignment 1 CS300: Utility Library

Author:	AKE development team

Creation date: <11/10/2004>

- End Header --------------------------------------------------------------*/

#ifndef _CIRCLE2D_H_
#define _CIRCLE2D_H_

class Circle2D
{
public:
	Circle2D();
	Circle2D(Point2D ce,float ra);
	Circle2D(const Circle2D & cir);
	~Circle2D();

	Point2D center;
	float radius;

	//Set
//	inline void SetCenter(Point2D cen) {center = cen;};
//	inline void SetRadius(float rad) {radius = rad;};
	//Get
//	inline Point2D center {return center;};
//	inline float radius {return radius;};

	CLASSIFY ClassifyToCircle(Circle2D & cir);
	CLASSIFY ClassifyToCircle(Circle2D & cir,Point2D pinter[2]);

protected:
private:	
};

#endif