/* Start Header ------------------------------------------------------------

Copyright (C) 2004 DigiPen Institute of Technology.
		Reproduction or disclosure of this file or its contents without the
		Prior written consent of DigiPen Institute of Technology is prohibited.

File Name:	Vector4D.h

Purpose:	<using 4d vectors functionnality>

Language:	C++ compiler of Visual C++ 7.x, Intel

Platform:	Windows

Project:	Assignment 1 CS300: Utility Library

Author:	AKE development team

Creation date: <11/10/2004>

- End Header --------------------------------------------------------------*/

#ifndef _VECTOR4D_H_
#define _VECTOR4D_H_

//#include "Matrix3D.h"

class Vector4D
{
public:
   float X;
   float Y;
   float Z;
   float W;

   Vector4D();
   Vector4D(float x,float y,float z,float w);
   Vector4D(const Vector4D & vc);
   ~Vector4D();

   float & operator[](int i);
   const float & operator[](int i) const;

   Vector4D & operator= (const Vector4D & v);
   Vector4D operator+ (const Vector4D & v);
   Vector4D operator- (const Vector4D & v);
   Vector4D & operator*=(const Matrix3D & m);
   bool operator== (const Vector4D & v);
   bool operator!= (const Vector4D & v);
   
   void Scale(float sx,float sy,float sz);
   void Translate(float tx,float  ty,float tz);
   void RotateOnX(float angle);//in degree
   void RotateOnY(float angle);//in degree
   void RotateOnZ(float angle);//in degree
   void RotateArbitraryAxis(const Vector4D & ax,float angle);//in degree

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
   float GetDotProduct(const Vector4D & v);
   float GetLength();
   Vector4D GetCrossProduct(const Vector4D & v);
      
   //statics
   static void SetNormalize(Vector4D & v);
   static Vector4D GetNormalize(const Vector4D & v);
   static float GetDotProduct(const Vector4D & v1,const Vector4D & v2);
   static float GetLength(const Vector4D & v);
   static Vector4D GetCrossProduct(const Vector4D & v1,const Vector4D & v2);
 
   //friends
   friend Vector4D operator*(const Vector4D & v,float s);
   friend Vector4D operator*(float s,const Vector4D & v);
   friend Vector4D operator/(const Vector4D & v,float s);
   friend void operator*=(Vector4D & v,float s);
   friend void operator*=(float s,Vector4D & v);
   friend void operator/=(Vector4D & v,float s);
   friend Vector4D operator*(const Vector4D & v,const Matrix3D & m);
   friend Vector4D operator*(const Matrix3D & m,const Vector4D & v);
};

#endif