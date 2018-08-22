#include "Graph.h"
#include "Standard Header.h"
#include <list>
#include "SearchTree.h"
#include "Trapezoid.h"

#define REALTIME_BUILDING 1

#ifndef _TRAPEZOIDALALGORITHM_H_
#define _TRAPEZOIDALALGORITHM_H_

/* Trapezoidal algorithm class */
class Trapezoidal_Algorithm
{
	public:
			/* To Store the number of segments to loop through */
			int numberOfSegments;
			/* A pointer to the array of segments (Solid Polygons) */
			Collision_Data *Segments;
			/* The list of trapezoids */
			list<Trapezoid *> trapezoidalList;
			/* The list of trapezoids lying in the solid area */
			list<Trapezoid *> solidtrapezoidalList;
			/* The number of intersected trapezoids with the linesegment */
			int tcount;
			/* The array of intersected trapezoids with the current linesegment */
			vector<Trapezoid *> trapezoidIntersections;
			/* An array to store the used points so far */
			vector<Vertex> UsedPoints;
			/* The search tree */
			SearchTree *searchTree;
			/* A Pointer to the graph to be built later */
			Graph *graph;
			

			/* Constructor */
			Trapezoidal_Algorithm (int numberofsegments, Collision_Data *segments) :
			numberOfSegments(numberofsegments), Segments(segments)
			{
				graph = new Graph;
				/* Adjust the Segments */
				Swap_Segments_StartEndPoints();
				/* Create the trapezoidal map */
				Create_TrapezoidalMap();
				/* Adjust the trapezoids segments */
				Adjust_Trapezoids_Segments();
				/* Remove the trapezoids lying in solid areas */
				Remove_UnusedTrapezoids();
				/* Create a model for the map to render */
				Create_MapAsCharacter();
				/* Generate the graph */
				PathBuilding();
			}
			
			void Swap_Segments_StartEndPoints (void);
			void Adjust_Trapezoids_Segments (void);
			/* Function to remove the trapezoids lying in solid areas */
			void Remove_UnusedTrapezoids (void);
			/* To compute the first trapezoid that includes all the segments */
			Trapezoid *GetBoundingRectangle (void);
			/* Function to get the intersected trapezoids with the current segment */
			void Get_IntersectedTrapezoids (Vertex LS_V0, Vertex LS_V1);
			/* Function to know whether the start or end points of a line segment has been used */
			void Check_Segment_StartEnd_Used (bool &startPointUsed, bool &endPointUsed, Vertex LS_V0, Vertex LS_V1);			
			void Create_MapAsCharacter (void);
			/* The BIG FUNCTION! that will create the trapezoids according to the line segments */
			void Create_TrapezoidalMap (void);
			void Draw (void);

			/* Function to get the least distant trapezoid in the x-axis */
			Trapezoid *GetLeastDistantTrapezoid (void);
			/* recursive function to generate the graph */
			void StatesBuild (Trapezoid *curTrap, int prevStateNbr);
			/* Function to build the graph and the shortest path */
			void PathBuilding (void);
			/* Function to get the state of the trapezoid containing the vertex */
			int Get_TrapezoidState (Vertex pt);
};

#endif