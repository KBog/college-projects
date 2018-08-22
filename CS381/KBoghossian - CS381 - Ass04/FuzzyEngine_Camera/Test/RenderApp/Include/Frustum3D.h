/* Start Header ------------------------------------------------------------

Copyright (C) 2004 DigiPen Institute of Technology.
		Reproduction or disclosure of this file or its contents without the
		Prior written consent of DigiPen Institute of Technology is prohibited.

File Name:	Frustum3D.h 

Purpose:	<using 3d frustums functionnality, and their classification along other shapes>

Language:	C++ compiler of Visual C++ 7.x, Intel

Platform:	Windows

Project:	Assignment 1 CS300: Utility Library

Author:	AKE development team

Creation date: <11/10/2004>

- End Header --------------------------------------------------------------*/

#ifndef _FRUSTUM_H_
#define _FRUSTUM_H_

//#include "Quad3D.h"

class Frustum3D
{
public:
    Frustum3D();
    Frustum3D(const Point3D coords[8],const Point3D & cen);
	Frustum3D(const Point3D coords[8]);
	Frustum3D(const Frustum3D & fru);
	~Frustum3D();

	Point3D ptA;	//(1,1,1)
	Point3D ptB;	//(-1,1,1)
	Point3D ptC;	//(-1,1,-1)
	Point3D ptD;	//(1,1,-1)
	Point3D ptE;	//(1,-1,1)
	Point3D ptF;	//(-1,-1,1)
	Point3D ptG;	//(-1,-1,-1)
	Point3D ptH;	//(1,-1,-1)
	Plane3D planes[6];
	Quad3D quads[6];

	//Set
	//inline void SetCenter(Point3D cen);
/*	inline void SetPtA(Point3D pta);
	inline void SetPtB(Point3D ptb);
	inline void SetPtC(Point3D ptc);
	inline void SetPtD(Point3D ptd);
	inline void SetPtE(Point3D pte);
	inline void SetPtF(Point3D ptf);
	inline void SetPtG(Point3D ptg);
	inline void SetPtH(Point3D pth);
	//Get
	//inline Point3D center {return center;};
	inline Point3D GetPtA() {return ptA;};
	inline Point3D GetPtB() {return ptB;};
	inline Point3D GetPtC() {return ptC;};
	inline Point3D GetPtD() {return ptD;};
	inline Point3D GetPtE() {return ptE;};
	inline Point3D GetPtF() {return ptF;};
	inline Point3D GetPtG() {return ptG;};
	inline Point3D GetPtH() {return ptH;};*/
	inline Plane3D GetPlane(int i) {if(i < 0 || i > 5) return Plane3D(); return planes[i];};
	inline Quad3D GetQuad(int i) {if(i < 0 || i > 5) return Quad3D(); return quads[i];};
	inline Point3D GetPoint(int ind);

	CLASSIFY ClassifyToBox(Box3D & box);
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
	
   // Point3D center;
	//to see about the following
	
};

//inline void Frustum3D::SetCenter(Point3D cen)
//{
//	center = cen;
//}
/*
inline void Frustum3D::SetPtA(Point3D pta)
{
	ptA = pta;
	//Top plane (face)
	//ABCD
	planes[0].Init(ptA,Point3D::GetCrossProduct(Point3D(ptD.X-ptA.X,ptD.Y-ptA.Y,ptD.Z-ptA.Z),Point3D(ptB.X-ptA.X,ptB.Y-ptA.Y,ptB.Z-ptA.Z)));
	
	//Front plane (face)
	//EFBA
	planes[2].Init(ptE,Point3D::GetCrossProduct(Point3D(ptA.X-ptE.X,ptA.Y-ptE.Y,ptA.Z-ptE.Z),Point3D(ptF.X-ptE.X,ptF.Y-ptE.Y,ptF.Z-ptE.Z)));

	//Right plane (face)
	//EADH
	planes[4].Init(ptE,Point3D::GetCrossProduct(Point3D(ptH.X-ptE.X,ptH.Y-ptE.Y,ptH.Z-ptE.Z),Point3D(ptA.X-ptE.X,ptA.Y-ptE.Y,ptA.Z-ptE.Z)));

	//quads
	quads[0].Init(ptA,ptD,ptC,ptB);
	quads[2].Init(ptE,ptA,ptB,ptF);
	quads[4].Init(ptE,ptH,ptD,ptA);
}

inline void Frustum3D::SetPtB(Point3D ptb)
{
	ptB = ptb;
	//Top plane (face)
	//ABCD
	planes[0].Init(ptA,Point3D::GetCrossProduct(Point3D(ptD.X-ptA.X,ptD.Y-ptA.Y,ptD.Z-ptA.Z),Point3D(ptB.X-ptA.X,ptB.Y-ptA.Y,ptB.Z-ptA.Z)));

	//Front plane (face)
	//EFBA
	planes[2].Init(ptE,Point3D::GetCrossProduct(Point3D(ptA.X-ptE.X,ptA.Y-ptE.Y,ptA.Z-ptE.Z),Point3D(ptF.X-ptE.X,ptF.Y-ptE.Y,ptF.Z-ptE.Z)));
	
	//Left plane (face)
	//FGCB
	planes[5].Init(ptF,Point3D::GetCrossProduct(Point3D(ptB.X-ptF.X,ptB.Y-ptF.Y,ptB.Z-ptF.Z),Point3D(ptG.X-ptF.X,ptG.Y-ptF.Y,ptG.Z-ptF.Z)));

	//quads
	quads[0].Init(ptA,ptD,ptC,ptB);
	quads[2].Init(ptE,ptA,ptB,ptF);
	quads[5].Init(ptF,ptB,ptC,ptG);
}

inline void Frustum3D::SetPtC(Point3D ptc)
{
	ptC = ptc;
	//Top plane (face)
	//ABCD
	planes[0].Init(ptA,Point3D::GetCrossProduct(Point3D(ptD.X-ptA.X,ptD.Y-ptA.Y,ptD.Z-ptA.Z),Point3D(ptB.X-ptA.X,ptB.Y-ptA.Y,ptB.Z-ptA.Z)));

	//Back plane (face)
	//GHDC
	planes[3].Init(ptG,Point3D::GetCrossProduct(Point3D(ptC.X-ptG.X,ptC.Y-ptG.Y,ptC.Z-ptG.Z),Point3D(ptH.X-ptG.X,ptH.Y-ptG.Y,ptH.Z-ptG.Z)));
	
	//Left plane (face)
	//FGCB
	planes[5].Init(ptF,Point3D::GetCrossProduct(Point3D(ptB.X-ptF.X,ptB.Y-ptF.Y,ptB.Z-ptF.Z),Point3D(ptG.X-ptF.X,ptG.Y-ptF.Y,ptG.Z-ptF.Z)));

	//quads
	quads[0].Init(ptA,ptD,ptC,ptB);
	quads[3].Init(ptG,ptC,ptD,ptH);
	quads[5].Init(ptF,ptB,ptC,ptG);
}

inline void Frustum3D::SetPtD(Point3D ptd)
{
	ptD = ptd;
	//Top plane (face)
	//ABCD
	planes[0].Init(ptA,Point3D::GetCrossProduct(Point3D(ptD.X-ptA.X,ptD.Y-ptA.Y,ptD.Z-ptA.Z),Point3D(ptB.X-ptA.X,ptB.Y-ptA.Y,ptB.Z-ptA.Z)));
	
	//Back plane (face)
	//GHDC
	planes[3].Init(ptG,Point3D::GetCrossProduct(Point3D(ptC.X-ptG.X,ptC.Y-ptG.Y,ptC.Z-ptG.Z),Point3D(ptH.X-ptG.X,ptH.Y-ptG.Y,ptH.Z-ptG.Z)));
	
	//Right plane (face)
	//EADH
	planes[4].Init(ptE,Point3D::GetCrossProduct(Point3D(ptH.X-ptE.X,ptH.Y-ptE.Y,ptH.Z-ptE.Z),Point3D(ptA.X-ptE.X,ptA.Y-ptE.Y,ptA.Z-ptE.Z)));

	//quads
	quads[0].Init(ptA,ptD,ptC,ptB);
	quads[3].Init(ptG,ptC,ptD,ptH);
	quads[4].Init(ptE,ptH,ptD,ptA);
}

inline void Frustum3D::SetPtE(Point3D pte)
{
	ptE = pte;
	//Bottom plane (face)
	//EFGH
	planes[1].Init(ptE,Point3D::GetCrossProduct(Point3D(ptF.X-ptE.X,ptF.Y-ptE.Y,ptF.Z-ptE.Z),Point3D(ptH.X-ptE.X,ptH.Y-ptE.Y,ptH.Z-ptE.Z)));

	//Front plane (face)
	//EFBA
	planes[3].Init(ptE,Point3D::GetCrossProduct(Point3D(ptA.X-ptE.X,ptA.Y-ptE.Y,ptA.Z-ptE.Z),Point3D(ptF.X-ptE.X,ptF.Y-ptE.Y,ptF.Z-ptE.Z)));
	
	//Right plane (face)
	//EADH
	planes[4].Init(ptE,Point3D::GetCrossProduct(Point3D(ptH.X-ptE.X,ptH.Y-ptE.Y,ptH.Z-ptE.Z),Point3D(ptA.X-ptE.X,ptA.Y-ptE.Y,ptA.Z-ptE.Z)));

	//quads
	quads[1].Init(ptE,ptF,ptG,ptH);
	quads[3].Init(ptG,ptC,ptD,ptH);
	quads[4].Init(ptE,ptH,ptD,ptA);
}

inline void Frustum3D::SetPtF(Point3D ptf)
{
	ptF = ptf;
	//Bottom plane (face)
	//EFGH
	planes[1].Init(ptE,Point3D::GetCrossProduct(Point3D(ptF.X-ptE.X,ptF.Y-ptE.Y,ptF.Z-ptE.Z),Point3D(ptH.X-ptE.X,ptH.Y-ptE.Y,ptH.Z-ptE.Z)));
	
	//Front plane (face)
	//EFBA
	planes[2].Init(ptE,Point3D::GetCrossProduct(Point3D(ptA.X-ptE.X,ptA.Y-ptE.Y,ptA.Z-ptE.Z),Point3D(ptF.X-ptE.X,ptF.Y-ptE.Y,ptF.Z-ptE.Z)));
	
	//Left plane (face)
	//FGCB
	planes[5].Init(ptF,Point3D::GetCrossProduct(Point3D(ptB.X-ptF.X,ptB.Y-ptF.Y,ptB.Z-ptF.Z),Point3D(ptG.X-ptF.X,ptG.Y-ptF.Y,ptG.Z-ptF.Z)));

	//quads
	quads[1].Init(ptE,ptF,ptG,ptH);
	quads[2].Init(ptE,ptA,ptB,ptF);
	quads[5].Init(ptF,ptB,ptC,ptG);
}

inline void Frustum3D::SetPtG(Point3D ptg)
{
	ptG = ptg;
	//Bottom plane (face)
	//EFGH
	planes[1].Init(ptE,Point3D::GetCrossProduct(Point3D(ptF.X-ptE.X,ptF.Y-ptE.Y,ptF.Z-ptE.Z),Point3D(ptH.X-ptE.X,ptH.Y-ptE.Y,ptH.Z-ptE.Z)));
	
	//Back plane (face)
	//GHDC
	planes[3].Init(ptG,Point3D::GetCrossProduct(Point3D(ptC.X-ptG.X,ptC.Y-ptG.Y,ptC.Z-ptG.Z),Point3D(ptH.X-ptG.X,ptH.Y-ptG.Y,ptH.Z-ptG.Z)));
	
	//Left plane (face)
	//FGCB
	planes[5].Init(ptF,Point3D::GetCrossProduct(Point3D(ptB.X-ptF.X,ptB.Y-ptF.Y,ptB.Z-ptF.Z),Point3D(ptG.X-ptF.X,ptG.Y-ptF.Y,ptG.Z-ptF.Z)));

	//quads
	quads[1].Init(ptE,ptF,ptG,ptH);
	quads[3].Init(ptG,ptC,ptD,ptH);
	quads[5].Init(ptF,ptB,ptC,ptG);
}

inline void Frustum3D::SetPtH(Point3D pth)
{
	ptH = pth;
	//Bottom plane (face)
	//EFGH
	planes[1].Init(ptE,Point3D::GetCrossProduct(Point3D(ptF.X-ptE.X,ptF.Y-ptE.Y,ptF.Z-ptE.Z),Point3D(ptH.X-ptE.X,ptH.Y-ptE.Y,ptH.Z-ptE.Z)));
	
	//Back plane (face)
	//GHDC
	planes[3].Init(ptG,Point3D::GetCrossProduct(Point3D(ptC.X-ptG.X,ptC.Y-ptG.Y,ptC.Z-ptG.Z),Point3D(ptH.X-ptG.X,ptH.Y-ptG.Y,ptH.Z-ptG.Z)));
	
	//Right plane (face)
	//EADH
	planes[4].Init(ptE,Point3D::GetCrossProduct(Point3D(ptH.X-ptE.X,ptH.Y-ptE.Y,ptH.Z-ptE.Z),Point3D(ptA.X-ptE.X,ptA.Y-ptE.Y,ptA.Z-ptE.Z)));

	//quads
	quads[1].Init(ptE,ptF,ptG,ptH);
	quads[3].Init(ptG,ptC,ptD,ptH);
	quads[4].Init(ptE,ptH,ptD,ptA);
}
*/
inline Point3D Frustum3D::GetPoint(int ind)//0-based (0-7)
{
	switch(ind)
	{
	case 0:
		return ptA;
		break;
	case 1:
		return ptB;
		break;
	case 2:
		return ptC;
		break;
	case 3:
		return ptD;
		break;
	case 4:
		return ptE;
		break;
	case 5:
		return ptF;
		break;
	case 6:
		return ptG;
		break;
	case 7:
		return ptH;
		break;
	default:
		return ptA;
	}
}

#endif