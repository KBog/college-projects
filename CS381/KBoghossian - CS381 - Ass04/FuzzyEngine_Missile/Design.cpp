#include "Standard Header.h"

/*Defining All Characters Primitives and Collisions*/

/*"TicTacMap" Primitives and Collision Data*/
Primitive TicTacMap_Primitives[5]=
{
	{Type_Line , {(float)-319,(float)80} , {(float)318,(float)80} , {0,0} , {100,100,100}},
	{Type_Line , {(float)-319,(float)-80} , {(float)318,(float)-80} , {0,0} , {100,100,100}},
	{Type_Line , {(float)-110,(float)239} , {(float)-110,(float)-239} , {0,0} , {100,100,100}},
	{Type_Line , {(float)110,(float)239} , {(float)110,(float)-238} , {0,0} , {100,100,100}},
};

Collision_Data TicTacMap_Collisions[1];

/*"Cic" Primitives and Collision Data*/
Primitive White_Primitives[4]=
{
	{Type_Line , {(float)0,(float)0} , {(float)10,(float)0} , {0,0} , {255,0,0}},
	{Type_Line , {(float)1,(float)2} , {(float)10,(float)0} , {0,0} , {255,255,0}},
	{Type_Line , {(float)1,(float)-2} , {(float)10,(float)0} , {0,0} , {255,255,0}},
	//{Type_Filled_Circle , {(float)0,(float)-0} , {(float)5,(float)-5} , {0,0} , {255,255,255}},
};

Collision_Data White_Collisions[1];

/*"Cic" Primitives and Collision Data*/
Primitive Black_Primitives[2]=
{
	{Type_Filled_Circle , {(float)0,(float)-0} , {(float)5,(float)-5} , {0,0} , {255,255,255}},
};

Collision_Data Black_Collisions[1];

/*"TicTacMap" Primitives and Collision Data*/
Primitive Cro_Primitives[3]=
{
	{Type_Line , {(float)-10,(float)10} , {(float)10,(float)-10} , {0,0} , {255,0,0}},
	{Type_Line , {(float)10,(float)10} , {(float)-10,(float)-10} , {0,0} , {255,0,0}},
};

Collision_Data Cro_Collisions[1];

/*"CheckersBoard" Primitives and Collision Data*/
Primitive Map_Primitives[10]=
{
	{Type_Line , {(float)-290,(float)220} , {(float)290,(float)220} , {0,0} , {100,100,100}},
	{Type_Line , {(float)290,(float)220} , {(float)290,(float)-220} , {0,0} , {100,100,100}},
	{Type_Line , {(float)290,(float)-220} , {(float)-290,(float)-220} , {0,0} , {100,100,100}},
	{Type_Line , {(float)-290,(float)-220} , {(float)-290,(float)220} , {0,0} , {100,100,100}},

	{Type_Filled_Circle , {(float)-200,(float)120} , {(float)55,(float)0} , {0,0} , {100,0,255}},
	{Type_Filled_Circle , {(float)-150,(float)-100} , {(float)110,(float)0} , {0,0} , {100,0,255}},
	{Type_Filled_Circle , {(float)180,(float)-90} , {(float)75,(float)0} , {0,0} , {100,0,255}},
	{Type_Filled_Circle , {(float)100,(float)120} , {(float)65,(float)0} , {0,0} , {100,0,255}},
};

Collision_Data Map_Collisions[1];

#include "Standard Header.h"

