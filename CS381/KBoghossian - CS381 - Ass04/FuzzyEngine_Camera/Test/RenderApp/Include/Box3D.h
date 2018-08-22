/* Start Header ------------------------------------------------------------

Copyright (C) 2004 DigiPen Institute of Technology.
		Reproduction or disclosure of this file or its contents without the
		Prior written consent of DigiPen Institute of Technology is prohibited.

File Name:	Box3D.h 

Purpose:	<using 3d boxes functionnality, and their classification along other shapes>

Language:	C++ compiler of Visual C++ 7.x, Intel

Platform:	Windows

Project:	Assignment 1 CS300: Utility Library

Author:	AKE development team

Creation date: <11/10/2004>

- End Header --------------------------------------------------------------*/

#ifndef _BOX3D_H_
#define _BOX3D_H_

//class Frustum3D;

//Box are considered to be concave and 90 degrees angles
class Box3D
{
public:
    Box3D();
    Box3D(const Point3D coords[8],const Point3D & cen);
	Box3D(const Point3D coords[8]);
	Box3D(const Box3D & box);
	~Box3D();

	Point3D center;
	//to see about the following
	Point3D ptA;	//(1,1,1)
	Point3D ptB;	//(-1,1,1)
	Point3D ptC;	//(-1,1,-1)
	Point3D ptD;	//(1,1,-1)
	Point3D ptE;	//(1,-1,1)
	Point3D ptF;	//(-1,-1,1)
	Point3D ptG;	//(-1,-1,-1)
	Point3D ptH;	//(1,-1,-1)
	Plane3D planes[6];	

	//Set
/*	inline void SetCenter(Point3D cen);
	inline void SetPtA(Point3D pta);
	inline void SetPtB(Point3D ptb);
	inline void SetPtC(Point3D ptc);
	inline void SetPtD(Point3D ptd);
	inline void SetPtE(Point3D pte);
	inline void SetPtF(Point3D ptf);
	inline void SetPtG(Point3D ptg);
	inline void SetPtH(Point3D pth);
	//Get
	inline Point3D center {return center;};
	inline Point3D GetPtA() {return ptA;};
	inline Point3D GetPtB() {return ptB;};
	inline Point3D GetPtC() {return ptC;};
	inline Point3D GetPtD() {return ptD;};
	inline Point3D GetPtE() {return ptE;};
	inline Point3D GetPtF() {return ptF;};
	inline Point3D GetPtG() {return ptG;};
	inline Point3D GetPtH() {return ptH;};*/
	inline Plane3D GetPlane(int i) {if(i < 0 || i > 5) return Plane3D(); return planes[i];};
	inline Point3D GetPoint(int ind);

	CLASSIFY ClassifyToBox(Box3D & box);
	CLASSIFY ClassifyToFrustum(Frustum3D & fru);
	CLASSIFY ClassifyToPlane(Plane3D & pln);
	CLASSIFY ClassifyToSphere(Sphere3D & sph);
	void GetMaxMin(Point3D & pmin,Point3D & pmax);
	void GetWrappedSphere(Sphere3D & sph);
	
protected:
private:
	//Top plane ABCD		(0)
	//Bottom plane EFGH		(1)
	//Front plane EFBA		(2)
	//Back plane GHDC		(3)
	//Right plane EADH		(4)
	//Left plane FGCB		(5)
	
    
};

#endif