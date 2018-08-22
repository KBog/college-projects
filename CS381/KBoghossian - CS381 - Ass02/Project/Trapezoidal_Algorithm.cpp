#include "Trapezoidal_Algorithm.h"
#include "Support_Functions.h"

extern ViewPort *ViewPort_List;

/* Function to adjust the start & end points of each segment so the algorithm works while check for the remaining trapezoids */
void Trapezoidal_Algorithm::Swap_Segments_StartEndPoints (void)
{
	for(int s=0; s<numberOfSegments; s++)
	{
		if(Segments[s].V1.x < Segments[s].V0.x)
			swap(Segments[s].V1,Segments[s].V0);
	}
}

void Trapezoidal_Algorithm::Adjust_Trapezoids_Segments (void)
{
	Vertex V0, V1;
	
	for(list<Trapezoid *>::iterator it = trapezoidalList.begin(); it!=trapezoidalList.end(); it++)
	{
		/* Get left vertical segment */
		V0.x = (*it)->leftPoint.x;	V0.y = (*it)->Top_Segment.V0.y;
		V1.x = (*it)->leftPoint.x;	V1.y = (*it)->Bottom_Segment.V0.y;
		/* Compute InterPoint (V0) Top @ Left vertical segment */
		Get_SegmentToSegment_Point((*it)->Top_Segment.V0,(*it)->Top_Segment.V1,V0,V1,(*it)->Top_Segment.V0);
		/* Compute InterPoint (V0) Bottom @ Left vertical segment */
		Get_SegmentToSegment_Point((*it)->Bottom_Segment.V0,(*it)->Bottom_Segment.V1,V0,V1,(*it)->Bottom_Segment.V0);

		/* Get right vertical segment */
		V0.x = (*it)->rightPoint.x;	V0.y = (*it)->Top_Segment.V1.y;
		V1.x = (*it)->rightPoint.x;	V1.y = (*it)->Bottom_Segment.V1.y;
		/* Compute InterPoint (V1) Top @ Right vertical segment */
		Get_SegmentToSegment_Point((*it)->Top_Segment.V0,(*it)->Top_Segment.V1,V0,V1,(*it)->Top_Segment.V1);
		/* Compute InterPoint (V1) Bottom @ Right vertical segment */
		Get_SegmentToSegment_Point((*it)->Bottom_Segment.V0,(*it)->Bottom_Segment.V1,V0,V1,(*it)->Bottom_Segment.V1);
		
		#ifdef REALTIME_BUILDING
			Create_MapAsCharacter();
			/* Drawing the viewports */
			Init_View(0);
			Draw();
			/*Copy buffer to Video RAM*/
			Graphics_Draw(NULL);
			Graphics_Flip();
			Sleep(300);
		#endif
	}
}

/* Function to remove the trapezoids lying in solid areas */
void Trapezoidal_Algorithm::Remove_UnusedTrapezoids (void)
{
	list<Trapezoid *>::iterator Next;
	for(list<Trapezoid *>::iterator it = trapezoidalList.begin(); it!=trapezoidalList.end(); it=Next)
	{
		it++;
		Next=it;
		it--;
		if((*it)->Top_Segment.Normal.y>0 && (*it)->Bottom_Segment.Normal.y<0)
		{
			solidtrapezoidalList.push_back(*it);
			trapezoidalList.remove(*it);

			#ifdef REALTIME_BUILDING
				Create_MapAsCharacter();
				/* Drawing the viewports */
				Init_View(0);
				Draw();
				/*Copy buffer to Video RAM*/
				Graphics_Draw(NULL);
				Graphics_Flip();
				Sleep(100);
			#endif
		}
	}
}

/* To compute the first trapezoid that includes all the segments */
Trapezoid *Trapezoidal_Algorithm::GetBoundingRectangle (void)
{
	Trapezoid *Bounding = new Trapezoid;
	/* Init dimensions */
	float Min_X=5000, Max_X=-5000;
	float Min_Y=5000, Max_Y=-5000;
	/* The added value to the borders */
	float Border = 20;

	/* Loop trough the number of segments */
	for(int s=0; s<numberOfSegments; s++)
	{
		/* Compare Seg's V0.X with the Min & Max */
		if(Segments[s].V0.x<Min_X)
			Min_X = Segments[s].V0.x;
		if(Segments[s].V0.x>Max_X)
			Max_X = Segments[s].V0.x;
		/* Compare Seg's V0.Y with the Min & Max */
		if(Segments[s].V0.y<Min_Y)
			Min_Y = Segments[s].V0.y;
		if(Segments[s].V0.y>Max_Y)
			Max_Y = Segments[s].V0.y;

		/* Compare Seg's V1.X with the Min & Max */
		if(Segments[s].V1.x<Min_X)
			Min_X = Segments[s].V1.x;
		if(Segments[s].V1.x>Max_X)
			Max_X = Segments[s].V1.x;
		/* Compare Seg's V1.Y with the Min & Max */
		if(Segments[s].V1.y<Min_Y)
			Min_Y = Segments[s].V1.y;
		if(Segments[s].V1.y>Max_Y)
			Max_Y = Segments[s].V1.y;
	}

	/* The Bounding Trapezoid Properties */
	/* Set the left and right points */
	Bounding->leftPoint.x = Min_X - Border;					Bounding->leftPoint.y = Min_Y - Border;
	Bounding->rightPoint.x = Max_X + Border;					Bounding->rightPoint.y = Max_Y + Border;
	/* Set the top and bottom segments (clock wise) */
	/* Top = V0-->V1 */
	Bounding->Top_Segment.V0.x = Bounding->leftPoint.x;		Bounding->Top_Segment.V0.y = Bounding->rightPoint.y;
	Bounding->Top_Segment.V1.x = Bounding->rightPoint.x;		Bounding->Top_Segment.V1.y = Bounding->rightPoint.y;
	Bounding->Top_Segment.Normal.x = 0;
	Bounding->Top_Segment.Normal.y = -1;
	Normalize_Vector(Bounding->Top_Segment.Normal);
	Bounding->Top_Segment.Type = Type_Collision_Line;
	/* Bottom = V1<--V0 */
	Bounding->Bottom_Segment.V0.x = Bounding->rightPoint.x;	Bounding->Bottom_Segment.V0.y = Bounding->leftPoint.y;
	Bounding->Bottom_Segment.V1.x = Bounding->leftPoint.x;	Bounding->Bottom_Segment.V1.y = Bounding->leftPoint.y;
	Bounding->Bottom_Segment.Normal.x = 0;
	Bounding->Bottom_Segment.Normal.y = 1;
	Normalize_Vector(Bounding->Bottom_Segment.Normal);
	Bounding->Bottom_Segment.Type = Type_Collision_Line;

	/* Swap the bottom start & end values (for the V0<V1 in x-value) */
	swap(Bounding->Bottom_Segment.V0,Bounding->Bottom_Segment.V1);

	return Bounding;
}

/* Function to get the intersected trapezoids with the current segment */
void Trapezoidal_Algorithm::Get_IntersectedTrapezoids (Vertex LS_V0, Vertex LS_V1)
{
	/* Reset the array of intersected trapezoids */
	trapezoidIntersections.clear();
	/* Get the first (left) intersected trapezoid with the line segment */
	trapezoidIntersections.push_back(searchTree->TrapezoidSearch(LS_V0,LS_V1));
	/* Reset tcount var to 1 */
	tcount=1;
	
	/* Get the first intersected trapezoid */
	Trapezoid *currentTrapezoid = trapezoidIntersections[tcount-1];
	/* while the line segment end point is greater than the current trapezoid's right point 
	--> there is still trapezoid intersecting the line segment (from LeftTrap --> RightTrap) */
	while(LS_V1.x > currentTrapezoid->rightPoint.x)
	{
		/* if the current intersected trapezoid's right point is ABOVE the current line seg */
		if(Classify_PointToLineSegment(currentTrapezoid->rightPoint,LS_V0,LS_V1)==LEFT)
			trapezoidIntersections.push_back(trapezoidIntersections[tcount-1]->neighbors[3]);
		else
			trapezoidIntersections.push_back(trapezoidIntersections[tcount-1]->neighbors[2]);
		/* Take the next trapezoid */
		currentTrapezoid = trapezoidIntersections[tcount];
		tcount++;
	}
}

/* Function to know whether the start or end points of a line segment has been used */
void Trapezoidal_Algorithm::Check_Segment_StartEnd_Used (bool &startPointUsed, bool &endPointUsed, Vertex LS_V0, Vertex LS_V1)
{
	/* Init the vars */
	startPointUsed=endPointUsed=false;

	/* Loop through the list of trapezoids */
	for(unsigned int p=0; p<UsedPoints.size(); p++)
	{
		/* if the any current booleans is still false --> continue to check */
		if(!startPointUsed || !endPointUsed)
		{
			/* if the start bool is still false --> check the starting point with all the points in the current trapez */
			if(!startPointUsed && Equal_Points(LS_V0,UsedPoints[p]))
								startPointUsed=true;
			/* if the end bool is still false --> check the ending point with all the points in the current trapez */
			if(!endPointUsed && Equal_Points(LS_V1,UsedPoints[p]))
								endPointUsed=true;
		}
		/* the 2 points are used --> don't search anymore */
		else
			return;
	}
	/* If the start point is not used --> Add it to the array of used points */
	if(!startPointUsed)
		UsedPoints.push_back(LS_V0);
	/* If the end point is not used --> Add it to the array of used points */
	if(!endPointUsed)
		UsedPoints.push_back(LS_V1);
}

/* Function to create the trapezoids models */
void Trapezoidal_Algorithm::Create_MapAsCharacter (void)
{
	/* Create the trapezoids in the list */
	for(list<Trapezoid *>::iterator it = trapezoidalList.begin(); it!=trapezoidalList.end(); it++)
		(*it)->CreateTrapezoidModel(0,0,200);
	/* create the solid trapezoids */
	for(list<Trapezoid *>::iterator it2 = solidtrapezoidalList.begin(); it2!=solidtrapezoidalList.end(); it2++)
		(*it2)->CreateTrapezoidModel(200,200,200);
}

void Trapezoidal_Algorithm::Draw (void)
{
	Matrix<3> Object_Matrix, Mapping_Matrix, Final;
	/* Building the viewing matrix */
	Viewing_Transformation(ViewPort_List, Mapping_Matrix);

	for(list<Trapezoid *>::iterator it = trapezoidalList.begin(); it!=trapezoidalList.end(); it++)
	{
		/* Transforming the objects from their model coodinates to the world coord */
		(*it)->model->BuildCharacterMatrix(Object_Matrix.M);
		/* Concatinating the final matrix */
		Final=Mapping_Matrix*Object_Matrix;
		/* Drawing the object */
		(*it)->model->Draw(Final.M);
	}

	for(list<Trapezoid *>::iterator it = solidtrapezoidalList.begin(); it!=solidtrapezoidalList.end(); it++)
	{
		/* Transforming the objects from their model coodinates to the world coord */
		(*it)->model->BuildCharacterMatrix(Object_Matrix.M);
		/* Concatinating the final matrix */
		Final=Mapping_Matrix*Object_Matrix;
		/* Drawing the object */
		(*it)->model->Draw(Final.M);
	}

	graph->Draw(ViewPort_List);
}

/* The BIG FUNCTION! that will create the trapezoids according to the line segments */
void Trapezoidal_Algorithm::Create_TrapezoidalMap (void)
{
	/* Get the first big bounding trapezoid */
	trapezoidalList.push_back(GetBoundingRectangle());
	/* Init the root of the search tree with the bounding quad */
	searchTree = new SearchTree((*trapezoidalList.begin()));

	/* Two booleans to know if the start of end segment has been used */
	bool startPointUsed, endPointUsed;

	/* Loop through the number of segments */
	for(int i=0; i<numberOfSegments; i++)
	{
		#ifdef REALTIME_BUILDING
			Create_MapAsCharacter();
			/* Drawing the viewports */
			Init_View(0);
			Draw();
			/*Copy buffer to Video RAM*/
			Graphics_Draw(NULL);
			Graphics_Flip();
			Sleep(300);
		#endif
		/* Get the intersected trapezoids of the current scene with the current segment */
		Get_IntersectedTrapezoids(Segments[i].V0,Segments[i].V1);
		/* check whether the start or end points of a line segment has been used */
		Check_Segment_StartEnd_Used(startPointUsed,endPointUsed,Segments[i].V0,Segments[i].V1);

		/* First general case (1 Trapez) */
		if(tcount==1)
		{
			/* --------------- 4 Sub-Cases --------------- */
			/* case 1/4 */
			if(startPointUsed && endPointUsed)				/* 2 new Trapezoids */
			{
				Trapezoid *trapA, *trapB;
				trapA = new Trapezoid;
				trapB = new Trapezoid;
			
				trapA->leftPoint = Segments[i].V0;
				trapA->rightPoint = Segments[i].V1;
				trapA->Top_Segment = trapezoidIntersections[0]->Top_Segment;
				trapA->Bottom_Segment = Segments[i];

				trapB->leftPoint = Segments[i].V0;
				trapB->rightPoint = Segments[i].V1;
				trapB->Top_Segment = Segments[i];
				trapB->Bottom_Segment = trapezoidIntersections[0]->Bottom_Segment;

				/* Add the 2 trapezoids to the list of trapezoids */
				trapezoidalList.push_back(trapA);
				trapezoidalList.push_back(trapB);

				/* 4 bools to manipulate the 7 Sub-Sub-Cases */
				bool startOnTop		= PointOnLineSegment(Segments[i].V0, trapezoidIntersections[0]->Top_Segment.V0, trapezoidIntersections[0]->Top_Segment.V1);
				bool startOnBottom	= PointOnLineSegment(Segments[i].V0, trapezoidIntersections[0]->Bottom_Segment.V0, trapezoidIntersections[0]->Bottom_Segment.V1);
				bool endOnTop		= PointOnLineSegment(Segments[i].V1, trapezoidIntersections[0]->Top_Segment.V0, trapezoidIntersections[0]->Top_Segment.V1);
				bool endOnBottom	= PointOnLineSegment(Segments[i].V1, trapezoidIntersections[0]->Bottom_Segment.V0, trapezoidIntersections[0]->Bottom_Segment.V1);

				/* case 1/7 */
				if(startOnBottom && endOnTop)
				{
					trapA->neighbors[0] = trapezoidIntersections[0]->neighbors[0];
					if(trapA->neighbors[0])
						trapA->neighbors[0]->neighbors[2] = trapA;
					trapB->neighbors[3] = trapezoidIntersections[0]->neighbors[3];
					if(trapB->neighbors[3])
						trapB->neighbors[3]->neighbors[1] = trapB;
				}
				/* case 2/7 */
				else if(startOnTop && endOnBottom)
				{
					trapA->neighbors[2] = trapezoidIntersections[0]->neighbors[2];
					if(trapA->neighbors[2])
						trapA->neighbors[2]->neighbors[0] = trapA;
					trapB->neighbors[1] = trapezoidIntersections[0]->neighbors[1];
					if(trapB->neighbors[1])
						trapB->neighbors[1]->neighbors[3] = trapB;
				}
				/* case 3/7 */
				else if(startOnTop)
				{
					trapA->neighbors[2] = trapezoidIntersections[0]->neighbors[2];
					if(trapA->neighbors[2])
						trapA->neighbors[2]->neighbors[0] = trapA;
					trapB->neighbors[1] = trapezoidIntersections[0]->neighbors[1];
					trapB->neighbors[3] = trapezoidIntersections[0]->neighbors[3];
					if(trapB->neighbors[1])
						trapB->neighbors[1]->neighbors[3] = trapB;
					if(trapB->neighbors[3])
						trapB->neighbors[3]->neighbors[1] = trapB;
				}
				/* case 4/7 */
				else if(startOnBottom)
				{
					trapA->neighbors[0] = trapezoidIntersections[0]->neighbors[0];
					trapA->neighbors[2] = trapezoidIntersections[0]->neighbors[2];
					if(trapA->neighbors[0])
						trapA->neighbors[0]->neighbors[2] = trapA;
					if(trapA->neighbors[2])
						trapA->neighbors[2]->neighbors[0] = trapA;
					trapB->neighbors[3] = trapezoidIntersections[0]->neighbors[3];
					if(trapB->neighbors[3])
						trapB->neighbors[3]->neighbors[1] = trapB;
				}
				/* case 5/7 */
				else if(endOnTop)
				{
					trapA->neighbors[0] = trapezoidIntersections[0]->neighbors[0];
					if(trapA->neighbors[0])
						trapA->neighbors[0]->neighbors[2] = trapA;
					trapB->neighbors[1] = trapezoidIntersections[0]->neighbors[1];
					trapB->neighbors[3] = trapezoidIntersections[0]->neighbors[3];
					if(trapB->neighbors[1])
						trapB->neighbors[1]->neighbors[3] = trapB;
					if(trapB->neighbors[3])
						trapB->neighbors[3]->neighbors[1] = trapB;
				}
				/* case 6/7 */
				else if(endOnBottom)
				{
					trapA->neighbors[0] = trapezoidIntersections[0]->neighbors[0];
					trapA->neighbors[2] = trapezoidIntersections[0]->neighbors[2];
					if(trapA->neighbors[0])
						trapA->neighbors[0]->neighbors[2] = trapA;
					if(trapA->neighbors[2])
						trapA->neighbors[2]->neighbors[0] = trapA;
					trapB->neighbors[1] = trapezoidIntersections[0]->neighbors[1];
					if(trapB->neighbors[1])
						trapB->neighbors[1]->neighbors[3] = trapB;
				}
				/* case 7/7 */
				else
				{
					trapA->neighbors[0] = trapezoidIntersections[0]->neighbors[0];
					trapA->neighbors[2] = trapezoidIntersections[0]->neighbors[2];
					if(trapA->neighbors[0])
						trapA->neighbors[0]->neighbors[2] = trapA;
					if(trapA->neighbors[2])
						trapA->neighbors[2]->neighbors[0] = trapA;
					trapB->neighbors[1] = trapezoidIntersections[0]->neighbors[1];
					trapB->neighbors[3] = trapezoidIntersections[0]->neighbors[3];
					if(trapB->neighbors[1])
						trapB->neighbors[1]->neighbors[3] = trapB;
					if(trapB->neighbors[3])
						trapB->neighbors[3]->neighbors[1] = trapB;
				}
				/* Update the search tree */
				TreeNode *Inter_Leaf = searchTree->Modify_SearchTree(trapezoidIntersections[0]);
				/* Replace by a Y-Node */
				Inter_Leaf->Type = Y_Node;
				Inter_Leaf->LineSegment_V0 = Segments[i].V0;
				Inter_Leaf->LineSegment_V1 = Segments[i].V1;
				/* Left child */
				Inter_Leaf->Left_Child = new TreeNode(trapA,Inter_Leaf);
				/* Right child */
				Inter_Leaf->Right_Child = new TreeNode(trapB,Inter_Leaf);
			}
			/* case 2/4 */
			else if(startPointUsed)							/* 3 new Trapezoids */
			{
				Trapezoid *trapA, *trapB, *trapC;
				trapA = new Trapezoid;
				trapB = new Trapezoid;
				trapC = new Trapezoid;

				trapA->leftPoint = Segments[i].V1;
				trapA->rightPoint = trapezoidIntersections[0]->rightPoint;
				trapA->Top_Segment = trapezoidIntersections[0]->Top_Segment;
				trapA->Bottom_Segment = trapezoidIntersections[0]->Bottom_Segment;

				trapB->leftPoint = Segments[i].V0;
				trapB->rightPoint = Segments[i].V1;
				trapB->Top_Segment = trapezoidIntersections[0]->Top_Segment;
				trapB->Bottom_Segment = Segments[i];

				trapC->leftPoint = Segments[i].V0;
				trapC->rightPoint = Segments[i].V1;
				trapC->Top_Segment = Segments[i];
				trapC->Bottom_Segment = trapezoidIntersections[0]->Bottom_Segment;

				/* Add the 3 trapezoids to the list */
				trapezoidalList.push_back(trapA);
				trapezoidalList.push_back(trapB);
				trapezoidalList.push_back(trapC);

				/* 3 Sub-Sub-Cases */
				/* case 1/3 */
				if(PointOnLineSegment(Segments[i].V0,trapezoidIntersections[0]->Top_Segment.V0,trapezoidIntersections[0]->Top_Segment.V1))
				{
					trapA->neighbors[0] = trapB;
					trapA->neighbors[1] = trapC;
					trapA->neighbors[2] = trapezoidIntersections[0]->neighbors[2];
					trapA->neighbors[3] = trapezoidIntersections[0]->neighbors[3];
					
					if(trapA->neighbors[2])
						trapA->neighbors[2]->neighbors[0] = trapA;
					if(trapA->neighbors[3])
						trapA->neighbors[3]->neighbors[1] = trapA;
					trapB->neighbors[2] = trapA;
					trapC->neighbors[1] = trapezoidIntersections[0]->neighbors[1];
					trapC->neighbors[3] = trapA;
					if(trapC->neighbors[1])
						trapC->neighbors[1]->neighbors[3] = trapC;
				}
				/* case 2/3 */
				else if(PointOnLineSegment(Segments[i].V0, trapezoidIntersections[0]->Bottom_Segment.V0, trapezoidIntersections[0]->Bottom_Segment.V1))
				{
					trapA->neighbors[0] = trapB;
					trapA->neighbors[1] = trapC;
					trapA->neighbors[2] = trapezoidIntersections[0]->neighbors[2];
					trapA->neighbors[3] = trapezoidIntersections[0]->neighbors[3];
					
					if(trapA->neighbors[2])
						trapA->neighbors[2]->neighbors[0] = trapA;
					if(trapA->neighbors[3])
						trapA->neighbors[3]->neighbors[1] = trapA;
					trapB->neighbors[0] = trapezoidIntersections[0]->neighbors[0];
					trapB->neighbors[2] = trapA;
					if(trapB->neighbors[0])
						trapB->neighbors[0]->neighbors[2] = trapB;
					trapC->neighbors[3] = trapA;
				}
				/* case 3/3 */
				else
				{
					trapA->neighbors[0] = trapB;
					trapA->neighbors[1] = trapC;
					trapA->neighbors[2] = trapezoidIntersections[0]->neighbors[2];
					trapA->neighbors[3] = trapezoidIntersections[0]->neighbors[3];
					
					if(trapA->neighbors[2])
						trapA->neighbors[2]->neighbors[0] = trapA;
					if(trapA->neighbors[3])
						trapA->neighbors[3]->neighbors[1] = trapA;
					trapB->neighbors[2] = trapA;
					trapB->neighbors[0] = trapezoidIntersections[0]->neighbors[0];
					if(trapB->neighbors[0])
						trapB->neighbors[0]->neighbors[2] = trapB;
					trapC->neighbors[3] = trapA;
					trapC->neighbors[1] = trapezoidIntersections[0]->neighbors[1];
					if(trapC->neighbors[1])
						trapC->neighbors[1]->neighbors[3] = trapC;
				}
				/* Update the search tree */
				TreeNode *Inter_Leaf = searchTree->Modify_SearchTree(trapezoidIntersections[0]);
				/* Replace by X-Node */
				Inter_Leaf->Type = X_Node;
				Inter_Leaf->point = Segments[i].V1;
				/* Right Child */
				Inter_Leaf->Right_Child = new TreeNode(trapA,Inter_Leaf);
				/* Left Child */
				Inter_Leaf->Left_Child = new TreeNode(Segments[i].V0,Segments[i].V1,Inter_Leaf);
				/* Left-Left Child */
				Inter_Leaf->Left_Child->Left_Child = new TreeNode(trapB,Inter_Leaf->Left_Child);
				/* Left-Right Child */
				Inter_Leaf->Left_Child->Right_Child = new TreeNode(trapC,Inter_Leaf->Left_Child);
			}
			/* case 3/4 */
			else if(endPointUsed)							/* 3 new Trapezoids */
			{
				Trapezoid *trapA, *trapB, *trapC;
				trapA = new Trapezoid;
				trapB = new Trapezoid;
				trapC = new Trapezoid;

				trapA->leftPoint = trapezoidIntersections[0]->leftPoint;
				trapA->rightPoint = Segments[i].V0;
				trapA->Top_Segment = trapezoidIntersections[0]->Top_Segment;
				trapA->Bottom_Segment = trapezoidIntersections[0]->Bottom_Segment;

				trapB->leftPoint = Segments[i].V0;
				trapB->rightPoint = Segments[i].V1;
				trapB->Top_Segment = trapezoidIntersections[0]->Top_Segment;
				trapB->Bottom_Segment = Segments[i];

				trapC->leftPoint = Segments[i].V0;
				trapC->rightPoint = Segments[i].V1;
				trapC->Top_Segment = Segments[i];
				trapC->Bottom_Segment = trapezoidIntersections[0]->Bottom_Segment;

				/* Add the 3 trapzeoids to the list */
				trapezoidalList.push_back(trapA);
				trapezoidalList.push_back(trapB);
				trapezoidalList.push_back(trapC);

				/* 3 Sub-Sub-Cases */
				/* case 1/3 */
				if(PointOnLineSegment(Segments[i].V1,trapezoidIntersections[0]->Top_Segment.V0,trapezoidIntersections[0]->Top_Segment.V1))
				{
					trapA->neighbors[1] = trapezoidIntersections[0]->neighbors[1];
					trapA->neighbors[2] = trapB;
					trapA->neighbors[3] = trapC;
					if(trapA->neighbors[1])
						trapA->neighbors[1]->neighbors[3] = trapA;
					trapB->neighbors[0] = trapA;
					trapC->neighbors[1] = trapA;
					trapC->neighbors[3] = trapezoidIntersections[0]->neighbors[3];
					if(trapC->neighbors[3])
						trapC->neighbors[3]->neighbors[1] = trapC;
				}
				/* case 2/3 */
				else if(PointOnLineSegment(Segments[i].V1,trapezoidIntersections[0]->Bottom_Segment.V0,trapezoidIntersections[0]->Bottom_Segment.V1))
				{
					trapA->neighbors[0] = trapezoidIntersections[0]->neighbors[0];
					trapA->neighbors[2] = trapB;
					trapA->neighbors[3] = trapC;
					if(trapA->neighbors[0])
						trapA->neighbors[0]->neighbors[2] = trapA;
					trapB->neighbors[0] = trapA;
					trapB->neighbors[2] = trapezoidIntersections[0]->neighbors[2];
					if(trapB->neighbors[2])
						trapB->neighbors[2]->neighbors[0] = trapB;
					trapC->neighbors[1] = trapA;
				}
				/* case 3/3 */
				else
				{
					trapA->neighbors[0] = trapezoidIntersections[0]->neighbors[0];
					trapA->neighbors[1] = trapezoidIntersections[0]->neighbors[1];
					trapA->neighbors[2] = trapB;
					trapA->neighbors[3] = trapC;
					if(trapA->neighbors[0])
						trapA->neighbors[0]->neighbors[2] = trapA;
					if(trapA->neighbors[1])
						trapA->neighbors[1]->neighbors[3] = trapA;
					trapB->neighbors[0] = trapA;
					trapB->neighbors[2] = trapezoidIntersections[0]->neighbors[2];
					if(trapB->neighbors[2])
						trapB->neighbors[2]->neighbors[0] = trapB;
					trapC->neighbors[1] = trapA;
					trapC->neighbors[3] = trapezoidIntersections[0]->neighbors[3];
					if(trapC->neighbors[3])
						trapC->neighbors[3]->neighbors[1] = trapC;
				}
				/* Update the Search tree */
				TreeNode *Inter_Leaf = searchTree->Modify_SearchTree(trapezoidIntersections[0]);
				/* Replace by X-Node */
				Inter_Leaf->Type = X_Node;
				Inter_Leaf->point = Segments[i].V0;
				/* Left Child */
				Inter_Leaf->Left_Child = new TreeNode(trapA,Inter_Leaf);
				/* Right Child */
				Inter_Leaf->Right_Child = new TreeNode(Segments[i].V0,Segments[i].V1,Inter_Leaf);
				/* Right-Left Child */
				Inter_Leaf->Right_Child->Left_Child = new TreeNode(trapB,Inter_Leaf->Right_Child);
				/* Right-Right Child */
				Inter_Leaf->Right_Child->Right_Child = new TreeNode(trapC,Inter_Leaf->Right_Child);
			}
			/* case 4/4 */
			else											/* 4 new Trapezoids */
			{
				Trapezoid *trapA, *trapB, *trapC, *trapD;
				trapA = new Trapezoid;
				trapB = new Trapezoid;
				trapC = new Trapezoid;
				trapD = new Trapezoid;

				trapA->leftPoint = trapezoidIntersections[0]->leftPoint;
				trapA->rightPoint = Segments[i].V0;
				trapA->Top_Segment = trapezoidIntersections[0]->Top_Segment;
				trapA->Bottom_Segment = trapezoidIntersections[0]->Bottom_Segment;

				trapB->leftPoint = Segments[i].V1;
				trapB->rightPoint = trapezoidIntersections[0]->rightPoint;
				trapB->Top_Segment = trapezoidIntersections[0]->Top_Segment;
				trapB->Bottom_Segment = trapezoidIntersections[0]->Bottom_Segment;

				trapC->leftPoint = Segments[i].V0;
				trapC->rightPoint = Segments[i].V1;
				trapC->Top_Segment = trapezoidIntersections[0]->Top_Segment;
				trapC->Bottom_Segment = Segments[i];

				trapD->leftPoint = Segments[i].V0;
				trapD->rightPoint = Segments[i].V1;
				trapD->Top_Segment = Segments[i];
				trapD->Bottom_Segment = trapezoidIntersections[0]->Bottom_Segment;

				/* Add 4 trapezoids to the list */
				trapezoidalList.push_back(trapA);
				trapezoidalList.push_back(trapB);
				trapezoidalList.push_back(trapC);
				trapezoidalList.push_back(trapD);

				/* 1 Sub-Sub-Case */
				trapA->neighbors[0] = trapezoidIntersections[0]->neighbors[0];
				trapA->neighbors[1] = trapezoidIntersections[0]->neighbors[1];
				trapA->neighbors[2] = trapC;
				trapA->neighbors[3] = trapD;
				if(trapA->neighbors[0])
					trapA->neighbors[0]->neighbors[2] = trapA;
				if(trapA->neighbors[1])
					trapA->neighbors[1]->neighbors[3] = trapA;
				trapB->neighbors[0] = trapC;
				trapB->neighbors[1] = trapD;
				trapB->neighbors[2] = trapezoidIntersections[0]->neighbors[2];
				trapB->neighbors[3] = trapezoidIntersections[0]->neighbors[3];
				if(trapB->neighbors[2])
					trapB->neighbors[2]->neighbors[0] = trapB;
				if(trapB->neighbors[3])
					trapB->neighbors[3]->neighbors[1] = trapB;
				trapC->neighbors[0] = trapA;
				trapC->neighbors[2] = trapB;
				trapD->neighbors[1] = trapA;
				trapD->neighbors[3] = trapB;

				/* Update the search tree */
				TreeNode *Inter_Leaf = searchTree->Modify_SearchTree(trapezoidIntersections[0]);
				
				/* Replace by X-Node */
				Inter_Leaf->Type = X_Node;
				Inter_Leaf->point = Segments[i].V0;
				/* Left Child */
				Inter_Leaf->Left_Child = new TreeNode(trapA,Inter_Leaf);
				/* Right Child */
				Inter_Leaf->Right_Child = new TreeNode(Segments[i].V1,Inter_Leaf);
				/* Right-Right Child */
				Inter_Leaf->Right_Child->Right_Child = new TreeNode(trapB,Inter_Leaf->Right_Child);
				/* Right-Left Child */
				Inter_Leaf->Right_Child->Left_Child = new TreeNode(Segments[i].V0,Segments[i].V1,Inter_Leaf->Right_Child);
				/* Right-Left-Left Child */
				Inter_Leaf->Right_Child->Left_Child->Left_Child = new TreeNode(trapC,Inter_Leaf->Right_Child->Left_Child);
				/* Right-Left-Right Child */
				Inter_Leaf->Right_Child->Left_Child->Right_Child = new TreeNode(trapD,Inter_Leaf->Right_Child->Left_Child);
			}
			/* Remove the old trapezoid */
			trapezoidalList.remove(trapezoidIntersections[0]);
			delete trapezoidIntersections[0];
			trapezoidIntersections[0]=NULL;
		}
		/* 2 or more intersecting trapez's */
		else
		{
			Trapezoid *previousTopPosition, *previousBottomPosition;
			/* Step 1: Updating the 1st trap, it could be 2 or 3 trapz */
			/* case 1/2 */
			if(startPointUsed)
			{
				Trapezoid *trapA, *trapB;
				trapA = new Trapezoid;
				trapB = new Trapezoid;

				trapA->leftPoint = Segments[i].V0;
				trapA->rightPoint = trapezoidIntersections[0]->rightPoint;
				trapA->Top_Segment = trapezoidIntersections[0]->Top_Segment;
				trapA->Bottom_Segment = Segments[i];

				trapB->leftPoint = Segments[i].V0;
				trapB->rightPoint = trapezoidIntersections[0]->rightPoint;
				trapB->Top_Segment = Segments[i];
				trapB->Bottom_Segment = trapezoidIntersections[0]->Bottom_Segment;

				/* Add 2 trap to the list */
				trapezoidalList.push_back(trapA);
				/* Save the prev top trap pointer (trapA) from the trapList */
				previousTopPosition = trapA;
				trapezoidalList.push_back(trapB);
				/* Save the prev bottom trap pointer (trapB) from the trapList */
				previousBottomPosition = trapB;

				/* 2 Sub-Sub-Cases */
				/* case 1/2 */
				if(Classify_PointToLineSegment(trapezoidIntersections[0]->rightPoint,Segments[i].V0,Segments[i].V1)==LEFT)
				{
					trapA->neighbors[0] = trapezoidIntersections[0]->neighbors[0];
					if(trapA->neighbors[0])
						trapA->neighbors[0]->neighbors[2] = trapA;
					trapA->neighbors[2] = trapezoidIntersections[0]->neighbors[2];
					if(trapA->neighbors[2])
						trapA->neighbors[2]->neighbors[0] = trapA;
					trapB->neighbors[1] = trapezoidIntersections[0]->neighbors[1];
					if(trapB->neighbors[1])
						trapB->neighbors[1]->neighbors[3] = trapB;
				}
				/* case 2/2 */
				else
				{
					trapA->neighbors[0] = trapezoidIntersections[0]->neighbors[0];
					if(trapA->neighbors[0])
						trapA->neighbors[0]->neighbors[2] = trapA;
					trapB->neighbors[1] = trapezoidIntersections[0]->neighbors[1];
					if(trapB->neighbors[1])
						trapB->neighbors[1]->neighbors[3] = trapB;
					trapB->neighbors[3] = trapezoidIntersections[0]->neighbors[3];
					if(trapB->neighbors[3])
						trapB->neighbors[3]->neighbors[1] = trapB;
				}
				/* Update the search tree */
				TreeNode *Inter_Leaf = searchTree->Modify_SearchTree(trapezoidIntersections[0]);
				
				/* Repace by Y-Node */
				Inter_Leaf->Type = Y_Node;
				Inter_Leaf->LineSegment_V0 = Segments[i].V0;
				Inter_Leaf->LineSegment_V1 = Segments[i].V1;
				/* Left Child */
				Inter_Leaf->Left_Child = new TreeNode(trapA,Inter_Leaf);
				/* Right Child */
				Inter_Leaf->Right_Child = new TreeNode(trapB,Inter_Leaf);
			}
			/* case 2/2 */
			else
			{
				Trapezoid *trapA, *trapB, *trapC;
				trapA = new Trapezoid;
				trapB = new Trapezoid;
				trapC = new Trapezoid;

				trapA->leftPoint = trapezoidIntersections[0]->leftPoint;
				trapA->rightPoint = Segments[i].V0;
				trapA->Top_Segment = trapezoidIntersections[0]->Top_Segment;
				trapA->Bottom_Segment = trapezoidIntersections[0]->Bottom_Segment;

				trapB->leftPoint = Segments[i].V0;
				trapB->rightPoint = trapezoidIntersections[0]->rightPoint;
				trapB->Top_Segment = trapezoidIntersections[0]->Top_Segment;
				trapB->Bottom_Segment = Segments[i];

				trapC->leftPoint = Segments[i].V0;
				trapC->rightPoint = trapezoidIntersections[0]->rightPoint;
				trapC->Top_Segment = Segments[i];
				trapC->Bottom_Segment = trapezoidIntersections[0]->Bottom_Segment;

				/* Add 3 trapz to the list */
				trapezoidalList.push_back(trapA);
				trapezoidalList.push_back(trapB);
				/* Save the prev top trap pointer (trapB) from the trapList */
				previousTopPosition = trapB;
				trapezoidalList.push_back(trapC);
				/* Save the prev bottom trap pointer (trapC) from the trapList */
				previousBottomPosition = trapC;

				/* 2 Sub-Sub-Cases */
				trapA->neighbors[0] = trapezoidIntersections[0]->neighbors[0];
				trapA->neighbors[1] = trapezoidIntersections[0]->neighbors[1];
				trapA->neighbors[2] = trapB;
				trapA->neighbors[3] = trapC;
				if(trapA->neighbors[0])
					trapA->neighbors[0]->neighbors[2] = trapA;
				if(trapA->neighbors[1])
					trapA->neighbors[1]->neighbors[3] = trapA;

				/* case 1/2 */
				if(Classify_PointToLineSegment(trapezoidIntersections[0]->rightPoint,Segments[i].V0,Segments[i].V1)==LEFT)
				{
					trapB->neighbors[0] = trapA;
					if(trapB->neighbors[0])
						trapB->neighbors[0]->neighbors[2] = trapB;
					trapB->neighbors[2] = trapezoidIntersections[0]->neighbors[2];
					if(trapB->neighbors[2])
						trapB->neighbors[2]->neighbors[0] = trapB;
					trapC->neighbors[1] = trapA;
					if(trapC->neighbors[1])
						trapC->neighbors[1]->neighbors[3] = trapC;
				}
				/* case 2/2 */
				else
				{
					trapB->neighbors[0] = trapA;
					if(trapB->neighbors[0])
						trapB->neighbors[0]->neighbors[2] = trapB;
					trapC->neighbors[1] = trapA;
					if(trapC->neighbors[1])
						trapC->neighbors[1]->neighbors[3] = trapC;
					trapC->neighbors[3] = trapezoidIntersections[0]->neighbors[3];
					if(trapC->neighbors[3])
						trapC->neighbors[3]->neighbors[1] = trapC;
				}
				/* Update the search tree */
				TreeNode *Inter_Leaf = searchTree->Modify_SearchTree(trapezoidIntersections[0]);
				
				/* Replace by X-Node */
				Inter_Leaf->Type = X_Node;
				Inter_Leaf->point = Segments[i].V0;
				/* Left Child */
				Inter_Leaf->Left_Child = new TreeNode(trapA,Inter_Leaf);
				/* Right Child */
				Inter_Leaf->Right_Child = new TreeNode(Segments[i].V0,Segments[i].V1,Inter_Leaf);
				/* Right-Left Child */
				Inter_Leaf->Right_Child->Left_Child = new TreeNode(trapB,Inter_Leaf->Right_Child);
				/* Right-Right Child */
				Inter_Leaf->Right_Child->Right_Child = new TreeNode(trapC,Inter_Leaf->Right_Child);
			}
			/* Step 2: continue with 1 to (last intersected trap) - 1 */
			for(int t=1; t<tcount-1; t++)
			{
				/* 2 Sub-Sub-Cases */
				/* case 1/2 */
				if(Classify_PointToLineSegment(trapezoidIntersections[t]->leftPoint,Segments[i].V0,Segments[i].V1)==LEFT)
				{
					Trapezoid *trapA, *trapB;
					trapA = new Trapezoid;

					trapA->leftPoint = trapezoidIntersections[t]->leftPoint;
					trapA->rightPoint = trapezoidIntersections[t]->rightPoint;
					trapA->Top_Segment = trapezoidIntersections[t]->Top_Segment;
					trapA->Bottom_Segment = Segments[i];

					/* Add 1 trap to the list */
					trapezoidalList.push_back(trapA);

					/* Prev bottom partition to be linked */
					trapB = previousBottomPosition;
					trapB->rightPoint = trapezoidIntersections[t]->rightPoint;

					/* 1 case */
					trapA->neighbors[1] = previousTopPosition;
					if(trapA->neighbors[1])
						trapA->neighbors[1]->neighbors[3] = trapA;
					trapA->neighbors[0] = trapezoidIntersections[t]->neighbors[0];
					if(trapA->neighbors[0])
						trapA->neighbors[0]->neighbors[2] = trapA;

					trapA->neighbors[2] = trapezoidIntersections[t]->neighbors[2];
					if(trapA->neighbors[2])
						trapA->neighbors[2]->neighbors[0] = trapA;

					trapB->neighbors[3] = trapezoidIntersections[t]->neighbors[3];
					if(trapB->neighbors[3])
						trapB->neighbors[3]->neighbors[1] = trapB;

					/* Save trapA for later use */
					previousTopPosition = trapA;
					/* previousBottomPosition is still the same (trapB) */
					//previousBottomPosition = trapB;
					
					/* Update the search tree */
					TreeNode *Inter_Leaf = searchTree->Modify_SearchTree(trapezoidIntersections[t]);
					
					/* Replace by Y-Node */
					Inter_Leaf->Type = Y_Node;
					Inter_Leaf->LineSegment_V0 = Segments[i].V0;
					Inter_Leaf->LineSegment_V1 = Segments[i].V1;
					/* Left Child */
					Inter_Leaf->Left_Child = new TreeNode(trapA,Inter_Leaf);
					/* Right Child */
					Inter_Leaf->Right_Child = new TreeNode(previousBottomPosition,Inter_Leaf);
				}
				/* case 2/2 */
				else
				{
					Trapezoid *trapA, *trapB;
					trapB = new Trapezoid;

					trapA = previousTopPosition;
					trapA->rightPoint = trapezoidIntersections[t]->rightPoint;

					trapB->leftPoint = trapezoidIntersections[t]->leftPoint;
					trapB->rightPoint = trapezoidIntersections[t]->rightPoint;
					trapB->Top_Segment = Segments[i];
					trapB->Bottom_Segment = trapezoidIntersections[t]->Bottom_Segment;

					/* Add 1 trapez to the list */
					trapezoidalList.push_back(trapB);

					/* 1 case */
					trapA->neighbors[2] = trapezoidIntersections[t]->neighbors[2];
					if(trapA->neighbors[2])
						trapA->neighbors[2]->neighbors[0] = trapA;

					trapB->neighbors[0] = previousBottomPosition;
					if(trapB->neighbors[0])
						trapB->neighbors[0]->neighbors[2] = trapB;
					trapB->neighbors[1] = trapezoidIntersections[t]->neighbors[1];
					if(trapB->neighbors[1])
						trapB->neighbors[1]->neighbors[3] = trapB;
					trapB->neighbors[3] = trapezoidIntersections[t]->neighbors[3];
					if(trapB->neighbors[3])
						trapB->neighbors[3]->neighbors[1] = trapB;

					/* Save the trapB for later use (prevtoppos is still the same) */
					previousBottomPosition = trapB;

					/* Update the search tree */
					TreeNode *Inter_Leaf = searchTree->Modify_SearchTree(trapezoidIntersections[t]);
					
					/* Replace by Y-Node */
					Inter_Leaf->Type = Y_Node;
					Inter_Leaf->LineSegment_V0 = Segments[i].V0;
					Inter_Leaf->LineSegment_V1 = Segments[i].V1;
					/* Left Child */
					Inter_Leaf->Left_Child = new TreeNode(previousTopPosition,Inter_Leaf);
					/* Right Child */
					Inter_Leaf->Right_Child = new TreeNode(trapB,Inter_Leaf);
				}
			}
			/* Step 3: end with the last trapezoid */
			int tcount2 = tcount-1;
			
			/* 2 Sub-Cases */
			/* case 1/2 */
			if(endPointUsed)
			{
				/* 2 Sub-Sub-Cases */
				if(Classify_PointToLineSegment(trapezoidIntersections[tcount2]->leftPoint,Segments[i].V0,Segments[i].V1)==LEFT)
				{
					Trapezoid *trapA, *trapB;
					trapA = new Trapezoid;

					trapA->leftPoint = trapezoidIntersections[tcount2]->leftPoint;
					trapA->rightPoint = trapezoidIntersections[tcount2]->rightPoint;
					trapA->Top_Segment = trapezoidIntersections[tcount2]->Top_Segment;
					trapA->Bottom_Segment = Segments[i];

					trapB = previousBottomPosition;
					trapB->rightPoint = trapezoidIntersections[tcount2]->rightPoint;

					trapezoidalList.push_back(trapA);

					/* 1 case */
					trapA->neighbors[0] = trapezoidIntersections[tcount2]->neighbors[0];
					if(trapA->neighbors[0])
						trapA->neighbors[0]->neighbors[2] = trapA;
					trapA->neighbors[1] = previousTopPosition;
					if(trapA->neighbors[1])
						trapA->neighbors[1]->neighbors[3] = trapA;
					trapA->neighbors[2] = trapezoidIntersections[tcount2]->neighbors[2];
					if(trapA->neighbors[2])
						trapA->neighbors[2]->neighbors[0] = trapA;
					trapB->neighbors[3] = trapezoidIntersections[tcount2]->neighbors[3];
					if(trapB->neighbors[3])
						trapB->neighbors[3]->neighbors[1] = trapB;

					/* Update the search tree */
					TreeNode *Inter_Leaf = searchTree->Modify_SearchTree(trapezoidIntersections[tcount2]);
					
					/* Replace by Y-Node */
					Inter_Leaf->Type = Y_Node;
					Inter_Leaf->LineSegment_V0 = Segments[i].V0;
					Inter_Leaf->LineSegment_V1 = Segments[i].V1;
					/* Left Child */
					Inter_Leaf->Left_Child = new TreeNode(trapA,Inter_Leaf);
					/* Right Child */
					Inter_Leaf->Right_Child = new TreeNode(previousBottomPosition,Inter_Leaf);
		
				}
				/* case 2/2 */
				else
				{
					Trapezoid *trapA, *trapB;
					trapB = new Trapezoid;

					trapA = previousTopPosition;
					trapA->rightPoint = trapezoidIntersections[tcount2]->rightPoint;

					trapB->leftPoint = trapezoidIntersections[tcount2]->leftPoint;
					trapB->rightPoint = trapezoidIntersections[tcount2]->rightPoint;
					trapB->Top_Segment = Segments[i];
					trapB->Bottom_Segment = trapezoidIntersections[tcount2]->Bottom_Segment;

					/* Add 1 trapezoid */
					trapezoidalList.push_back(trapB);

					/* 1 case */
					trapA->neighbors[2] = trapezoidIntersections[tcount2]->neighbors[2];
					if(trapA->neighbors[2])
						trapA->neighbors[2]->neighbors[0] = trapA;
					trapB->neighbors[0] = previousBottomPosition;
					trapB->neighbors[1] = trapezoidIntersections[tcount2]->neighbors[1];
					if(trapB->neighbors[0])
						trapB->neighbors[0]->neighbors[2] = trapB;
					if(trapB->neighbors[1])
						trapB->neighbors[1]->neighbors[3] = trapB;
					trapB->neighbors[3] = trapezoidIntersections[tcount2]->neighbors[3];
					if(trapB->neighbors[3])
						trapB->neighbors[3]->neighbors[1] = trapB;

					/* Update the search tree */
					TreeNode *Inter_Leaf = searchTree->Modify_SearchTree(trapezoidIntersections[tcount2]);
					
					/* Replace by Y-Node */
					Inter_Leaf->Type = Y_Node;
					Inter_Leaf->LineSegment_V0 = Segments[i].V0;
					Inter_Leaf->LineSegment_V1 = Segments[i].V1;
					/* Left Child */
					Inter_Leaf->Left_Child = new TreeNode(previousTopPosition,Inter_Leaf);
					/* Right Child */
					Inter_Leaf->Right_Child = new TreeNode(trapB,Inter_Leaf);
				}
			}
			/* case 2/2 */
			else
			{
				Trapezoid *trapA, *trapB, *trapC;
				trapA = new Trapezoid;

				trapA->leftPoint = Segments[i].V1;
				trapA->rightPoint = trapezoidIntersections[tcount2]->rightPoint;
				trapA->Top_Segment = trapezoidIntersections[tcount2]->Top_Segment;
				trapA->Bottom_Segment = trapezoidIntersections[tcount2]->Bottom_Segment;

				/* Add 1 trapezoid */
				trapezoidalList.push_back(trapA);

				/* 2 Sub-Sub-Cases */
				bool above=false;
				/* case 1/2 */
				if(Classify_PointToLineSegment(trapezoidIntersections[tcount2]->leftPoint,Segments[i].V0,Segments[i].V1)==LEFT)
				{
					above=true;
					trapB = new Trapezoid;

					trapB->leftPoint = trapezoidIntersections[tcount2]->leftPoint;
					trapB->rightPoint = Segments[i].V1;
					trapB->Top_Segment = trapezoidIntersections[tcount2]->Top_Segment;
					trapB->Bottom_Segment = Segments[i];

					/* Add 1 trapezoid */
					trapezoidalList.push_back(trapB);

					trapC = previousBottomPosition;
					trapC->rightPoint = Segments[i].V1;
				}	
				/* case 2/2 */
				else
				{
					trapB = previousTopPosition;
					trapB->rightPoint = Segments[i].V1;

					trapC = new Trapezoid;
					trapC->leftPoint = trapezoidIntersections[tcount2]->leftPoint;
					trapC->rightPoint = Segments[i].V1;
					trapC->Top_Segment = Segments[i];
					trapC->Bottom_Segment = trapezoidIntersections[tcount2]->Bottom_Segment;

					/* Add 1 trapezoid */
					trapezoidalList.push_back(trapC);
				}

				/* 2 cases to link the neighbors */
				/* case 1/2: C is the prev partition to be linked */
				if(above)
				{
					trapA->neighbors[0] = trapB;
					trapA->neighbors[1] = trapC;
				}
				/* case 2/2: B is the prev partition to be linked */
				else
				{
					list<Trapezoid *>::iterator it = trapezoidalList.end();
					it--;
					
					trapC = *it;
					trapA->neighbors[0] = trapB;
					trapA->neighbors[1] = trapC;
				}
				trapA->neighbors[2] = trapezoidIntersections[tcount2]->neighbors[2];
				trapA->neighbors[3] = trapezoidIntersections[tcount2]->neighbors[3];
				if(trapA->neighbors[0])
					trapA->neighbors[0]->neighbors[2] = trapA;
				if(trapA->neighbors[1])
					trapA->neighbors[1]->neighbors[3] = trapA;
				if(trapA->neighbors[2])
					trapA->neighbors[2]->neighbors[0] = trapA;
				if(trapA->neighbors[3])
					trapA->neighbors[3]->neighbors[1] = trapA;

				/* C is the prev partition to be linked */
				if(above)
				{
					trapB->neighbors[0] = trapezoidIntersections[tcount2]->neighbors[0];
					if(trapB->neighbors[0])
						trapB->neighbors[0]->neighbors[2] = trapB;
					trapB->neighbors[1] = previousTopPosition;
					if(trapB->neighbors[1])
						trapB->neighbors[1]->neighbors[3] = trapB;
					trapB->neighbors[2] = trapA;
					if(trapB->neighbors[2])
						trapB->neighbors[2]->neighbors[0] = trapB;
					trapC->neighbors[3] = trapA;
					
					TreeNode *Inter_Leaf = searchTree->Modify_SearchTree(trapezoidIntersections[tcount2]);
					
					/* Replace by Y-Node */
					Inter_Leaf->Type = X_Node;
					Inter_Leaf->point = Segments[i].V1;
					/* Right Child */
					Inter_Leaf->Right_Child = new TreeNode(trapA,Inter_Leaf);
					/* Left Child */
					Inter_Leaf->Left_Child = new TreeNode(Segments[i].V0,Segments[i].V1,Inter_Leaf);
					/* Left-Left Child */
					Inter_Leaf->Left_Child->Left_Child = new TreeNode(trapB,Inter_Leaf->Left_Child);
					/* Left-Right Child */
					Inter_Leaf->Left_Child->Right_Child = new TreeNode(previousBottomPosition,Inter_Leaf->Left_Child);
				}
				/* B is the prev partition to be linked */
				else
				{
					trapB->neighbors[2] = trapA;
					trapC->neighbors[0] = previousBottomPosition;
					if(trapC->neighbors[0])
						trapC->neighbors[0]->neighbors[2] = trapC;
					trapC->neighbors[3] = trapA;
					if(trapC->neighbors[3])
						trapC->neighbors[3]->neighbors[1] = trapC;
					trapC->neighbors[1] = trapezoidIntersections[tcount2]->neighbors[1];
					if(trapC->neighbors[1])
						trapC->neighbors[1]->neighbors[3] = trapC;
					
					TreeNode *Inter_Leaf = searchTree->Modify_SearchTree(trapezoidIntersections[tcount2]);
					
					/* Replace by Y-Node */
					Inter_Leaf->Type = X_Node;
					Inter_Leaf->point = Segments[i].V1;
					/* Right Child */
					Inter_Leaf->Right_Child = new TreeNode(trapA,Inter_Leaf);
					/* Left Child */
					Inter_Leaf->Left_Child = new TreeNode(Segments[i].V0,Segments[i].V1,Inter_Leaf);
					/* Left-Left Child */
					Inter_Leaf->Left_Child->Left_Child = new TreeNode(previousTopPosition,Inter_Leaf->Left_Child);
					/* Left-Right Child */
					Inter_Leaf->Left_Child->Right_Child = new TreeNode(trapC,Inter_Leaf->Left_Child);
				}
			}
			/* Remove all the old intersected trapezoids */
			for(int k=0; k<tcount; k++)
			{
				trapezoidalList.remove(trapezoidIntersections[k]);
				delete trapezoidIntersections[k];
				trapezoidIntersections[k]=NULL;
			}
		}
	}
}

/* Function to get the least distant trapezoid */
Trapezoid *Trapezoidal_Algorithm::GetLeastDistantTrapezoid (void)
{
	Trapezoid *Min=NULL;
	list<Trapezoid *>::iterator it = trapezoidalList.begin();
	/* Suppose the first trapezoid as the min */
	Min=(*it);
	/* Take the next one */
	it++;
	/* Loop through the remaining trapezoids */
	for(; it!=trapezoidalList.end(); it++)
	{
		/* if the current trapezoid has a less x-value than the min */
		if((*it)->leftPoint.x<Min->leftPoint.x)
			/* Update the min */
			Min=(*it);
	}
	return Min;
}

/* Recursive function to build the graph nodes */
void Trapezoidal_Algorithm::StatesBuild (Trapezoid *curTrap, int prevStateNbr)
{
	/* Get the center of the current trapezoid */
	curTrap->Compute_Center();
	/* if the current trapezoid hasn't a node yet */
	if(curTrap->stateIndex==-1)
	{
		/* Add a node centered at the trap's center */
		graph->Add_Node(curTrap->Center.x, curTrap->Center.y);
		/* Set its state index */
		curTrap->stateIndex=graph->NodeNbr-1;

		#ifdef REALTIME_BUILDING
			/* Drawing the viewports */
			Init_View(0);
			Draw();
			/*Copy buffer to Video RAM*/
			Graphics_Draw(NULL);
			Graphics_Flip();
			Sleep(100);
		#endif
	}

	/* if it's not the starting trapezoid --> it has a prev */
	if(prevStateNbr!=-1)
	{
		/* Link the current node with the prev node */
		graph->Link_Nodes(curTrap->stateIndex,prevStateNbr);
		/* Link the current node with the prev node */
		graph->Link_Nodes(prevStateNbr,curTrap->stateIndex);

		#ifdef REALTIME_BUILDING
			/* Drawing the viewports */
			Init_View(0);
			Draw();
			/*Copy buffer to Video RAM*/
			Graphics_Draw(NULL);
			Graphics_Flip();
			Sleep(100);
		#endif
	}


	/* Loop through the neighbors of the current trapezoid */
	for(int i=0; i<4; i++)
	{
		if(curTrap->neighborUsed[i]==false)
		{
			/* Tag the current neighbor as used */
			curTrap->neighborUsed[i]=true;
			if(curTrap->neighbors[i])
			{
				/* Modify the boolean of the current neighbor to tag the current trap as used */
				for(int j=0; j<4; j++)
					if(curTrap->neighbors[i]->neighbors[j]==curTrap)
					{
						curTrap->neighbors[i]->neighborUsed[j]=true;
						break;
					}

				Vertex commonCenter;
				float curSegLength, neiSegLength;
				/* To know if we are checking the neighbors 2,3 */
				if(i>1)
				{
					/* Compute the length of the 2 segments */
					/* the length of the right segment of the current trap */
					curSegLength = curTrap->model->GetPrimitives()[3].GetLength();
					/* the length of the left segment of the neighbor */
					neiSegLength = curTrap->neighbors[i]->model->GetPrimitives()[2].GetLength();
					/* Compute the center of the segment having a length less than the current trap */
					if(neiSegLength<curSegLength)
						/* Get the center of the leftsegment of the neighbor */
						commonCenter = curTrap->neighbors[i]->model->GetPrimitives()[2].GetCenter();
					else
						/* Get the center of the rightsegment of the curTrap */
						commonCenter = curTrap->model->GetPrimitives()[3].GetCenter();
				}
				else
				{
					/* Compute the length of the 2 segments */
					/* the length of the left segment of the current trap */
					curSegLength = curTrap->model->GetPrimitives()[2].GetLength();
					/* the length of the right segment of the neighbor */
					neiSegLength = curTrap->neighbors[i]->model->GetPrimitives()[3].GetLength();
					/* Compute the center of the segment having a length less than the current trap */
					if(neiSegLength<curSegLength)
						/* Get the center of the rightsegment of the neighbor */
						commonCenter = curTrap->neighbors[i]->model->GetPrimitives()[3].GetCenter();
					else
						/* Get the center of the rightsegment of the curTrap */
						commonCenter = curTrap->model->GetPrimitives()[2].GetCenter();
				}

				graph->Add_Node(commonCenter.x,commonCenter.y);
				/* create a link TrapCenter-->CommonEdge */
				graph->Link_Nodes(curTrap->stateIndex,graph->NodeNbr-1);
				/* create a link CommonEdge-->TrapCenter */
				graph->Link_Nodes(graph->NodeNbr-1,curTrap->stateIndex);

				#ifdef REALTIME_BUILDING
					/* Drawing the viewports */
					Init_View(0);
					Draw();
					/*Copy buffer to Video RAM*/
					Graphics_Draw(NULL);
					Graphics_Flip();
					Sleep(100);
				#endif
				/* Call recursively with the current neighbor with the current edge's stateindex */
				StatesBuild(curTrap->neighbors[i],graph->NodeNbr-1);
			}
		}
	}
}

/* Function to build the graph and the shortest path */
void Trapezoidal_Algorithm::PathBuilding (void)
{
	/* Get the least distant trapezoid */
	Trapezoid *Least = GetLeastDistantTrapezoid();
	/* Build the graph recursively */
	StatesBuild(Least,-1);
	/* Init the shortest path */
	graph->Init_ShortestPath();
}

/* Function to get the state of the trapezoid containing the vertex */
int Trapezoidal_Algorithm::Get_TrapezoidState (Vertex pt)
{
	/* Init the state to -1 (solid) */
	int state=-1;
	/* Loop through the list of trapezoids */
	for(list<Trapezoid *>::iterator it = trapezoidalList.begin(); it!=trapezoidalList.end(); it++)
	{
		/* classify the current trapezoid with the cursor world position */
		if((*it)->Classify_PtToTrapezoid(pt)==INSIDE)
		{
			/* if the point lies in the current trapezoid --> store its index */
			state=(*it)->stateIndex;
			break;
		}
	}
	/* return the state */
	return state;
}
















