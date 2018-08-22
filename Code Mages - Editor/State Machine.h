/*Include Windows library*/
#include<windows.h>

/*Algorithm Enumeration*/
enum Algorithm
{
	None,
	Algorithm_Line,
	Algorithm_Circle,
	Algorithm_Filled_Circle,
	Algorithm_Triangle,
	Algorithm_Filled_Triangle,
	Algorithm_Collision_Line,
	Algorithm_Collision_Circle,
};

/*Definitions*/
#define MAXCLICKS 3

/*Function Prototypes*/
bool PointsNumber(HWND hwnd,POINT pt,Color_RGB Color);
void Draw(HWND hwnd,POINT pt,Color_RGB Color);