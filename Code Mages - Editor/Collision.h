
/* Defining the collision color */
#define COLLISION_COLOR_R 100
#define COLLISION_COLOR_G 100
#define COLLISION_COLOR_B 200


#ifndef _COLLISION_TYPE_
#define _COLLISION_TYPE_
/*Collision Type*/
enum Collision_Type
{
	Type_Collision_Line,
	Type_Collision_Circle,
};
#endif

#ifndef _VERTEX_STRUCT_
#define _VERTEX_STRUCT_
/*Vertex Structure*/
struct Vertex
{
	float x,y;
};
#endif

#ifndef _VECTOR_STRUCT_
#define _VECTOR_STRUCT_
/*Vector Structure*/
struct Vector
{
	float x,y;
};
#endif

#ifndef _COLLISION_DATA_
#define _COLLISION_DATA_
struct Collision_Data
{
	enum Collision_Type Type;	//Collision Data Type
	Vertex V0,V1;				//Two Vertices Represent both Collision Data Types
	Vector Normal;				//Normal in case of Line Collision Data
	Collision_Data *next;		//Next Pointer in List
};
#endif

/*Function Protoypes*/
Collision_Data *BuildCollision(Vertex V0,Vertex V1,enum Collision_Type type);
void DrawCollisionNormal(Vertex V0,Vertex V1,Vector N);