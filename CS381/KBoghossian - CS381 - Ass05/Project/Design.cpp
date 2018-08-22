#include "Standard Header.h"

/*Defining All Characters Primitives and Collisions*/

/*"Cic" Primitives and Collision Data*/
Primitive White_Primitives[2]=
{
	{Type_Filled_Circle , {(float)0,(float)-0} , {(float)20,(float)-20} , {0,0} , {255,255,255}},
};

Collision_Data White_Collisions[1];

/*"Cic" Primitives and Collision Data*/
Primitive Black_Primitives[2]=
{
	{Type_Filled_Circle , {(float)0,(float)-0} , {(float)10,(float)-10} , {0,0} , {0,0,0}},
};

Collision_Data Black_Collisions[1];

/*"TicTacMap" Primitives and Collision Data*/
Primitive Cro_Primitives[3]=
{
	{Type_Line , {(float)-10,(float)10} , {(float)10,(float)-10} , {0,0} , {255,0,0}},
	{Type_Line , {(float)10,(float)10} , {(float)-10,(float)-10} , {0,0} , {255,0,0}},
};

Collision_Data Cro_Collisions[1];

/*"Board" Primitives and Collision Data*/
Primitive Board_Primitives[25]=
{
	{Type_Line , {(float)-260,(float)200} , {(float)220,(float)200} , {0,0} , {100,100,100}},
	{Type_Line , {(float)220,(float)200} , {(float)220,(float)-200} , {0,0} , {100,100,100}},
	{Type_Line , {(float)220,(float)-200} , {(float)-260,(float)-200} , {0,0} , {100,100,100}},
	{Type_Line , {(float)-260,(float)-200} , {(float)-260,(float)200} , {0,0} , {100,100,100}},
	{Type_Line , {(float)-221,(float)200} , {(float)-221,(float)-200} , {0,0} , {100,100,100}},
	{Type_Line , {(float)-180,(float)-200} , {(float)-180,(float)200} , {0,0} , {100,100,100}},
	{Type_Line , {(float)-140,(float)-200} , {(float)-140,(float)200} , {0,0} , {100,100,100}},
	{Type_Line , {(float)-100,(float)200} , {(float)-100,(float)-200} , {0,0} , {100,100,100}},
	{Type_Line , {(float)-60,(float)-200} , {(float)-60,(float)200} , {0,0} , {100,100,100}},
	{Type_Line , {(float)-20,(float)200} , {(float)-20,(float)-200} , {0,0} , {100,100,100}},
	{Type_Line , {(float)20,(float)-200} , {(float)20,(float)200} , {0,0} , {100,100,100}},
	{Type_Line , {(float)60,(float)199} , {(float)60,(float)-200} , {0,0} , {100,100,100}},
	{Type_Line , {(float)100,(float)-200} , {(float)100,(float)200} , {0,0} , {100,100,100}},
	{Type_Line , {(float)140,(float)-200} , {(float)140,(float)200} , {0,0} , {100,100,100}},
	{Type_Line , {(float)180,(float)-200} , {(float)180,(float)200} , {0,0} , {100,100,100}},
	{Type_Line , {(float)-260,(float)161} , {(float)220,(float)161} , {0,0} , {100,100,100}},
	{Type_Line , {(float)220,(float)120} , {(float)-260,(float)120} , {0,0} , {100,100,100}},
	{Type_Line , {(float)-260,(float)80} , {(float)220,(float)80} , {0,0} , {100,100,100}},
	{Type_Line , {(float)220,(float)40} , {(float)-260,(float)40} , {0,0} , {100,100,100}},
	{Type_Line , {(float)-260,(float)1} , {(float)220,(float)1} , {0,0} , {100,100,100}},
	{Type_Line , {(float)220,(float)-40} , {(float)-260,(float)-40} , {0,0} , {100,100,100}},
	{Type_Line , {(float)-260,(float)-80} , {(float)220,(float)-80} , {0,0} , {100,100,100}},
	{Type_Line , {(float)220,(float)-120} , {(float)-260,(float)-120} , {0,0} , {100,100,100}},
	{Type_Line , {(float)-260,(float)-160} , {(float)220,(float)-160} , {0,0} , {100,100,100}},
};

Collision_Data Board_Collisions[1];

/*"Square" Primitives and Collision Data*/
Primitive Square_Primitives[3]=
{
	{Type_Filled_Triangle , {(float)-19,(float)19} , {(float)-19,(float)-19} , {(float)19,(float)-19} , {50,50,50}},
	{Type_Filled_Triangle , {(float)-19,(float)19} , {(float)19,(float)-19} , {(float)19,(float)19} , {50,50,50}},
};

Collision_Data Square_Collisions[1];