#ifndef _SEARCHTREE_H_
#define _SEARCHTREE_H_

#include <list>
#include "Standard Header.h"
#include "Trapezoid.h"
#include "Support_Functions.h"

enum TreeNode_Type { X_Node, Y_Node, Leaf };

/* TreeNode class */
class TreeNode
{
	public:
			/* The Node's state */
			TreeNode_Type Type;
			/* In case of "X-Node" used to classify 
			a point along the x-axis */
			Vertex point;
			/* In case of "Y-Node" used to classify
			a point against a line segment */
			Vertex LineSegment_V0, LineSegment_V1;
			/* In case of "Leaf" used to store a reference
			to a trapezoid found in the trapezoidalList */
			Trapezoid *trapezoid;

			/* A Pointer to 2 children */
			TreeNode *Left_Child, *Right_Child;
			/* A list of parents (because a node can have many parents */
			vector<TreeNode *> Parents;

			TreeNode (void)
			{
				Left_Child=Right_Child=NULL;
				point.x=point.y=0;
				LineSegment_V0.x=LineSegment_V0.y=0;
				LineSegment_V1.x=LineSegment_V1.y=0;
				trapezoid=NULL;
			}
			/* TreeNode constructor for "X-Node" */
			TreeNode (Vertex pt, TreeNode *Parent) : point(pt)
			{
				Type=X_Node;
				Left_Child=Right_Child=NULL;
				LineSegment_V0.x=LineSegment_V0.y=0;
				LineSegment_V1.x=LineSegment_V1.y=0;
				trapezoid=NULL;
				Parents.push_back(Parent);
			}
			/* TreeNode constructor for "Y-Node" */
			TreeNode (Vertex lineseg_v0, Vertex lineseg_v1, TreeNode *Parent) : LineSegment_V0(lineseg_v0), LineSegment_V1(lineseg_v1)
			{
				Type=Y_Node;
				Left_Child=Right_Child=NULL;
				point.x=point.y=0;
				trapezoid=NULL;
				Parents.push_back(Parent);
			}
			/* TreeNode constructor for "Leaf" */
			TreeNode (Trapezoid *trapez, TreeNode *Parent) : trapezoid(trapez)
			{
				Type=Leaf;
				Left_Child=Right_Child=NULL;
				point.x=point.y=0;
				LineSegment_V0.x=LineSegment_V0.y=0;
				LineSegment_V1.x=LineSegment_V1.y=0;
				Parents.push_back(Parent);
			}
			/* TreeNode constructor for "Leaf" */
			TreeNode (Trapezoid *trapez) : trapezoid(trapez)
			{
				Type=Leaf;
				Left_Child=Right_Child=NULL;
				point.x=point.y=0;
				LineSegment_V0.x=LineSegment_V0.y=0;
				LineSegment_V1.x=LineSegment_V1.y=0;
			}
};

/* SearchTree class */
class SearchTree
{
	public:
			/* The tree's root */
			TreeNode *root;

			/* Search Tree constructor that takes the first bounding trapezoid */
			SearchTree (Trapezoid *BoundingTrapezoid)
			{
				root = new TreeNode(BoundingTrapezoid);
			}

			/* Function to check which trapezoid in the tree is intersected by the segment */
			Trapezoid * TrapezoidSearch (Vertex pointS, Vertex pointE)
			{
				TreeNode *temp = root;

				/* While !NULL */
				while(temp)
				{
					/* Check the current node's type */
					switch(temp->Type)
					{
						/* "X-Node" */
						case X_Node:
							/* because of the line segment's general positions strategy
							the points should not have the same x */
							if(pointS.x < temp->point.x)
								temp=temp->Left_Child;
							else
								temp=temp->Right_Child;
							break;

						/* "Y-Node" */
						case Y_Node:
							/* Check the classification's result */
							switch(Classify_PointToLineSegment(pointS,temp->LineSegment_V0,temp->LineSegment_V1))
							{
								/* ABOVE */
								case LEFT:
									temp=temp->Left_Child;
									break;

								/* UNDER */
								case RIGHT:
									temp=temp->Right_Child;
									break;

								/* ON */
								case INTERSECT:
								case INTERSECT_A:
								case INTERSECT_B:
									/* Classify the segment's end point with the segment in the Y-Node */
									if(Classify_PointToLineSegment(pointE,temp->LineSegment_V0,temp->LineSegment_V1)==LEFT)
										temp=temp->Left_Child;
									else
										temp=temp->Right_Child;
									break;

							}
							break;

						/* "Leaf" */
						case Leaf:
							return temp->trapezoid;
							break;
					}
				}
				return NULL;
			}

			/* Function to check which trapezoid in the tree is intersected by the segment */
			TreeNode * TreeNodeSearch (Vertex pointS, Vertex pointE)
			{
				TreeNode *temp = root;

				/* While !NULL */
				while(temp)
				{
					/* Check the current node's type */
					switch(temp->Type)
					{
						/* "X-Node" */
						case X_Node:
							/* because of the line segment's general positions strategy
							the points should not have the same x */
							if(pointS.x < temp->point.x)
								temp=temp->Left_Child;
							else
								temp=temp->Right_Child;
							break;

						/* "Y-Node" */
						case Y_Node:
							/* Check the classification's result */
							switch(Classify_PointToLineSegment(pointS,temp->LineSegment_V0,temp->LineSegment_V1))
							{
								/* ABOVE */
								case LEFT:
									temp=temp->Left_Child;
									break;

								/* UNDER */
								case RIGHT:
									temp=temp->Right_Child;
									break;

								/* ON */
								case INTERSECT:
								case INTERSECT_A:
								case INTERSECT_B:
									/* Classify the segment's end point with the segment in the Y-Node */
									if(Classify_PointToLineSegment(pointE,temp->LineSegment_V0,temp->LineSegment_V1)==LEFT)
										temp=temp->Left_Child;
									else
										temp=temp->Right_Child;
									break;

							}
							break;

						/* "Leaf" */
						case Leaf:
							//temp->trapezoid=NULL;
							return temp;
							break;
					}
				}
				return NULL;
			}


			TreeNode * Find_Node (TreeNode *node, Trapezoid *trap)
			{
				if(node->Type == Leaf && node->trapezoid==trap)
				{
					node->trapezoid=NULL;
					return node;
				}
				else
				{
					if(node->Left_Child)
					{
						TreeNode *temp = Find_Node(node->Left_Child,trap);
						if (temp)
							return temp;
					}
					if(node->Right_Child)
					{
						TreeNode *temp = Find_Node(node->Right_Child,trap);
						if (temp)
							return temp;
					}
				}
				return NULL;
			}

			TreeNode * Modify_SearchTree (Trapezoid *ToBeDeleted)
			{
				TreeNode *tmp=root;
				return Find_Node(tmp,ToBeDeleted);
			}
};

#endif