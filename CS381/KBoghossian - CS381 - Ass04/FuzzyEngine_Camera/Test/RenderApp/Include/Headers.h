/* Start Header ------------------------------------------------------------

Copyright (C) 2004 DigiPen Institute of Technology.
		Reproduction or disclosure of this file or its contents without the
		Prior written consent of DigiPen Institute of Technology is prohibited.

File Name:	Headers.h 

Purpose:	<envelop all used headers in the project, contain global data and functions, used in most of the files>

Language:	C++ compiler of Visual C++ 7.x, Intel

Platform:	Windows

Project:	Assignment 1 CS300: Utility Library

Author:	AKE development team

Creation date: <11/10/2004>

- End Header --------------------------------------------------------------*/

#ifndef _HEADERS_H_
#define _HEADERS_H_

#include <iostream>
#include <stdio.h>
#include <math.h>
#include <limits>
#include <utility>

enum CLASSIFY 
{
	INTERSECT,
	NO_INTERSECT,
	OUTSIDE,
	INSIDE,
	LEFT,
	RIGHT,
	BEHIND,
	BEYOND,
	FRONT,
	BACK,
	INTERSECT_ORIGIN,
	INTERSECT_A,
	INTERSECT_B,
	TANGENT,
	ONE_POINT,
	TWO_POINTS,
	ONE_FACE_INTERSECT,
	ONE_EDGE_INTERSECT,
	ONE_CORNER_INTERSECT,
	OVERLAP,
	PARALLEL,
	ALLIGNED,
	SEGMENT_OVERLAP,
	RAY_OVERLAP,
	SEGMENT_INTERSECT,
	POINT_INTERSECT,
	ENVELOP,
	INTERN_TANGENT,
	EXTERN_TANGENT,
	BACK_INTERSECT,
	FRONT_INTERSECT,
	EXTERN_TANGENT_AND_ENVELOP,
	FUN_ERROR,
	FRONT_TANGENT,
	BACK_TANGENT,
	SEGMENT_POINT_INTERSECT,
	ENVELOP_TANGENT,
	INSIDE_INTERSECT,
	ONE_POINT_INTERSECT,
	ONE_SEGMENT_INTERSECT,
	TWO_POINTS_INTERSECT,
	TWO_SEGMENT_INTERSECT,	
};

#include "Vector2D.h"
#include "Point3D.h"
#include "Vector3D.h"
#include "Vector4D.h"
#include "Matrix2D.h"
#include "Matrix3D.h"
#include "Point2D.h"
#include "Point4D.h"
#include "Line2D.h"
#include "Line3D.h"
#include "Plane3D.h"
#include "LineSegment2D.h"
#include "LineSegment3D.h"
#include "Ray2D.h"
#include "Ray3D.h"
#include "Box3D.h"
#include "Sphere3D.h"
#include "Triangle2D.h"
#include "Triangle3D.h"
#include "Quad3D.h"
#include "Circle2D.h"
#include "Circle3D.h"
#include "Frustum3D.h"



//A small number (useful for comparing floating-point numbers to zero.)
const float epsilon = std::numeric_limits< float>::epsilon();
const double PI = 3.1415926535897932384626433832795l;

#define FUN_EPSILON	0.0005f

#define Minimum(a,b) (((a) < (b)) ? (a) : (b))
#define Maximum(a,b) (((a) > (b)) ? (a) : (b))

#endif