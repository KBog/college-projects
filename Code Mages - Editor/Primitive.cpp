#include"Primitive.h"

Primitive *BuildPrimitive(Vertex *Vertices,Color_RGB Color,enum Primitive_Type type)
{
	/*Building New Primitive*/
	Primitive* p=new Primitive;
	int Num=2;
	p->Color=Color;
	p->Type=type;
	p->next=0;

	/*In case Primitive was a Triangle*/
	if(type>=Type_Triangle)
		Num=3;
	
	/*Copy Primitive Vertices*/
	p->V=new Vertex[Num];
	for(int i=0;i<Num;i++)
		p->V[i]=Vertices[i];

	/*Return Built Primitive*/
	return p;
}