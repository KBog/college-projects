#ifndef MATERIAL_H
#define MATERIAL_H

#include "Color.h"

class Material
{
public:
    Color Ambient;
    Color Diffuse;
	Color Specular;
    Color Emissive;
	float Shininess;

	Material(Color ambient = Color(0.2f, 0.2f, 0.2f, 1.0f),
		Color diffuse = Color(0.8f, 0.8f, 0.8f, 1.0f),
		Color specular = Color(0.0f, 0.0f, 0.0f, 1.0f),
		Color emissive = Color(0.0f, 0.0f, 0.0f, 1.0f),
		float shininess = 0.0f):
	Ambient(ambient), 
	Diffuse(diffuse),
	Specular(specular),
	Emissive(emissive),
	Shininess(shininess)
	{

	}
};


#endif