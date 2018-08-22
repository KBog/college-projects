/* Start Header ------------------------------------------------------------

Copyright (C) 2004 DigiPen Institute of Technology.
		Reproduction or disclosure of this file or its contents without the
		Prior written consent of DigiPen Institute of Technology is prohibited.

File Name:	Vector2D.h

Purpose:	<using 2d vectors functionnality>

Language:	C++ compiler of Visual C++ 7.x, Intel

Platform:	Windows

Project:	Assignment 1 CS300: Utility Library

Author:	AKE development team

Creation date: <11/10/2004>

- End Header --------------------------------------------------------------*/

#ifndef _VECTOR2D_H_
#define _VECTOR2D_H_

class Point3D;
class Matrix2D;

class Vector2D
{
public:
	float  X;
	float  Y;

	Vector2D();
	Vector2D(float x,float y);
	Vector2D(const Vector2D & vc);
	~Vector2D();

	float & operator[](int i);
    const float & operator[](int i) const;

	Vector2D & operator= (const Vector2D & v);
	Vector2D operator+ (const Vector2D & v);
	Vector2D operator- (const Vector2D & v);
	bool operator== (const Vector2D & v);
	bool operator!= (const Vector2D & v);
	Vector2D & operator= (const Point3D & v);

	void Scale(float  sx,float  sy);
	void Translate(float  tx,float  ty);
	void Rotate(float angle);//in degree

	void Clear();//set arguments to 0
	bool IsZero() const;
	void SetOppositeX();
	void SetOppositeY();
	void SetOppositeXY();

	Vector2D GetNormal();
	Vector2D GetReflectedVector(const Vector2D & vn);
	void SetNormalize();
	float GetDotProduct(const Vector2D & v);
	float GetLength();
	
	//statics
	static Vector2D GetNormal(const Vector2D & v);
	static Vector2D GetReflectedVector(const Vector2D & v1,const Vector2D & vn);
	static void SetNormalize(Vector2D & v);
	static Vector2D GetNormalize(const Vector2D & v);
	static float GetDotProduct(const Vector2D & v1,const Vector2D & v2);
	static float GetLength(const Vector2D & v);

	//friends
	friend Vector2D operator*(const Vector2D & v,float s);
	friend Vector2D operator*(float s,const Vector2D & v);
	friend Vector2D operator/(const Vector2D & v,float s);
	friend void operator*=(Vector2D & v,float s);
	friend void operator*=(float s,Vector2D & v);
	friend void operator/=(Vector2D & v,float s);
	friend Vector2D operator*(const Vector2D & v,const Matrix2D & m);
	friend Vector2D operator*(const Matrix2D & m,const Vector2D & v);

protected:
private:
};

#endif