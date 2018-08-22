#include "Support_Functions.h"

CLASSIFY Classify_PointToLineSegment(Vertex point, Vertex LineSeg_V0, Vertex LineSeg_V1)
{
	float res1 = LineSeg_V1.y - LineSeg_V0.y;
	float res2 = LineSeg_V1.x - LineSeg_V0.x;
	float res3 = point.x - LineSeg_V0.x;
	float res4 = point.y - LineSeg_V0.y;
	float t;
	if(res3*res1 == res4*res2)//the point is on the line of the segment
	{		
		if((res1<= -EPSILON) || (res1 >=EPSILON))
			t = res4/res1;
		else //if(res2 != 0)
			t = res3/res2;
		
		/*if(t < 0.0f)
			return BEHIND;
		else if(t > 1.0f)
			return BEYOND;*/
		if((t > 0.0f) && (t < 1.0f))
			return INTERSECT;
		else if(t == 0.0f)
			return INTERSECT_A;
		else if(t == 1.0f)
			return INTERSECT_B;
	}
	else //aside
	{
		t = res2*res4 - res3*res1;
		if(t > 0.0f)
			return LEFT;
		else if(t < 0.0f)
			return RIGHT;		
		//t == 0.0f is the first case
	}
	//never reached
	return INTERSECT;
}

bool PointOnLineSegment(Vertex point, Vertex LineSeg_V0, Vertex LineSeg_V1)
{
	float res1 = LineSeg_V1.y - LineSeg_V0.y;
	float res2 = LineSeg_V1.x - LineSeg_V0.x;
	float res3 = point.x - LineSeg_V0.x;
	float res4 = point.y - LineSeg_V0.y;
	float t;
	if(res3*res1 == res4*res2)//the point is on the line of the segment
	{		
		if((res1<= -EPSILON) || (res1 >=EPSILON))
			t = res4/res1;
		else //if(res2 != 0)
			t = res3/res2;
		
		if((t >= 0.0f) && (t <= 1.0f))
			return true;
	}
	return false;
}

void Get_SegmentToSegment_Point (Vertex L1_a, Vertex L1_b, Vertex L2_a, Vertex L2_b, Vertex &PtInter)
{
	Vector V1, V2;
	V1.x = L1_b.x - L1_a.x;	
	V1.y = L1_b.y - L1_a.y;

	V2.x = L2_b.x - L2_a.x;	
	V2.y = L2_b.y - L2_a.y;

	float t1,t2;
	float deno = -V2.x*V1.y + V2.y*V1.x;
	if((deno > EPSILON) || (deno < -EPSILON))
	{
		t1 = -(L1_a.x*V2.y - V2.x*L1_a.y + V2.x*L2_a.y - L2_a.x*V2.y)/deno;
		t2 = -(L1_a.x*V1.y  - V1.y*L2_a.x - L1_a.y*V1.x + L2_a.y*V1.x)/deno;

		if((t1 >= 0.0f) && (t1 <= 1.0f) && (t2 >= 0.0f) && (t2 <= 1.0f))
		{
			PtInter.x = L1_a.x + V1.x*t1;
			PtInter.y = L1_a.y + V1.y*t1;
		}
	}
}

/* Function to compute the dot product of 2 vectors */
void Vector_DotProduct (float &DP, VECTOR V0, VECTOR V1)
{
	DP = V0.x*V1.x + V0.y*V1.y;
}
/* Function to compute the cross product of 2 vectors */
void Vector_CrossProduct (float &z, VECTOR &V0, VECTOR &V1)
{
	z =   (V0.x*V1.y - V1.x*V0.y);
}
/* Function to compute the Magnitude of a vector */
void Vector_Magnitude (float &Mag, VECTOR &V)
{
	Mag = sqrtf( V.x*V.x + V.y*V.y );
}

/* Function to compute the unit Vector of a given vector */
void Vector_Unit (VECTOR &Unit, VECTOR &V)
{
	float Mag;

	Mag = sqrtf( V.x*V.x + V.y*V.y );
	
	Unit.x = V.x/Mag;
	Unit.y = V.y/Mag;
}

/* Function to compute the distance between 2 points */
void Vector_Distance (float &distance, VERTEX &P0, VERTEX &P1)
{
	distance = sqrtf((P1.x-P0.x)*(P1.x-P0.x) + (P1.y-P0.y)*(P1.y-P0.y));
}