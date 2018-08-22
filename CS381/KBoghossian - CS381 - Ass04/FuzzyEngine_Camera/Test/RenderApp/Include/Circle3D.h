/* Start Header ------------------------------------------------------------

Copyright (C) 2004 DigiPen Institute of Technology.
		Reproduction or disclosure of this file or its contents without the
		Prior written consent of DigiPen Institute of Technology is prohibited.

File Name:	Circle3D.h 

Purpose:	<using 3d circles functionnality, and their classification along other shapes>

Language:	C++ compiler of Visual C++ 7.x, Intel

Platform:	Windows

Project:	Assignment 1 CS300: Utility Library

Author:	AKE development team

Creation date: <11/10/2004>

- End Header --------------------------------------------------------------*/

#ifndef _CIRCLE3D_H_
#define _CIRCLE3D_H_

class Circle3D
{
public:
	Circle3D();
	Circle3D(Point3D ce,float ra);
	Circle3D(const Circle3D & cir);
	~Circle3D();

	Point3D center;
	float radius;

	//Set
//	inline void SetCenter(Point3D cen) {center = cen;};
//	inline void SetRadius(float rad) {radius = rad;};
	//Get
//	inline Point3D center {return center;};
//	inline float radius {return radius;};

protected:
private:	
};

#endif