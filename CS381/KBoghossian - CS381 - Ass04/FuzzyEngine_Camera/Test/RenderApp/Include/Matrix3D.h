/* Start Header ------------------------------------------------------------

Copyright (C) 2004 DigiPen Institute of Technology.
		Reproduction or disclosure of this file or its contents without the
		Prior written consent of DigiPen Institute of Technology is prohibited.

File Name:	Matrix3D.h

Purpose:	<using 3d matrices functionnality, and their mathematics transformations>

Language:	C++ compiler of Visual C++ 7.x, Intel

Platform:	Windows

Project:	Assignment 1 CS300: Utility Library

Author:	AKE development team

Creation date: <11/10/2004>

- End Header --------------------------------------------------------------*/

#ifndef _MATRIX3D_H_
#define _MATRIX3D_H_

class Point3D;

class Matrix3D
{
public:
	float Ma00;
	float Ma10;
	float Ma20;
	float Ma30;
	float Ma01;
	float Ma11;
	float Ma21;
	float Ma31;
	float Ma02;
	float Ma12;
	float Ma22;
	float Ma32;
	float Ma03;
	float Ma13;
	float Ma23;
	float Ma33;

	Matrix3D();
	Matrix3D(const Matrix3D & ma);
	~Matrix3D();

	Matrix3D & operator= (const Matrix3D& m);
	Matrix3D operator+ (const Matrix3D & m);
	Matrix3D operator- (const Matrix3D & m);
	Matrix3D operator* (const Matrix3D & m);
	bool operator== (const Matrix3D & m);
	bool operator!= (const Matrix3D & m);
	Matrix3D & operator+= (const Matrix3D & m);
	Matrix3D & operator-= (const Matrix3D & m);
	Matrix3D & operator*= (const Matrix3D & m);

	void Clear();

	void SetScale(float  sx,float  sy,float  sz);
	void SetTranslate(float  tx,float  ty,float  tz);
	void SetRotateOnX(float angle);//in degree
	void SetRotateOnY(float angle);//in degree
	void SetRotateOnZ(float angle);//in degree
	void SetRotateArbitraryAxis(const Point3D& ax,float angle);//in degree
	void SetInverseScale(float  sx,float  sy,float  sz);
	void SetInverseTranslate(float  tx,float  ty,float  tz);
	void SetInverseRotateOnX(float angle);//in degree
	void SetInverseRotateOnY(float angle);//in degree
	void SetInverseRotateOnZ(float angle);//in degree
	void SetInverse3x3();
	void SetInverse4x4();

	void SetIdentity();
	void SetTranspose();
	float GetDeterminant3x3();
	float GetDeterminant4x4();
	void SetOpposite();
	void SetTransformation(Point3D & scaling,Point3D & rotation,Point3D & translation);
	void SetPerspective(float half_width,float half_height,float nearPlane,float farPlane);
	void SetPerspectiveAsOGL(int fieldOfViewY,float aspect,float nearz,float farz);//fieldOfView indegree
	void SetToScreenAsOGL(float viewportX,float viewportY,unsigned int viewportWidth,unsigned int viewportHeight);
	void SetLookAt(Point3D & center,Point3D & target,float siderotation);
	void SetLookAtAsOGL(Point3D & center,Point3D & target,Point3D & up);
	void SetFrustumAsOGL(float left,float right,float bottom,float top,float nearz,float farz);
	void SetOrthoAsOGL(float left,float right,float bottom,float top,float nearz,float farz);
	void SetOrtho2DAsOGL(float left,float right,float bottom,float top);	

	//statics
	static void GetScale(Matrix3D & temp,float  sx,float  sy,float  sz);
	static Matrix3D GetScale(float  sx,float  sy,float  sz);
	static void GetTranslate(Matrix3D & temp,float  tx,float  ty,float  tz);
	static Matrix3D GetTranslate(float  tx,float  ty,float  tz);
	static void GetRotateOnX(Matrix3D & temp,float angle);//in degree
	static Matrix3D GetRotateOnX(float angle);//in degree
	static void GetRotateOnY(Matrix3D & temp,float angle);//in degree
	static Matrix3D GetRotateOnY(float angle);//in degree
	static void GetRotateOnZ(Matrix3D & temp,float angle);//in degree
	static Matrix3D GetRotateOnZ(float angle);//in degree
	static void GetRotateArbitraryAxis(Matrix3D & temp,Point3D ax,float angle);//in degree
	static Matrix3D GetRotateArbitraryAxis(Point3D ax,float angle);//in degree
	static void GetIdentity(Matrix3D & temp);
	static Matrix3D GetIdentity();
	static Matrix3D GetTranspose(const Matrix3D & m);
	static void GetTranspose(const Matrix3D & m,Matrix3D & trans);
	static float GetDeterminant3x3(const Matrix3D & m);
	static float GetDeterminant4x4(const Matrix3D & m);
	static Matrix3D GetInverseScale(float  sx,float  sy,float  sz);
	static void GetInverseScale(Matrix3D & m,float  sx,float  sy,float  sz);
	static Matrix3D GetInverseTranslate(float  tx,float  ty,float  tz);
	static void GetInverseTranslate(Matrix3D & m,float  tx,float  ty,float  tz);
	static Matrix3D GetInverseRotateOnX(float angle);//in degree
	static void GetInverseRotateOnX(Matrix3D & m,float angle);//in degree
	static Matrix3D GetInverseRotateOnY(float angle);//in degree
	static void GetInverseRotateOnY(Matrix3D & m,float angle);//in degree
	static Matrix3D GetInverseRotateOnZ(float angle);//in degree
	static void GetInverseRotateOnZ(Matrix3D & m,float angle);//in degree
	static Matrix3D GetInverse3x3(const Matrix3D & m);
	static void GetInverse3x3(const Matrix3D & m,Matrix3D & inv);
	static Matrix3D GetInverse4x4(Matrix3D & m);
	static void GetInverse4x4(Matrix3D & m,Matrix3D & inv);
	static Matrix3D GetTransformation(Point3D & scaling,Point3D & rotation,Point3D & translation);
	static void GetTransformation(Matrix3D & trans,Point3D & scaling,Point3D & rotation,Point3D & translation);
	static Matrix3D GetPerspective(float half_width,float half_height,float nearPlane,float farPlane );
	static void GetPerspective(Matrix3D & persp,float half_width,float half_height,float nearPlane,float farPlane );
	static Matrix3D GetPerspectiveAsOGL(int fieldOfViewY,float aspect,float nearz,float farz);//fieldOfView indegree
	static void GetPerspectiveAsOGL(Matrix3D & persp,int fieldOfViewY,float aspect,float nearz,float farz);//fieldOfView indegree
	static Matrix3D GetToScreenAsOGL(float viewportX,float viewportY,unsigned int viewportWidth,unsigned int viewportHeight);
	static void GetToScreenAsOGL(Matrix3D & toscreen,float viewportX,float viewportY,unsigned int viewportWidth,unsigned int viewportHeight);
	static Matrix3D GetLookAtAsOGL(Point3D & center,Point3D & target,Point3D & up);
	static void GetLookAtAsOGL(Matrix3D & lookat,Point3D & center,Point3D & target,Point3D & up);
	static Matrix3D GetLookAt(Point3D & center,Point3D & target,float siderotation);
	static void GetLookAt(Matrix3D & lookat,Point3D & center,Point3D & target,float siderotation);
	static Matrix3D GetFrustumAsOGL(float left,float right,float bottom,float top,float nearz,float farz);
	static void GetFrustumAsOGL(Matrix3D & fru,float left,float right,float bottom,float top,float nearz,float farz);
	static Matrix3D GetOrthoAsOGL(float left,float right,float bottom,float top,float nearz,float farz);
	static void GetOrthoAsOGL(Matrix3D & ortho,float left,float right,float bottom,float top,float nearz,float farz);
	static Matrix3D GetOrtho2DAsOGL(float left,float right,float bottom,float top);
	static void GetOrtho2DAsOGL(Matrix3D & ortho,float left,float right,float bottom,float top);

	//friend operators
	friend Matrix3D operator*(const Matrix3D & m1,float s);
	friend Matrix3D operator*(float s,const Matrix3D & m1);
	friend void operator*=(Matrix3D & m1,float s);
	friend void operator*=(float s,Matrix3D & m1);
	friend Point3D operator*(const Point3D & v,const Matrix3D & m);
	friend Point3D operator*(const Matrix3D & m,const Point3D & v);

protected:
private:
};

#endif