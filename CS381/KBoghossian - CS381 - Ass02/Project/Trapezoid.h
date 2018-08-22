#ifndef _TRAPEZOID_H_
#define _TRAPEZOID_H_

#include "Standard Header.h"
#include "Support_Functions.h"

/* Trapezoid class */
class Trapezoid
{
	public:
			/* The free top and bottom segments */
			Collision_Data Top_Segment, Bottom_Segment;
			/* The lower x-value of the trapezoid (x component of the vertical line) */
			Vertex leftPoint, rightPoint;
			/* Four trapezoids neighbors where:
				[0] = upper left neighbor
				[1] = lower left neighbor
				[2] = upper right neighbor
				[3] = lower right neighbor
			*/
			Trapezoid *neighbors [4];
			/* Four neighbors' status flags (used while building the graph) */
			bool neighborUsed [4];
			/* State index (save the state index in the states array of the graph */
			int stateIndex;

			Character *model;
			Vertex Center;

			/* Trapezoid Constructor */
			Trapezoid ()
			{
				for(int i=0; i<4; i++)
				{
					neighborUsed[i]=false;				
					neighbors[i]=NULL;
				}
				model=NULL;
				stateIndex=-1;
				leftPoint.x=leftPoint.y=rightPoint.x=rightPoint.y=0;
				Top_Segment.V0.x=Top_Segment.V0.y=Bottom_Segment.V0.x=Bottom_Segment.V0.y=0;
				Top_Segment.V1.x=Top_Segment.V1.y=Bottom_Segment.V1.x=Bottom_Segment.V1.y=0;
				Center.x=Center.y=0;
			}

			void CreateTrapezoidModel (unsigned char r, unsigned char g, unsigned char b)
			{
				if(model)
				{
					delete [] model->GetPrimitives();
					delete model;
					model=NULL;
				}
				Primitive *P = new Primitive [4];
				Collision_Data Trap_Collisions[1];

				int i=0;
					P[i].Type = P[i+1].Type = P[i+2].Type = P[i+3].Type = Type_Line;
					P[i].Color.r = r;	P[i].Color.g = g;	P[i].Color.b = b;
					P[i+1].Color.r = r;	P[i+1].Color.g = g; P[i+1].Color.b = b;
					P[i+2].Color.r = r;	P[i+2].Color.g = g; P[i+2].Color.b = b;
					P[i+3].Color.r = r;	P[i+3].Color.g = g; P[i+3].Color.b = b;
					
					P[i].V0 = Top_Segment.V0;
					P[i].V1 = Top_Segment.V1;

					P[i+1].V0 = Bottom_Segment.V0;
					P[i+1].V1 = Bottom_Segment.V1;

					P[i+2].V0.x = leftPoint.x;	P[i+2].V0.y = Top_Segment.V0.y;
					P[i+2].V1.x = leftPoint.x;	P[i+2].V1.y = Bottom_Segment.V0.y;

					P[i+3].V0.x = rightPoint.x;	P[i+3].V0.y = Top_Segment.V1.y;
					P[i+3].V1.x = rightPoint.x;	P[i+3].V1.y = Bottom_Segment.V1.y;
	
				model = new Character("Trapezoids",P,Trap_Collisions,4,0,0,0,1,0,0,0,0,0,200);
			}

			/* Function to get the trapezoid center */
			void Compute_Center (void)
			{
				Vertex LeftMid, RightMid;
				/* Compute the middle of the left segment */
				LeftMid = model->GetPrimitives()[2].GetCenter();
				/* Compute the middle of the right segment */
				RightMid = model->GetPrimitives()[3].GetCenter();

				/* Compute the center */
				Center.x = LeftMid.x + (RightMid.x-LeftMid.x)*0.5f;
				Center.y = LeftMid.y + (RightMid.y-LeftMid.y)*0.5f;
			}

			/* Function to classify a point to a trapezoid */
			CLASSIFY Classify_PtToTrapezoid (Vertex pt)
			{
				CLASSIFY Test;
				/* if the pt is to the right of seg0 --> check the others */
				if (!(model->GetPrimitives()[0].V0==model->GetPrimitives()[0].V1))
				{
					Test=Classify_PointToLineSegment(pt,model->GetPrimitives()[0].V0,model->GetPrimitives()[0].V1);
					if(Test!=RIGHT && Test!=INTERSECT && Test!=INTERSECT_A && Test!=INTERSECT_B)
						return OUTSIDE;
				}
				/* if the pt is to the right of seg3 --> check the others */
				if (!(model->GetPrimitives()[3].V0==model->GetPrimitives()[3].V1))
				{
					Test=Classify_PointToLineSegment(pt,model->GetPrimitives()[3].V0,model->GetPrimitives()[3].V1);
					if(Test!=RIGHT && Test!=INTERSECT && Test!=INTERSECT_A && Test!=INTERSECT_B)
						return OUTSIDE;
				}
				/* if the pt is to the left of seg2 --> check the others */
				if (!(model->GetPrimitives()[2].V0==model->GetPrimitives()[2].V1))
				{
					Test=Classify_PointToLineSegment(pt,model->GetPrimitives()[2].V0,model->GetPrimitives()[2].V1);
					if(Test!=LEFT && Test!=INTERSECT && Test!=INTERSECT_A && Test!=INTERSECT_B)
						return OUTSIDE;
				}
				/* if the pt is to the left of seg2 --> check the others */
				if (!(model->GetPrimitives()[1].V0==model->GetPrimitives()[1].V1))
				{
					Test=Classify_PointToLineSegment(pt,model->GetPrimitives()[1].V0,model->GetPrimitives()[1].V1);
					if(Test!=LEFT && Test!=INTERSECT && Test!=INTERSECT_A && Test!=INTERSECT_B)
						return OUTSIDE;
				}

				/* the point lies in the trapezoid */
				return INSIDE;
			}
};

#endif