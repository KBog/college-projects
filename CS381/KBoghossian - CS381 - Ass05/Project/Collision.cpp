#include "Collision.h"

bool Line_Circle_Collision (Collision_Data Circle, Collision_Data Line) 				
{ 				
	Vector s_m, VD; 				
	float A, B, C, D; 				
	float t, t1, t2; 				
	/* Getting the vector s(Vector from a point on the line to O(0,0)) - m(Vector from the Center of the circle to O(0,0))*/ 				
	s_m.x=Line.V0.x-Circle.V0.x; 				
	s_m.y=Line.V0.y-Circle.V0.y; 				
	/* Defining the vector direction of the line */ 				
	VD.x=Line.V1.x-Line.V0.x; 				
	VD.y=Line.V1.y-Line.V0.y; 				
	/* The equation is of the form At^2 + Bt + C = 0 */ 				
	A = VD.x*VD.x + VD.y*VD.y; 				
	B = (2*s_m.x*VD.x + 2*s_m.y*VD.y); 				
	C = (s_m.x*s_m.x + s_m.y*s_m.y) - Circle.V1.x*Circle.V1.x; 				
	/* Descriminant */ 				
	D = B*B - 4*A*C; 				
	float sqrt_D=(float)sqrt(D); 				
	/* Checking if the descriminant is >=0 */ 				
	if(D>=0) 				
	{ 				
		/* One point of intersection */ 				
		if(D==0) 				
			t = (-B)/(2*A); 				
		/* Two points of intersection */ 				
		else 				
		{ 				
			/* Taking the parameter t of the 2 points of intersection */ 				
			t1 = (-B + sqrt_D)/(2*A); 				
			t2 = (-B - sqrt_D)/(2*A); 				
			/* Taking the smaller value of t */ 				
			t = (t1<t2)?t1:t2; 				
		} 				
		/* Taking the line as a segment */ 				
		if(t>=0 && t<=1) 				
			/* Collision */ 				
			return true; 				
	} 				
	/* No collision */ 				
	return false; 				
}

bool Circle_Circle_Collision (Collision_Data Circle_1, Collision_Data Circle_2) 				
{ 				
	/* Calculate the distance from the center of circle_1 to the center of circle_2, if its 				
	less than the radius of circle_1 + the radius of circle_2 --> there is collision */ 				
	if(sqrt(((Circle_1.V0.x - Circle_2.V0.x)*(Circle_1.V0.x - Circle_2.V0.x)) + ((Circle_1.V0.y - Circle_2.V0.y)*(Circle_1.V0.y - Circle_2.V0.y)))<= (Circle_1.V1.x + Circle_2.V1.x)) 				
		return true; 				
	/* No collision */ 				
	return false; 				
}

bool Line_Line_Collision (Collision_Data Line_1, Collision_Data Line_2) 				
{ 				
	Vector VD_2, s_1, s_2; 				
	/* Defining the vector direction of the second line */ 				
	VD_2.x=Line_2.V1.x-Line_2.V0.x; 				
	VD_2.y=Line_2.V1.y-Line_2.V0.y; 				
	/* Defining the vector which is from any point on the line to the origin */ 				
	/* Line 1 */ 				
	s_1.x=Line_1.V0.x; 				
	s_1.y=Line_1.V0.y; 				
	/* Line 2 */ 				
	s_2.x=Line_2.V0.x; 				
	s_2.y=Line_2.V0.y; 				
	/* Calculating the dot product of the vector normal 				
	N and the vector director C */ 				
	float N_C = Line_1.Normal.x*VD_2.x + Line_1.Normal.y*VD_2.y; 				
	/* if the 2 lines are perpendi */ 				
	if(N_C==0) 				
		return false; 				
	/* If the dot product result is different from 0 				
	--> calculate the time t */ 				
	float t = ((Line_1.Normal.x*s_1.x + Line_1.Normal.y*s_1.y) - (Line_1.Normal.x*s_2.x + Line_1.Normal.y*s_2.y)) / N_C; 				
	/* Take the segments */ 				
	if(t>=0 && t<=1) 				
		return true; 				
	return false; 				
}
