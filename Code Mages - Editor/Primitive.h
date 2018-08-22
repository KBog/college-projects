#ifndef PRIMITIVE_HEADER_
#define PRIMITIVE_HEADER_
/*Type Enumeration*/
enum Primitive_Type
{
	Type_Line,
	Type_Circle,
	Type_Filled_Circle,
	Type_Triangle,
	Type_Filled_Triangle,
};

#ifndef _VERTEX_STRUCT_
#define _VERTEX_STRUCT_
/*Vertex Structure*/
struct Vertex
{
	float x,y;
};
#endif

#ifndef _COLOR_STRUCT_
#define _COLOR_STRUCT_
/*Color Structure*/
struct Color_RGB
{
	unsigned char r,g,b;
};
#endif

/*Primitive Structure*/
struct Primitive
{
	Vertex *V;					//Primitive Vertices
	enum Primitive_Type Type;	//Primitive Type
	Color_RGB Color;			//Primitive Color
	Primitive *next;			//Next Pointer in List
};

/*Function Prototypes*/
Primitive *BuildPrimitive(Vertex *Vertices,Color_RGB Color,enum Primitive_Type type);
#endif