/* Start Header ------------------------------------------------------------

Copyright (C) 2004 DigiPen Institute of Technology.
		Reproduction or disclosure of this file or its contents without the
		Prior written consent of DigiPen Institute of Technology is prohibited.

File Name:	Point3D.h

Purpose:	<using 3d points functionnality, and their classification along other shapes>

Language:	C++ compiler of Visual C++ 7.x, Intel

Platform:	Windows

Project:	Assignment 1 CS300: Utility Library

Author:	AKE development team

Creation date: <11/10/2004>

- End Header --------------------------------------------------------------*/

#ifndef _POINT3D_H_
#define _POINT3D_H_

extern enum CLASSIFY;
class Line3D;
class Ray3D;
class LineSegment3D;
class Plane3D;
class Sphere3D;
class Box3D;
class Triangle3D;
class Quad3D;
class Frustum3D;
class Matrix3D;

class Point3D;
typedef Point3D Vertex;
typedef Point3D Vector3D;

class Point3D
{
public:
	float X;
	float Y;
	float Z;

	Point3D();
	Point3D(float x,float y,float z);
	Point3D(const Point3D & pc);
	~Point3D();

	//Set
/*	inline void X = float x) {X = x;};
	inline void Y = float y) {Y = y;};
	inline void Z = float z) {Z = z;};*/
	inline void SetXYZ(float x,float y,float z) {X = x;Y = y;Z = z;};
	//Get
	/*inline float X {return X;};
	inline float Y {return Y;};
	inline float Z {return Z;};*/
	inline float GetCoordinate(int i);

	float & operator[](int i);
	const float & operator[](int i) const;

	Point3D & operator= (const Point3D & p);
	Point3D operator+ (const Vector3D & v);
	Point3D operator- (const Vector3D & v);
	Point3D operator* (float f);
	Point3D operator/ (float f);
	Point3D & operator+= (const Vector3D & v);
	Point3D & operator-= (const Vector3D & v);
	Point3D & operator*= (float f);
	Point3D & operator/= (float f);
	bool operator== (const Point3D & p);
	bool operator!= (const Point3D & p);
	
	//From Vector3D
	Vector3D & operator*=(const Matrix3D & m);

	float Distance(Point3D & pnt);
	CLASSIFY ClassifyToLine(Line3D & lin);
    bool PointOnLine(Line3D & lin);
	float ProjectionOnLine(Line3D & lin,Point3D & pro);
	CLASSIFY ClassifyToRay(Ray3D & ray);
	bool PointOnRay(Ray3D & ray);
	float ProjectionOnRay(Ray3D & ray,Point3D & pro,bool & onray);
	CLASSIFY ClassifyToLineSegment(LineSegment3D & lns);
	CLASSIFY ClassifyToLineSegmentSimplified(LineSegment3D & lns);
	bool PointOnLineSegment(LineSegment3D & lns);
	float ProjectionOnLineSegment(LineSegment3D & lns,Point3D & pro,bool & onlinesegment);
	CLASSIFY ClassifyToPlane(Plane3D & pln);
	bool PointOnPlane(Plane3D & pln);
	float ProjectionOnPlane(Plane3D & pln);
	float ProjectionOnPlane(Plane3D & pln,Point3D & pro);
	CLASSIFY ClassifyToQuad(Quad3D & qua);
	CLASSIFY ClassifyToQuadCoplanar(Quad3D & qua);
	CLASSIFY ClassifyToQuad(Quad3D & qua,LineSegment3D & segmentintersection);
	CLASSIFY ClassifyToSphere(Sphere3D & sph);
	bool PointOnSphere(Sphere3D & sph);
	bool PointInsideSphere(Sphere3D & sph);
	bool PointOutsideSphere(Sphere3D & sph);
	float ProjectionOnSphere(Sphere3D & sph,Point3D & pro);
	CLASSIFY ClassifyToBox(Box3D & box);
	CLASSIFY ClassifyToBoxSimplified(Box3D & box);
	CLASSIFY ClassifyToBox(Box3D & box,int index[3]);
	CLASSIFY ClassifyToFrustum(Frustum3D & fru);
	bool PointOnBox(Box3D & box);
	bool PointInsideBox(Box3D & box);
	bool PointOutsideBox(Box3D & box);
	float ProjectionOnBox(Box3D & box,Point3D & pro,int planenum[3],int & numofplanes);
	CLASSIFY ClassifyToTriangle(Triangle3D & tri);
	CLASSIFY ClassifyToTriangle(Triangle3D & tri,LineSegment3D & segmentintersection,Point3D & pointintersection);
	bool OnTriangle(Triangle3D & tri);
	bool InsideTriangle(Triangle3D & tri);
	bool OutsideTriangle(Triangle3D & tri);

	//From Vector3D
	void Scale(float sx,float sy,float sz);
	void Translate(float tx,float  ty,float tz);
	void RotateOnX(float angle);//in degree
	void RotateOnY(float angle);//in degree
	void RotateOnZ(float angle);//in degree
	void RotateArbitraryAxis(const Vector3D & ax,float angle);//in degree
	void Clear();//set arguments to 0
	bool IsZero() const;
	void SetOppositeX();
	void SetOppositeY();
	void SetOppositeZ();
	void SetOppositeXY();
	void SetOppositeXZ();
	void SetOppositeYZ();
	void SetOppositeXYZ();
	void SetNormalize();
	float GetDotProduct(const Vector3D & v);
	float GetLength();
	Vector3D GetCrossProduct(const Vector3D & v);
	//statics
	static void SetNormalize(Vector3D & v);
	static Vector3D GetNormalize(const Vector3D & v);
	static float GetDotProduct(const Vector3D & v1,const Vector3D & v2);
	static float GetLength(const Vector3D & v);
	static Vector3D GetCrossProduct(const Vector3D & v1,const Vector3D & v2);
	
	//friends
	friend bool operator==(const Point3D & p1,const Point3D & p2);
    friend bool operator!=(const Point3D & p1,const Point3D & p2);
    friend Point3D operator+(const Point3D & p,const Vector3D & v);
    friend Point3D operator+(const Vector3D & v,const Point3D & p);
	friend Point3D operator-(const Point3D & p,const Vector3D & v);
    friend Vector3D operator-(const Point3D & p1,const Point3D & p2);
    friend Point3D operator*(const Point3D & p,float f);
    friend Point3D operator*(float f,const Point3D & p);

	//From Vector3D
	friend Vector3D operator/(const Vector3D & v,float s);
	friend void operator*=(Vector3D & v,float s);
	friend void operator*=(float s,Vector3D & v);
	friend void operator/=(Vector3D & v,float s);
	friend Vector3D operator*(const Vector3D & v,const Matrix3D & m);
	friend Vector3D operator*(const Matrix3D & m,const Vector3D & v);
	
protected:
private:	
};

inline float Point3D::GetCoordinate(int i)
{
	switch(i)
	{
	case 0:
		return X;
		break;
	case 1:
		return Y;
		break;
	case 2:
		return Z;
		break;
	}
	return X;
}

#endif