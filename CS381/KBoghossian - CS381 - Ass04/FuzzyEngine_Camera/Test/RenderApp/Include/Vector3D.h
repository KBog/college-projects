/* Start Header ------------------------------------------------------------

Copyright (C) 2004 DigiPen Institute of Technology.
		Reproduction or disclosure of this file or its contents without the
		Prior written consent of DigiPen Institute of Technology is prohibited.

File Name:	Point3D.h

Purpose:	<using 3d vectors functionnality>

Language:	C++ compiler of Visual C++ 7.x, Intel

Platform:	Windows

Project:	Assignment 1 CS300: Utility Library

Author:	AKE development team

Creation date: <11/10/2004>

- End Header --------------------------------------------------------------*/

#ifndef _VECTOR3D_H_
#define _VECTOR3D_H_

/*
class Matrix3D;

class Point3D
{
public:
    float  X;
	float  Y;
	float  Z;

	Point3D();
	Point3D(float x,float y,float z);
	Point3D(const Point3D & vc);
	~Point3D();

    float & operator[](int i);
    const float & operator[](int i) const;

	Point3D & operator= (const Point3D & v);
	Point3D operator+ (const Point3D & v);
	Point3D operator- (const Point3D & v);
	Point3D & operator*=(const Matrix3D & m);
	bool operator== (const Point3D & v);
	bool operator!= (const Point3D & v);

	void Scale(float sx,float sy,float sz);
	void Translate(float tx,float  ty,float tz);
	void RotateOnX(float angle);//in degree
	void RotateOnY(float angle);//in degree
	void RotateOnZ(float angle);//in degree
	void RotateArbitraryAxis(const Point3D & ax,float angle);//in degree

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
	float GetDotProduct(const Point3D & v);
	float GetLength();
	Point3D GetCrossProduct(const Point3D & v);

	//statics
	static void SetNormalize(Point3D & v);
	static Point3D GetNormalize(const Point3D & v);
	static float GetDotProduct(const Point3D & v1,const Point3D & v2);
	static float GetLength(const Point3D & v);
	static Point3D GetCrossProduct(const Point3D & v1,const Point3D & v2);

	//friends
	friend Point3D operator*(const Point3D & v,float s);
	friend Point3D operator*(float s,const Point3D & v);
	friend Point3D operator/(const Point3D & v,float s);
	friend void operator*=(Point3D & v,float s);
	friend void operator*=(float s,Point3D & v);
	friend void operator/=(Point3D & v,float s);
	friend Point3D operator*(const Point3D & v,const Matrix3D & m);
	friend Point3D operator*(const Matrix3D & m,const Point3D & v);
protected:
private:
};*/

#endif