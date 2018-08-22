/* Start Header ------------------------------------------------------------

Copyright (C) 2004 DigiPen Institute of Technology.
		Reproduction or disclosure of this file or its contents without the
		Prior written consent of DigiPen Institute of Technology is prohibited.

File Name:	Plane3D.h

Purpose:	<using 3d planes functionnality, and their classification along other shapes>

Language:	C++ compiler of Visual C++ 7.x, Intel

Platform:	Windows

Project:	Assignment 1 CS300: Utility Library

Author:	AKE development team

Creation date: <11/10/2004>

- End Header --------------------------------------------------------------*/

#ifndef _PLANE3D_H_
#define _PLANE3D_H_

class Circle3D;
//class Box3D;

class Plane3D
{
public:
	Plane3D();
	Plane3D(float a,float b,float c,float d);
	Plane3D(Point3D pnt,Point3D nor);
	Plane3D(const Plane3D & pln);
	~Plane3D();

	void Init(float a,float b,float c,float d);
	void Init(Point3D pnt,Point3D nor);

	Point3D point;
	Point3D normal;
	float A;
	float B;
	float C;
	float D;

	//Set
//	inline void SetA(float a) {A = a;};
//	inline void SetB(float b) {B = b;};
//	inline void SetC(float c) {C = c;};
//	inline void SetD(float d) {D = d;};
//	inline void SetNormal(Point3D nor) {normal = nor; A=nor.X; B=nor.X; C=nor.Z; D=};
//	inline void SetPoint(Point3D poi) {point = poi;};
	//Get
/*	inline float A {return A;};
	inline float B {return B;};
	inline float C {return C;};
	inline float D {return D;};
	inline Point3D normal {return normal;};
	inline Point3D point {return point;};	*/

	float & operator[](int i);
	const float & operator[](int i) const;

	CLASSIFY ClassifyToPlane(Plane3D & pln);
	CLASSIFY ClassifyToPlane(Plane3D & pln,Line3D & linter);
	CLASSIFY ClassifyToBox(Box3D & box);
	CLASSIFY ClassifyToSphere(Sphere3D & sph);
	CLASSIFY ClassifyToSphere(Sphere3D & sph,Circle3D & cinter,Point3D & pinter);
	float AngleBetween(Plane3D & pln);
	bool Perpendicular(Plane3D & pln);
	bool Parallel(Plane3D & pln);
	
protected:
private:	
};

#endif