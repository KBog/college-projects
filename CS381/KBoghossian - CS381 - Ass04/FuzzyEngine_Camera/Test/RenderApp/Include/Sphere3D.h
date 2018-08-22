/* Start Header ------------------------------------------------------------

Copyright (C) 2004 DigiPen Institute of Technology.
		Reproduction or disclosure of this file or its contents without the
		Prior written consent of DigiPen Institute of Technology is prohibited.

File Name:	Sphere3D.h

Purpose:	<using 3d spheres functionnality, and their classification along other shapes>

Language:	C++ compiler of Visual C++ 7.x, Intel

Platform:	Windows

Project:	Assignment 1 CS300: Utility Library

Author:	AKE development team

Creation date: <11/10/2004>

- End Header --------------------------------------------------------------*/

#ifndef _SPHERE3D_H_
#define _SPHERE3D_H_

class Sphere3D
{
public:
    Sphere3D();
	Sphere3D(const Point3D & c,float r);
	Sphere3D(const Sphere3D & sph);
	~Sphere3D();

	/// The center point of the sphere.
    Point3D center;
    /// The radius of the sphere.
	float radius;

	//Set
//	inline void SetCenter(Point3D cen) {center = cen;};
//	inline void SetRadius(float rad) {radius = rad;};
	//Get
//	inline Point3D center {return center;};
//	inline float radius {return radius;};

	CLASSIFY ClassifyToSphere(Sphere3D & sph);
	CLASSIFY ClassifyToSphere(Sphere3D & sph,Circle3D & cinter,Point3D & pinter);
	CLASSIFY ClassifyToPlane(Plane3D & pln);
	CLASSIFY ClassifyToPlane(Plane3D & pln,Circle3D & cinter,Point3D & pinter);
	CLASSIFY ClassifyToBox(Box3D & box);
	CLASSIFY ClassifyToFrustum(Frustum3D & fru);

protected:
private:	
};

#endif