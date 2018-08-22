/* Start Header ------------------------------------------------------------

Copyright (C) 2004 DigiPen Institute of Technology.
		Reproduction or disclosure of this file or its contents without the
		Prior written consent of DigiPen Institute of Technology is prohibited.

File Name:	Point4D.h

Purpose:	<using 4d points functionnality, and their classification along other shapes>

Language:	C++ compiler of Visual C++ 7.x, Intel

Platform:	Windows

Project:	Assignment 1 CS300: Utility Library

Author:	AKE development team

Creation date: <11/10/2004>

- End Header --------------------------------------------------------------*/

#ifndef _POINT4D_H_
#define _POINT4D_H_

class Point4D
{
public:
	float X;
	float Y;
	float Z;
	float W;

	Point4D();
	Point4D(float x,float y,float z,float w);
	Point4D(const Point4D & pc);
	~Point4D();

	//Set
/*	inline void X = float x) {X = x;};
	inline void Y = float y) {Y = y;};
	inline void Z = float z) {Z = z;};
	inline void SetW(float w) {W = w;};*/
	inline void SetXYZW(float x,float y,float z,float w) {X = x;Y = y;Z = z;W = w;};
	//Get
/*	inline float X {return X;};
	inline float Y {return Y;};
	inline float Z {return Z;};
	inline float GetW() {return W;};*/
	inline float GetCoordinate(int i);

	float & operator[](int i);
	const float & operator[](int i) const;

	Point4D & operator= (const Point4D & p);
	Point4D & operator= (const Vector4D & v);
	Point4D operator+ (const Vector4D & v);
	Point4D operator- (const Vector4D & v);
	Point4D operator* (float f);
	Point4D operator/ (float f);
	Point4D & operator+= (const Vector4D & v);
	Point4D & operator-= (const Vector4D & v);
	Point4D & operator*= (float f);
	Point4D & operator/= (float f);
	bool operator== (const Point4D & p);
	bool operator!= (const Point4D & p);
	
	//static

	//friends
	friend bool operator==(const Point4D & p1,const Point4D & p2);
    friend bool operator!=(const Point4D & p1,const Point4D & p2);
    friend Point4D operator+(const Point4D & p,const Vector4D & v);
    friend Point4D operator+(const Vector4D & v,const Point4D & p);
	friend Point4D operator-(const Point4D & p,const Vector4D & v);
    friend Vector4D operator-(const Point4D & p1,const Point4D & p2);
    friend Point4D operator*(const Point4D & p,float f);
    friend Point4D operator*(float f,const Point4D & p);
	
protected:
private:	
};

inline float Point4D::GetCoordinate(int i)
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
	case 3:
		return W;
		break;
	}
	return X;
}

#endif