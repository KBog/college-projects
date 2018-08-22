/* Start Header ------------------------------------------------------------

Copyright (C) 2004 DigiPen Institute of Technology.
		Reproduction or disclosure of this file or its contents without the
		Prior written consent of DigiPen Institute of Technology is prohibited.

File Name:	Matrix2D.h

Purpose:	<using 2d matrices functionnality, and their mathematics transformation>

Language:	C++ compiler of Visual C++ 7.x, Intel

Platform:	Windows

Project:	Assignment 1 CS300: Utility Library

Author:	AKE development team

Creation date: <11/10/2004>

- End Header --------------------------------------------------------------*/

#ifndef _MATRIX2D_H_
#define _MATRIX2D_H_

class Matrix2D
{
public:
	float Ma00;
	float Ma01;
	float Ma02;
	float Ma10;
	float Ma11;
	float Ma12;
	float Ma20;
	float Ma21;
	float Ma22;

	Matrix2D();
	Matrix2D(const Matrix2D & ma);
	~Matrix2D();

	Matrix2D & operator= (const Matrix2D& m);
	Matrix2D operator+ (const Matrix2D & m);
	Matrix2D operator- (const Matrix2D & m);
	Matrix2D operator* (const Matrix2D & m);
	bool operator== (const Matrix2D & m);
	bool operator!= (const Matrix2D & m);
	Matrix2D & operator+= (const Matrix2D & m);
	Matrix2D & operator-= (const Matrix2D & m);
	Matrix2D & operator*= (const Matrix2D & m);

	void Clear();

	void SetScale(float  sx,float  sy);
	void SetTranslate(float  tx,float  ty);
	void SetRotateOnZ(float angle);//in degree
	void SetInverse2x2();

	void SetIdentity();
	void SetTranspose();
	float GetDeterminant2x2();
	void SetOpposite();

	//statics
	static void GetScale(Matrix2D & temp,float  sx,float  sy);
	static Matrix2D GetScale(float  sx,float  sy);
	static void GetTranslate(Matrix2D & temp,float  tx,float  ty);
	static Matrix2D GetTranslate(float  tx,float  ty);
	static void GetRotateOnZ(Matrix2D & temp,float angle);//in degree
	static Matrix2D GetRotateOnZ(float angle);//in degree
	static void GetIdentity(Matrix2D & temp);
	static Matrix2D GetIdentity();
	static Matrix2D GetTranspose(const Matrix2D & m);
	static void GetTranspose(const Matrix2D & m,Matrix2D & trans);
	static float GetDeterminant2x2(const Matrix2D & m);
	static Matrix2D GetInverse2x2(const Matrix2D & m);
	static void GetInverse2x2(const Matrix2D & m,Matrix2D & inv);
	
	//friend operators
	friend Matrix2D operator*(const Matrix2D & m1,float s);
	friend Matrix2D operator*(float s,const Matrix2D & m1);
	friend void operator*=(Matrix2D & m1,float s);
	friend void operator*=(float s,Matrix2D & m1);
	friend Vector2D operator*(const Vector2D & v,const Matrix2D & m);
	friend Vector2D operator*(const Matrix2D & m,const Vector2D & v);

protected:
private:
};

#endif