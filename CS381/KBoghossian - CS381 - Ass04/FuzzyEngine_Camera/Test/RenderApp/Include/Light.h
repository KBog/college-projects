#ifndef LIGHT_H
#define LIGHT_H

#include "Color.h"
#include "Point3D.h"
#include "Point3D.h"

enum LightType
{
	LightTypePoint,
	LightTypeSpot,
	LightTypeDirectional,
};

class Light
{
public:
	LightType LightType;
	Color Ambient;
	Color Diffuse;
    Color Specular;
    Point3D Position;
    Point3D Direction;
	float Exponent;
    float Cutoff;
	float ConstantAttenuation;
    float LinearAttenuation;
    float QuadraticAttenuation;

	Light()
	{
        LightType = LightTypeDirectional;
		Ambient = Color::Black;
		Diffuse = Color::White;
		Specular = Color::White;
		Position.X = 0;
		Position.Y = 0;
		Position.Z = 0;
		Direction.X = 0;
		Direction.Y = 0;
		Direction.Z = -1;
		Exponent = 0;
		Cutoff = 180;
		ConstantAttenuation = 1;
		LinearAttenuation = 0;
		QuadraticAttenuation = 0;
	}
};


#endif
